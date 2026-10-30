#include <stddef.h>
#ifndef STUDENT_H
#define STUDENT_H

#define OWNER_ID    "0355272820"
#define NAME_LEN    50
#define COURSE_LEN  40
#define PASS_THRESHOLD 50.0f

typedef struct {
    int id;
    char name[NAME_LEN];
    char course[COURSE_LEN];
    float grade;
} Student;

typedef struct {
    Student *data;
    size_t count;
    size_t capacity;
} StudentList;

int list_init(StudentList *list, size_t initial_capacity);
int list_load_initial(StudentList *list);
void list_free(StudentList *list);

#endif