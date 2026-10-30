#include <stdio.h>
#include "student.h"

static double compute_average(const StudentList *list) {
    const Student *p;
    const Student *end = list->data + list->count;
    double sum = 0.0;

    for (p = list->data; p < end; p++) {
        sum += p->grade;
    }

    return sum / (double)list->count;
}

static void find_extremes(const StudentList *list, const Student **top, const Student **low) {
    const Student *p;
    const Student *end = list->data + list->count;

    *top = *low = list->data;
    for (p = list->data + 1; p < end; p++) {
        if (p->grade > (*top)->grade) *top = p;
        if (p->grade < (*low)->grade) *low = p;
    }
}

void calculate_average(StudentList *list) {
    if (list->count == 0) {
        printf("No record, so there is no average.\n");
        return;
    }
    printf("Average grade: %.2f (across %lu students)\n", compute_average(list), (unsigned long)list->count);
}

void find_top_student(StudentList *list) {
    const Student *top, *low, *p;
    const Student *end = list->data + list->count;
    
    if (list->count == 0) {
        printf("No records.\n");
        return;
    }

    find_extremes(list, &top, &low);

    printf("Top-performing students(s), grade %.1f:\n", top->grade);
    for (p = list->data; p < end; p++) {
        if (p->grade == top->grade) {
            printf(" %d | %s | %s\n", p->id, p->name, p->course);
        }
    }
}
void custom_analysis(StudentList *list) {
    (void)list; printf("[custom analysis: not implemented yet]\n");
}
void generate_summary_report(StudentList *list) {
    (void)list; printf("[summary: not implemented yet]\n");
}