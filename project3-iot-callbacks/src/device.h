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

#endif