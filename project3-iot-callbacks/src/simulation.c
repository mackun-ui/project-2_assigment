#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "device.h"

static float rand_float(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}

static int rand_int(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

void generate_readings(DeviceList *list) {
    static int seeded = 0;
    struct Device *p;
    struct Device *end = list->data + list->count;
    unsigned long n = 0;

    if (list->count == 0) {
        printf("No devices registered.\n");
        return;
    }
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    for (p = list->data; p < end; p++) {
        switch (p->type) {
            case TYPE_TEMPERATURE:
                p->reading.temperature = rand_float(TEMP_SIM_MIN, TEMP_SIM_MAX);
                break;
            case TYPE_PRESSURE:
                p->reading.pressure = rand_int(PRESSURE_SIM_MIN, PRESSURE_SIM_MAX);
                break;
            case TYPE_BATTERY:
                p->reading.voltage = rand_float(VOLTAGE_SIM_MIN, VOLTAGE_SIM_MAX);
                break;
            default:
                continue;
        }
        p->has_reading = 1;
        p->pending = 1;
        n++;
    }
    printf("Generated %lu new reading(s). Use option 5 to process them.\n", n);
}

void process_readings(DeviceList *list)
{
    struct Device *p;
    struct Device *end = list->data + list->count;
    unsigned long n = 0;
    char text[32];

    printf("\n%-6s %-22s %-12s %-12s %-20s %s\n", "ID", "Name", "Type", "Reading", "Callback", "Result");
    printf("------------------------------------------------------------------------------\n");

    for (p = list->data; p < end; p++) {
        size_t alerts_before;

        if (!p->pending) {
            continue;
        }

        alerts_before = monitor.alert_count;
        dispatch_reading(p);
        format_reading(p, text, sizeof text);

        printf("%-6d %-22.22s %-12s %-12s %-20s ", p->id, p->name, type_name(p->type), text, callback_name(p->type));

        if (p->status == STATUS_NORMAL) {
            printf("NORMAL\n");
        } else {
            size_t i;
            printf("ALERT");
            for (i = alerts_before; i < monitor.alert_count; i++) {
                printf("%s %s (%s)", i == alerts_before ? ":" : ";", monitor.alerts[i].reason, monitor.alerts[i].callback);
            }
            printf("\n");
        }
        n++;
    }

    if (n == 0) {
        printf("No pending readings. Use option 4 first.\n");
    } else {
        printf("Processed %lu reading(s). Total processed so far: %lu\n", n, monitor.readings_processed);
    }
}

void display_alerts(DeviceList *list) {
    const struct Alert *a;
    const struct Alert *end = monitor.alerts + monitor.alert_count;
    unsigned long n = 1;

    (void)list;

    if (monitor.alert_count == 0) {
        printf("No alerts detected so far.\n");
        return;
    }

    printf("\n========== DETECTED ALERTS (%lu) ==========\n", (unsigned long)monitor.alert_count);

    for (a = monitor.alerts; a < end; a++, n++) {
        printf("Alert %lu\n", n);
        printf("Device: %d  %s\n", a->device_id, a->device_name);
        printf("Reading: %s\n", a->value_text);
        printf("Expected: %s\n", a->range);
        printf("Reason: %s\n", a->reason);
        printf("Callback: %s\n", a->callback);
    }
}

static const struct Device *find_device(const DeviceList *list, int id) {
    const struct Device *p;
    const struct Device *end = list->data + list->count;

    for (p = list->data; p < end; p++) {
        if (p->id == id) {
            return p;
        }
    }
    return NULL;
}

void monitoring_summary(DeviceList *list)
{
    int t;

    printf("\n========== MONITORING SUMMARY ==========\n");
    printf("Registered devices: %lu\n", (unsigned long)list->count);
    printf("Readings processed: %lu\n", monitor.readings_processed);
    printf("Normal readings: %lu\n", monitor.normal_readings);
    printf("Abnormal readings: %lu\n", monitor.abnormal_readings);
    printf("Total alerts raised: %lu\n", (unsigned long)monitor.alert_count);

    printf("Alerts per device type:\n");
    for (t = 0; t < NUM_TYPES; t++) {
        printf("%-12s: %lu\n", type_name((DeviceType)t), monitor.alerts_by_type[t]);
    }

    printf("\n-- Custom callback: Sensor Drift Detector --\n");
    printf("Drift checks performed: %lu\n", monitor.drift_checks);
    printf("Drift alerts raised: %lu\n", monitor.drift_alerts);

    if (monitor.drift_checks > 0) {
        const struct Device *d = find_device(list, monitor.max_drift_id);
        if (d != NULL) {
            printf("Largest change seen: %.1f %s (device %d, %s)\n", monitor.max_drift, type_unit(d->type), d->id, d->name);
        } else {
             printf("Largest change seen: %.1f (device %d, since removed)\n", monitor.max_drift, monitor.max_drift_id);
        }
    } else {
        printf("No drift checks yet (needs at least 2 rounds of readings).\n");
    }
    printf("=========================================\n");
}