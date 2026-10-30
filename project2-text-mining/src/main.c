#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "textmine.h"

static void print_header(const Analyzer *a) {
    printf("==================================================\n");
    printf("Student ID: %s\n", OWNER_ID);
    printf("Project: Dynamic Text Mining and Linguistic Pattern Analyzer\n");
    printf("Source: %s\n", a->source);
    printf("Custom Analysis: Lexical Diversity Score\n");
    printf("==================================================\n");
}

int read_line(char *buf, size_t size) {
    size_t len;

    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }

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
    if (read_line(buf, sizeof buf) != 0) {
        return -2;
    }
    if (buf[0] == '\0') {
        return -1;
    }

    value = strtol(buf, &end, 10);
    if (*end != '\0' || value < INT_MIN || value > INT_MAX) {
        return -1;
    }

    *out = (int)value;
    return 0;
}

static void print_menu(void) {
    printf("\n----------------- MENU -----------------\n");
    printf("1. Count words\n");
    printf("2. Longest word\n");
    printf("3. Most frequent word\n");
    printf("4. Display word statistics\n");
    printf("5. Sort word statistics\n");
    printf("6. Search for a word\n");
    printf("7. Text summary\n");
    printf("8. Custom analysis (Lexical Diversity Score)\n");
    printf("9. Memory inspection\n");
    printf("10. Enter or paste a new paragraph\n");
    printf("11. Reload the built-in paragraph\n");
    printf("0. Exit\n");
    printf("----------------------------------------\n");
}

int main(void) {
    Analyzer a;
    int choice, rc;

    static const Operation actions[] = {
        count_words, longest_word, most_frequent,
        display_word_statistics, sort_word_statistics, search_word,
        generate_text_summary, custom_analysis, memory_inspection,
        enter_paragraph, reload_builtin_text
    };
    const size_t n_actions = sizeof actions / sizeof actions[0];

    analyzer_init(&a);
    if (load_builtin_text(&a) != 0) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        analyzer_free(&a);
        return 1;
    }

    print_header(&a);

    for (;;) {
        print_menu();
        rc = read_int("Choose an option: ", &choice);

        if (rc == -2) {
            break;
        }
        if (rc == -1) {
            printf("Invalid input. Enter a number from the menu.\n");
            continue;
        }
        if (choice == 0) {
            break;
        }
        if (choice < 1 || (size_t)choice > n_actions) {
            printf("Option %d does not exist.\n", choice);
            continue;
        }

        actions[choice - 1](&a);
    }

    analyzer_free(&a);
    printf("Memory released. Goodbye.\n");
    return 0;
}