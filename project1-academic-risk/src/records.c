#include <stdlib.h>
#include <string.h>
#include "student.h"

static const Student initial_data[] = {
    {1001, "Kwame Mensah",   "Programming in C", 78.5f},
    {1002, "Ama Owusu",      "Programming in C", 91.0f},
    {1003, "Kofi Boateng",   "Data Structures",  45.0f},
    {1004, "Akosua Darko",   "Data Structures",  62.5f},
    {1005, "Menes Adisso",     "Databases",        38.0f},
    {1006, "Abena Osei",     "Databases",        84.0f},
    {1007, "Kojo Appiah",    "Programming in C", 55.0f},
    {1008, "Esi Quartey",    "Data Structures",  97.5f},
    {1009, "David Achibiri",     "Databases",        49.5f},
    {1010, "Efua Tetteh",    "Programming in C", 70.0f},
    {1011, "Fiifi Annan",    "Data Structures",  33.0f},
    {1012, "Samantha Tchwenko",  "Databases",        66.0f}
};

int list_init(StudentList *list, size_t initial_capacity) {
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;

    if (initial_capacity == 0) {
        return -1;
    }

    list->data = calloc(initial_capacity, sizeof(Student));

    if (list->data == NULL) {
        return -1;
    }

    list->capacity = initial_capacity;
    return 0;
}

int list_load_initial(StudentList *list) {
    size_t n = sizeof(initial_data) / sizeof(initial_data[0]);

    if (n > list->capacity) {
        return -1;
    }

    memcpy(list->data, initial_data, n * sizeof(Student));
    list->count = n;
    return 0;
}

void list_free(StudentList *list) {
    free(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}