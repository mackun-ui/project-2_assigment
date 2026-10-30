#ifndef DEVICE_H
#define DEVICE_H

#include <stddef.h>

#define OWNER_ID "0355272820"
#define DEVICE_NAME_LEN 40
#define INITIAL_CAPACITY 6
#define NUM_TYPES 3

#define TEMP_SIM_MIN 10.0f
#define TEMP_SIM_MAX 90.0f
#define TEMP_LIMIT_MIN 15.0f
#define TEMP_LIMIT_MAX 70.0f

#define PRESSURE_SIM_MIN 80
#define PRESSURE_SIM_MAX 160
#define PRESSURE_LIMIT_MIN 90
#define PRESSURE_LIMIT_MAX 145

#define VOLTAGE_SIM_MIN 10.0f
#define VOLTAGE_SIM_MAX 14.5f
#define VOLTAGE_LIMIT_MIN 11.0f
#define VOLTAGE_LIMIT_MAX 14.2f
#define DRIFT_LIMIT_TEMP 40.0f
#define DRIFT_LIMIT_PRESSURE 40.0f
#define DRIFT_LIMIT_VOLTAGE 2.5f

typedef enum {
    TYPE_TEMPERATURE = 0,
    TYPE_PRESSURE = 1,
    TYPE_BATTERY = 2
} DeviceType;

typedef enum {
    STATUS_NORMAL = 0,
    STATUS_ALERT = 1
} DeviceStatus;

union Reading {
    float temperature;
    int pressure;
    float voltage;
};

struct Device {
    int id;
    char name[DEVICE_NAME_LEN];
    DeviceType type;
    union Reading reading;
    DeviceStatus status;
    int has_reading;
    int pending;
    float prev_value;
    int has_prev;
};

typedef struct {
    struct Device *data;
    size_t count;
    size_t capacity;
} DeviceList;

typedef void (*MenuAction)(DeviceList *list);

int read_line(char *buf, size_t size);
int read_int(const char *prompt, int *out);

int list_init(DeviceList *list, size_t initial_capacity);
int list_load_initial(DeviceList *list);
void list_free(DeviceList *list);
void register_device(DeviceList *list);
void display_devices(DeviceList *list);
void remove_device(DeviceList *list);

void generate_readings(DeviceList *list);
void process_readings(DeviceList *list);
void display_alerts(DeviceList *list);
void monitoring_summary(DeviceList *list);

const char *type_name(DeviceType type);
const char *type_unit(DeviceType type);
const char *status_name(DeviceStatus status);
void format_reading(const struct Device *d, char *buf, size_t size);

typedef void (*MonitorCallback)(struct Device *);

struct Alert {
    int device_id;
    char device_name[DEVICE_NAME_LEN];
    DeviceType type;
    char value_text[32];
    char range[48];
    char reason[64];
    const char *callback;
};

struct Monitor {
    struct Alert *alerts;
    size_t alert_count;
    size_t alert_capacity;
    unsigned long readings_processed;
    unsigned long normal_readings;
    unsigned long abnormal_readings;
    unsigned long alerts_by_type[NUM_TYPES];
    unsigned long drift_checks;
    unsigned long drift_alerts;
    float max_drift;
    int max_drift_id;
};
extern struct Monitor monitor;

void process_device(struct Device *device, void (*callback)(struct Device *));
void dispatch_reading(struct Device *device);
void raise_alert(struct Device *d, const char *callback, const char *range, const char *reason);
const char *callback_name(DeviceType type);
void temperature_monitor(struct Device *d);
void pressure_monitor(struct Device *d);
void battery_monitor(struct Device *d);
void monitor_free(void);
void sensor_drift_detector(struct Device *d);

#endif