#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "textmine.h"

static int read_query(const char *prompt, char *buf, size_t size) {
    char *p;

    printf("%s", prompt);

    if (read_line(buf, size) != 0) {
        return -1;
    }
    for (p = buf; *p != '\0'; p++) {
        *p = (char)tolower((unsigned char)*p);
    }

    return 0;
}

static const struct wordStat *find_word(const Analyzer *a, const char *word) {
    const struct wordStat *p;
    const struct wordStat *end;

    if (a->stats == NULL) {
        return NULL;
    }

    end = a->stats + a->distinct;

    for (p = a->stats; p < end; p++) {
        if (strcmp(p->name, word) == 0) {
            return p;
        }
    }

    return NULL;
}

static const struct wordStat *find_longest(const Analyzer *a, size_t *length, size_t *ties) {
    const struct wordStat *p, *end, *best;

    *length = 0;
    *ties = 0;
    if (a->distinct == 0) {
        return NULL;
    }

    end = a->stats + a->distinct;
    best = a->stats;
    *length = strlen(best->name);

    for (p = a->stats + 1; p < end; p++) {
        size_t len = strlen(p->name);
        if (len > *length) {
            best = p;
            *length = len;
            *ties = 0;
        } else if (len == *length) {
            (*ties)++;
        }
    }

    return best;
}

static const struct wordStat *find_most_frequent(const Analyzer *a, size_t *ties) {
    const struct wordStat *p, *end, *best;

    *ties = 0;
    if (a->distinct == 0) {
        return NULL;
    }

    end = a->stats + a->distinct;
    best = a->stats;

    for (p = a->stats + 1; p < end; p++) {
        if (p->count > best->count) {
            best = p;
            *ties = 0;
        } else if (p->count == best->count) {
            (*ties)++;
        }
    }

    return best;
}

void longest_word(Analyzer *a) {
    size_t length, ties;
    const struct wordStat *w = find_longest(a, &length, &ties);

    if (w == NULL) {
        printf("No words to analyse.\n");
        return;
    }

    printf("Longest word: \"%s\" (%lu characters)\n", w->name, (unsigned long)length);
    
    if (ties > 0) {
        printf("(%lu other word(s) have the same length; the first is shown.)\n", (unsigned long)ties);
    }
}

void most_frequent(Analyzer *a) {
    size_t ties;
    const struct wordStat *w = find_most_frequent(a, &ties);

    if (w == NULL) {
        printf("No words to analyse.\n");
        return;
    }
    printf("Most frequent word: \"%s\" (%d times, %.1f%% of all words)\n", w->name, w->count, 100.0 * w->count / (double)a->total_words);
    if (ties > 0) {
        printf("(%lu other word(s) have the same count; the first is shown.)\n", (unsigned long)ties);
    }
}

void display_word_statistics(Analyzer *a) {
    const struct wordStat *p;
    const struct wordStat *end;
    size_t n = 1;

    if (a->distinct == 0) {
        printf("No words to display.\n");
        return;
    }

    end = a->stats + a->distinct;
    printf("\n%-5s %-22s %s\n", "No.", "Word", "Count");
    printf("-------------------------------------\n");
    for (p = a->stats; p < end; p++, n++) {
        printf("%-5lu %-22s %d\n", (unsigned long)n, p->name, p->count);
    }
    printf("-------------------------------------\n");
    printf("%lu distinct word(s), %lu in total\n", (unsigned long)a->distinct, (unsigned long)a->total_words);
}

void search_word(Analyzer *a) {
    char query[64];
    const struct wordStat *p;

    if (a->distinct == 0) {
        printf("No words to search.\n");
        return;
    }
    if (read_query("Enter a word to search for: ", query, sizeof query) != 0 || query[0] == '\0') {
        printf("No word entered.\n");
        return;
    }

    p = find_word(a, query);
    if (p == NULL) {
        printf("\"%s\" does not appear in the text.\n", query);
    } else {
        printf("\"%s\" appears %d time(s) (%.1f%% of all words), entry #%ld in the list.\n", p->name, p->count, 100.0 * p->count / (double)a->total_words, (long)(p - a->stats) + 1);
    }
}

void sort_word_statistics(Analyzer *a) { 
    (void)a;
    printf("[sort: not implemented yet]\n"); 
}

void generate_text_summary(Analyzer *a) { 
    (void)a; 
    printf("[summary: not implemented yet]\n"); 
}

void custom_analysis(Analyzer *a) { 
    (void)a; 
    printf("[custom: not implemented yet]\n"); 
}

void memory_inspection(Analyzer *a) { 
    (void)a; printf("[memory: not implemented yet]\n"); 
}