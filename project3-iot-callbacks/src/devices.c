#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device.h"

static const struct Device initial_data[] = {
    { .id = 101, .name = "Boiler Temp Sensor",   .type = TYPE_TEMPERATURE },
    { .id = 102, .name = "Greenhouse Temp",      .type = TYPE_TEMPERATURE },
    { .id = 201, .name = "Pipeline Pressure",    .type = TYPE_PRESSURE },
    { .id = 202, .name = "Tank Pressure",        .type = TYPE_PRESSURE },
    { .id = 301, .name = "Solar Battery Bank",   .type = TYPE_BATTERY },
    { .id = 302, .name = "Sensor Node Battery",  .type = TYPE_BATTERY }
};

int list_init(DeviceList *list, size_t initial_capacity) {
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;

    if (initial_capacity == 0) {
        return -1;
    }

    list->data = calloc(initial_capacity, sizeof(struct Device));
    if (list->data == NULL) {
        return -1;
    }

    list->capacity = initial_capacity;
    return 0;
}

int list_load_initial(DeviceList *list) {
    size_t n = sizeof(initial_data) / sizeof(initial_data[0]);

    if (n > list->capacity) {
        return -1;
    }

    memcpy(list->data, initial_data, n * sizeof(struct Device));
    list->count = n;
    return 0;
}

void list_free(DeviceList *list) {
    free(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* temp */
void register_device(DeviceList *list) {
    (void)list;
    printf("[register: not implemented yet]\n");
}
void display_devices(DeviceList *list) { 
    (void)list; 
    printf("[display: not implemented yet]\n"); 
}
void remove_device(DeviceList *list) { 
    (void)list; 
    printf("[remove: not implemented yet]\n"); 
}