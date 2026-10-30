#include <stdio.h>
#include "device.h"

static void print_header(void) {
    printf("==================================================\n");
    printf("Student ID: %s\n", OWNER_ID);
    printf("Project: Dynamic IoT Event Monitoring and Callback Engine\n");
    printf("Custom Callback: Sensor Drift Detector\n");
    printf("==================================================\n");
}

int main(void) {
    print_header();

    /* temp */
    printf("sizeof(union Reading) = %lu\n", (unsigned long)sizeof(union Reading));
    printf("sizeof(struct Device) = %lu\n", (unsigned long)sizeof(struct Device));
    return 0;
}