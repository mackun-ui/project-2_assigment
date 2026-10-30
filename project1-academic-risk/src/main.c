#include <stdio.h>
#include <stdlib.h>
#include "student.h"

static void print_header(void) {
    printf("====================================================\n");
    printf("Student ID: %s\n", OWNER_ID);
    printf("Project: Dyname Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index\n");
    printf("====================================================\n");
}

int main(void) {
    StudentList list;

    print_header();

    if (list_init(&list, 12) != 0) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        return 1;
    }
    if (list_load_initial(&list) != 0) {
        fprintf(stderr, "Error: could not load initial data.\n");
        list_free(&list);
        return 1;
    }

    printf("Loaded %lu students (capacity %lu)\n", (unsigned long)list.count, (unsigned long)list.capacity);
    printf("First: %d %s %.1f\n", list.data[0].id, list.data[0].name, list.data[0].grade);

    list_free(&list);
    return 0;
}