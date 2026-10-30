#include <stdio.h>
#include "student.h"

static void print_header(void) {
    printf("====================================================\n");
    printf("Student ID: %s\n", OWNER_ID);
    printf("Project: Dyname Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index\n");
    printf("====================================================\n");
}

int main(void) {
    print_header();
    return 0;
}