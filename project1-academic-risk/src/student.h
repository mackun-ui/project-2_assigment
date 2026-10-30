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

#endif