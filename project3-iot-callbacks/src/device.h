#ifndef DEVICE_H
#define DEVICE_H

#include <stddef.h>

#define OWNER_ID "0355272820"
#define DEVICE_NAME_LEN 40
#define INITIAL_CAPACITY 6
#define NUM_TYPES 3

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

#endif