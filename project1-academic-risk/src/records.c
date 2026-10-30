#include <stdlib.h>
#include <stdio.h>
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

static int id_exists(const StudentList *list, int id)
{
    const Student *p;
    const Student *end = list->data + list->count;

    for (p = list->data; p < end; p++)
        if (p->id == id)
            return 1;
    return 0;
}

static int read_grade(const char *prompt, float *out)
{
    char buf[32];
    char *end;
    double value;

    printf("%s", prompt);
    if (read_line(buf, sizeof buf) != 0)
        return -2;
    if (buf[0] == '\0')
        return -1;

    value = strtod(buf, &end);
    if (*end != '\0' || !(value >= 0.0 && value <= 100.0))
        return -1;

    *out = (float)value;
    return 0;
}

static int list_grow(StudentList *list)
{
    size_t new_capacity = list->capacity * 2;
    Student *bigger = realloc(list->data, new_capacity * sizeof(Student));

    if (bigger == NULL)
        return -1;

    list->data = bigger;
    list->capacity = new_capacity;
    return 0;
}

void add_student(StudentList *list)
{
    Student s;
    int rc;

    rc = read_int("Enter student ID (positive number): ", &s.id);
    if (rc != 0 || s.id <= 0) {
        printf("Invalid ID. Student not added.\n");
        return;
    }
    if (id_exists(list, s.id)) {
        printf("Error: ID %d already exists. Student not added.\n", s.id);
        return;
    }

    printf("Enter full name: ");
    if (read_line(s.name, sizeof s.name) != 0 || s.name[0] == '\0') {
        printf("Name cannot be empty. Student not added.\n");
        return;
    }

    printf("Enter course: ");
    if (read_line(s.course, sizeof s.course) != 0 || s.course[0] == '\0') {
        printf("Course cannot be empty. Student not added.\n");
        return;
    }

    if (read_grade("Enter grade (0-100): ", &s.grade) != 0) {
        printf("Invalid grade. It must be a number from 0 to 100.\n");
        return;
    }

    if (list->count == list->capacity) {
        if (list_grow(list) != 0) {
            printf("Error: memory allocation failed. Student not added.\n");
            return;
        }
        printf("(Memory expanded: capacity is now %lu)\n",
               (unsigned long)list->capacity);
    }

    list->data[list->count] = s;
    list->count++;
    printf("Student %d added. Total records: %lu\n",
           s.id, (unsigned long)list->count);
}

void display_students(StudentList *list)
{
    const Student *p;
    const Student *end = list->data + list->count;
    if (list->count == 0) {
        printf("No student records to display.\n");
        return;
    }

    printf("\n%-6s %-22s %-18s %6s\n", "ID", "Name", "Course", "Grade");
    printf("---------------------------------------------------------\n");

    for (p = list->data; p < end; p++)
        printf("%-6d %-22.22s %-18.18s %6.1f\n",
               p->id, p->name, p->course, p->grade);

    printf("---------------------------------------------------------\n");
    printf("Total: %lu record(s)\n", (unsigned long)list->count);
}

void sort_students(StudentList *list) {
    (void)list; printf("[sort: not implemented yet]\n");
}

void search_student(StudentList *list) {
    (void)list; printf("[search: not implemented yet]\n");
}

void remove_student(StudentList *list) {
    (void)list; printf("[remove: not implemented yet]\n");
}