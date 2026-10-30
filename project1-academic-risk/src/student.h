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

typedef void (*MenuAction)(StudentList *list);

int read_line(char *buf, size_t size);
int read_int(const char *prompt, int *out);

void add_student(StudentList *list);
void display_students(StudentList *list);
void sort_students(StudentList *list);
void search_student(StudentList *list);
void remove_student(StudentList *list);

void calculate_average(StudentList *list);
void find_top_student(StudentList *list);
void custom_analysis(StudentList *list);
void generate_summary_report(StudentList *list);

#endif