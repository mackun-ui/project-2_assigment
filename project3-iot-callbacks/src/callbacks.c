#include <stdio.h>
#include <stdlib.h>
#include "device.h"

struct Monitor monitor;
static const MonitorCallback type_callbacks[NUM_TYPES] = {
    temperature_monitor, pressure_monitor, battery_monitor
};
static const char *const callback_names[NUM_TYPES] = {
    "temperature_monitor", "pressure_monitor", "battery_monitor"
};

static const MonitorCallback drift_callback = sensor_drift_detector;

const char *callback_name(DeviceType type) {
    int i = (int)type;
    return (i >= 0 && i < NUM_TYPES) ? callback_names[i] : "unknown";
}

static float reading_as_float(const struct Device *d) {
    switch (d->type) {
        case TYPE_TEMPERATURE: return d->reading.temperature;
        case TYPE_PRESSURE: return (float)d->reading.pressure;
        case TYPE_BATTERY: return d->reading.voltage;
        default: return 0.0f;
    }
}

static int alerts_reserve(void) {
    size_t new_capacity;
    struct Alert *bigger;

    if (monitor.alert_count < monitor.alert_capacity) {
        return 0;
    }

    new_capacity = monitor.alert_capacity ? monitor.alert_capacity * 2 : 8;
    bigger = realloc(monitor.alerts, new_capacity * sizeof(struct Alert));
    if (bigger == NULL) {
        return -1;
    }

    monitor.alerts = bigger;
    monitor.alert_capacity = new_capacity;
    return 0;
}

void raise_alert(struct Device *d, const char *callback, const char *range, const char *reason) {
    struct Alert *a;
    int idx = (int)d->type;

    d->status = STATUS_ALERT;
    if (idx >= 0 && idx < NUM_TYPES) {
        monitor.alerts_by_type[idx]++;
    }

    if (alerts_reserve() != 0) {
        printf("Warning: could not record alert (out of memory).\n");
        return;
    }

    a = &monitor.alerts[monitor.alert_count++];
    a->device_id = d->id;
    snprintf(a->device_name, sizeof a->device_name, "%s", d->name);
    a->type = d->type;
    format_reading(d, a->value_text, sizeof a->value_text);
    snprintf(a->range, sizeof a->range, "%s", range);
    snprintf(a->reason, sizeof a->reason, "%s", reason);
    a->callback = callback;
}

void process_device(struct Device *device, void (*callback)(struct Device *)) {
    if (device == NULL || callback == NULL) {
        return;
    }
    callback(device);
}

void dispatch_reading(struct Device *d) {
    int idx = (int)d->type;

    if (idx < 0 || idx >= NUM_TYPES) {
        return;
    }

    d->status = STATUS_NORMAL;
    process_device(d, type_callbacks[idx]);
    process_device(d, drift_callback);

    monitor.readings_processed++;
    if (d->status == STATUS_ALERT) {
        monitor.abnormal_readings++;
    } else {
        monitor.normal_readings++;
    }

    d->prev_value = reading_as_float(d);
    d->has_prev = 1;
    d->pending = 0;
}

void temperature_monitor(struct Device *d) {
    float t;
    char range[48];

    if (d->type != TYPE_TEMPERATURE) {
        return;
    }

    t = d->reading.temperature;
    snprintf(range, sizeof range, "%.0f to %.0f C", TEMP_LIMIT_MIN, TEMP_LIMIT_MAX);

    if (t < TEMP_LIMIT_MIN) {
        raise_alert(d, "temperature_monitor", range, "Temperature below minimum limit");
    } else if (t > TEMP_LIMIT_MAX) {
        raise_alert(d, "temperature_monitor", range, "Temperature above maximum limit");
    }
}

void pressure_monitor(struct Device *d) {
    int p;
    char range[48];

    if (d->type != TYPE_PRESSURE) {
        return;
    }

    p = d->reading.pressure;
    snprintf(range, sizeof range, "%d to %d kPa", PRESSURE_LIMIT_MIN, PRESSURE_LIMIT_MAX);

    if (p < PRESSURE_LIMIT_MIN) {
        raise_alert(d, "pressure_monitor", range, "Pressure below minimum limit");
    } else if (p > PRESSURE_LIMIT_MAX) {
        raise_alert(d, "pressure_monitor", range, "Pressure above maximum limit");
    }
}

void battery_monitor(struct Device *d) {
    float v;
    char range[48];

    if (d->type != TYPE_BATTERY) {
        return;
    }

    v = d->reading.voltage;
    snprintf(range, sizeof range, "%.1f to %.1f V", VOLTAGE_LIMIT_MIN, VOLTAGE_LIMIT_MAX);

    if (v < VOLTAGE_LIMIT_MIN) {
        raise_alert(d, "battery_monitor", range, "Battery voltage too low (deep discharge risk)");
    } else if (v > VOLTAGE_LIMIT_MAX) {
        raise_alert(d, "battery_monitor", range, "Battery voltage too high (overcharge risk)");
    }
}

void sensor_drift_detector(struct Device *d) {
    float current, drift, limit;
    char range[48];
    char reason[64];

    if (!d->has_prev) {
        return;
    }

    switch (d->type) {
        case TYPE_TEMPERATURE: limit = DRIFT_LIMIT_TEMP; break;
        case TYPE_PRESSURE: limit = DRIFT_LIMIT_PRESSURE; break;
        case TYPE_BATTERY: limit = DRIFT_LIMIT_VOLTAGE; break;
        default: return;
    }

    current = reading_as_float(d);
    drift = current - d->prev_value;
    if (drift < 0.0f) {
        drift = -drift;
    }

    monitor.drift_checks++;
    if (drift > monitor.max_drift) {
        monitor.max_drift = drift;
        monitor.max_drift_id = d->id;
    }

    if (drift > limit) {
        monitor.drift_alerts++;
        snprintf(range, sizeof range, "change <= %.1f %s between readings", limit, type_unit(d->type));
        snprintf(reason, sizeof reason, "Sudden change of %.1f %s since previous reading", drift, type_unit(d->type));
        raise_alert(d, "sensor_drift_detector", range, reason);
    }
}

void monitor_free(void) {
    free(monitor.alerts);
    monitor.alerts = NULL;
    monitor.alert_count = 0;
    monitor.alert_capacity = 0;
}