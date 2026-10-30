#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "student.h"

static void print_header(void) {
    printf("====================================================\n");
    printf("Student ID: %s\n", OWNER_ID);
    printf("Project: Dynamic Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index\n");
    printf("====================================================\n");
}

int read_line(char *buf, size_t size) {
    size_t len;

    if (fgets(buf, (int)size, stdin) == NULL)
        return -1;
    
    len = strlen(buf);

    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 0;
}

int read_int(const char *prompt, int *out) {
    char buf[32];
    char *end;
    long value;

    printf("%s", prompt);

    if (read_line(buf, sizeof buf) != 0)
        return -2;
    if (buf[0] == '\0')
        return -1;
    
    value = strtol(buf, &end, 10);
    if (*end != '\0' || value < INT_MIN || value > INT_MAX)
        return -1;

    *out = (int)value;
    return 0;
}

static void print_menu(void)
{
    printf("\n----------------- MENU -----------------\n");
    printf(" 1. Add a student\n");
    printf(" 2. Display all students\n");
    printf(" 3. Sort students\n");
    printf(" 4. Calculate average grade\n");
    printf(" 5. Find top-performing student\n");
    printf(" 6. Search for a student\n");
    printf(" 7. Remove a student\n");
    printf(" 8. Run custom analysis (Academic Risk Index)\n");
    printf(" 9. Display summary report\n");
    printf(" 0. Exit\n");
    printf("----------------------------------------\n");
}

int main(void) {
    StudentList list;
    int choice, rc;

    static const MenuAction actions[] = {
        add_student, display_students, sort_students, calculate_average,
        find_top_student, search_student, remove_student, custom_analysis,
        generate_summary_report
    };
    const size_t n_actions = sizeof actions / sizeof actions[0];

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

    for (;;) {
        print_menu();
        rc = read_int("Choose an option: ", &choice);

        if (rc == -2)
            break;
        if (rc == -1) {
            printf("Invalid input. Enter a number from the menu.\n");
            continue;
        }
        if (choice == 0)
            break;
        if (choice < 1 || (size_t)choice > n_actions) {
            printf("Option %d does not exist.\n", choice);
            continue;
        }

        actions[choice - 1](&list);
    }

    list_free(&list);
    printf("Memory released. Goodbye.\n");
    return 0;
}