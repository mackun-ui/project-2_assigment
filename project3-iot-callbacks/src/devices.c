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

const char *type_name(DeviceType type) {
    static const char *names[] = { "Temperature", "Pressure", "Battery" };
    int i = (int)type;
    return (i >= 0 && i < NUM_TYPES) ? names[i] : "Unknown";
}

const char *type_unit(DeviceType type) {
    static const char *units[] = { "C", "kPa", "V" };
    int i = (int)type;
    return (i >= 0 && i < NUM_TYPES) ? units[i] : "?";
}

const char *status_name(DeviceStatus status) {
    return status == STATUS_ALERT ? "ALERT" : "NORMAL";
}

void format_reading(const struct Device *d, char *buf, size_t size) {
    if (!d->has_reading) {
        snprintf(buf, size, "--");
        return;
    }
    switch (d->type) {
        case TYPE_TEMPERATURE:
            snprintf(buf, size, "%.1f C", d->reading.temperature);
            break;
        case TYPE_PRESSURE:
            snprintf(buf, size, "%d kPa", d->reading.pressure);
            break;
        case TYPE_BATTERY:
            snprintf(buf, size, "%.2f V", d->reading.voltage);
            break;
        default:
            snprintf(buf, size, "?");
            break;
    }
}

static int id_exists(const DeviceList *list, int id) {
    const struct Device *p;
    const struct Device *end = list->data + list->count;

    for (p = list->data; p < end; p++)
        if (p->id == id)
            return 1;
    return 0;
}

static int list_grow(DeviceList *list) {
    size_t new_capacity = list->capacity * 2;
    struct Device *bigger = realloc(list->data,
                                    new_capacity * sizeof(struct Device));
    if (bigger == NULL)
        return -1;

    list->data = bigger;
    list->capacity = new_capacity;
    return 0;
}

void register_device(DeviceList *list) {
    struct Device d;
    int choice;

    memset(&d, 0, sizeof d);

    if (read_int("Enter device ID (positive number): ", &d.id) != 0 || d.id <= 0) {
        printf("Invalid ID. Device not registered.\n");
        return;
    }
    if (id_exists(list, d.id)) {
        printf("Error: ID %d already exists. Device not registered.\n", d.id);
        return;
    }

    printf("Enter device name: ");
    if (read_line(d.name, sizeof d.name) != 0 || d.name[0] == '\0') {
        printf("Name cannot be empty. Device not registered.\n");
        return;
    }

    printf("Type: 1) Temperature  2) Pressure  3) Battery voltage\n");
    if (read_int("Choose type: ", &choice) != 0 || choice < 1 || choice > NUM_TYPES) {
        printf("Invalid type. Device not registered.\n");
        return;
    }
    d.type = (DeviceType)(choice - 1);

    if (list->count == list->capacity) {
        if (list_grow(list) != 0) {
            printf("Error: memory allocation failed. Device not registered.\n");
            return;
        }
        printf("(Memory expanded: capacity is now %lu)\n",
               (unsigned long)list->capacity);
    }

    list->data[list->count] = d;
    list->count++;
    printf("Device %d registered. Total devices: %lu\n",
           d.id, (unsigned long)list->count);
}

void display_devices(DeviceList *list) {
    const struct Device *p;
    const struct Device *end = list->data + list->count;
    char text[32];

    if (list->count == 0) {
        printf("No devices registered.\n");
        return;
    }

    printf("\n%-6s %-22s %-12s %-12s %-8s\n",
           "ID", "Name", "Type", "Reading", "Status");
    printf("----------------------------------------------------------\n");

    for (p = list->data; p < end; p++) {
        format_reading(p, text, sizeof text);
        printf("%-6d %-22.22s %-12s %-12s %-8s\n",
               p->id, p->name, type_name(p->type), text,
               status_name(p->status));
    }

    printf("----------------------------------------------------------\n");
    printf("Total: %lu device(s)\n", (unsigned long)list->count);
}

static void list_shrink_if_sparse(DeviceList *list) {
    size_t new_capacity = list->capacity / 2;
    struct Device *smaller;

    if (list->capacity <= INITIAL_CAPACITY || list->count > new_capacity) {
        return;
    }
    if (new_capacity < INITIAL_CAPACITY) {
        new_capacity = INITIAL_CAPACITY;
    }

    smaller = realloc(list->data, new_capacity * sizeof(struct Device));
    if (smaller == NULL) {
        return;
    }

    list->data = smaller;
    list->capacity = new_capacity;
    printf("(Memory shrunk: capacity is now %lu)\n", (unsigned long)list->capacity);
}

void remove_device(DeviceList *list) {
    int id;
    struct Device *p;
    struct Device *end = list->data + list->count;

    if (list->count == 0) {
        printf("No devices to remove.\n");
        return;
    }
    if (read_int("Enter ID of device to remove: ", &id) != 0) {
        printf("Invalid ID.\n");
        return;
    }

    for (p = list->data; p < end; p++)
        if (p->id == id) {
            break;
        }

    if (p == end) {
        printf("No device with ID %d found.\n", id);
        return;
    }

    printf("Removed: %d %s\n", p->id, p->name);

    memmove(p, p + 1, (size_t)(end - p - 1) * sizeof(struct Device));
    list->count--;

    printf("Total devices: %lu\n", (unsigned long)list->count);
    list_shrink_if_sparse(list);
}