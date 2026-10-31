#include <stdio.h>
#include <stdlib.h>
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

/* ---------- Sorting ---------- */

static int cmp_alpha(const void *p1, const void *p2) {
    const struct wordStat *x = p1, *y = p2;
    return strcmp(x->name, y->name);
}

static int cmp_frequency(const void *p1, const void *p2) {
    const struct wordStat *x = p1, *y = p2;

    if (x->count != y->count) {
        return y->count - x->count;
    }

    return strcmp(x->name, y->name);
}

void sort_word_statistics(Analyzer *a) {
    int choice;
    int (*cmp)(const void *, const void *);

    const struct wordStat *p;
    const struct wordStat *end;

    size_t shown = 0;

    if (a->distinct < 2) {
        printf("Not enough words to sort.\n");
        return;
    }

    printf("Sort by: 1) Alphabetical (A-Z)  2) Frequency (high to low)\n");

    if (read_int("Choose: ", &choice) != 0) {
        printf("Invalid choice.\n");
        return;
    }

    if (choice == 1) {
        cmp = cmp_alpha;
    } else if (choice == 2) {
        cmp = cmp_frequency;
    } else {
        printf("Invalid choice.\n");
        return;
    }

    qsort(a->stats, a->distinct, sizeof *a->stats, cmp);

    printf("Sorted %lu words. First 10 entries:\n", (unsigned long)a->distinct);
    end = a->stats + a->distinct;

    for (p = a->stats; p < end && shown < 10; p++, shown++) {
        printf("  %-22s %d\n", p->name, p->count);
    }
    
    printf("Use option 4 to see the full sorted list.\n");
}

static double average_word_length(const Analyzer *a) {
    const struct wordStat *p, *end;
    size_t letters = 0;

    if (a->total_words == 0) {
        return 0.0;
    }
    end = a->stats + a->distinct;

    for (p = a->stats; p < end; p++) {
        letters += strlen(p->name) * (size_t)p->count;
    }

    return (double)letters / (double)a->total_words;
}

static double lexical_diversity(const Analyzer *a) {
    if (a->total_words == 0) {
        return 0.0;
    }
    return 100.0 * (double)a->distinct / (double)a->total_words;
}

static const char *diversity_level(double lds) {
    if (lds >= 70.0) {
        return "Very high";
    }

    if (lds >= 50.0) {
        return "High";
    }
    if (lds >= 30.0) {
        return "Moderate";
    }
    return "Low";
}

static size_t count_hapax(const Analyzer *a) {
    const struct wordStat *p, *end;
    size_t once = 0;

    if (a->distinct == 0) {
        return 0;
    }

    end = a->stats + a->distinct;

    for (p = a->stats; p < end; p++) {
        if (p->count == 1) {
            once++;
        }
    }
    return once;
}

void memory_inspection(Analyzer *a) {
    char query[64];
    const struct wordStat *entry;

    if (a->text.data == NULL || a->stats == NULL || a->distinct == 0) {
        printf("No text or word statistics to inspect.\n");
        return;
    }

    if (read_query("Word to inspect (press Enter for the first entry): ", query, sizeof query) != 0) {
        return;
    }

    if (query[0] == '\0') {
        entry = a->stats;
    } else {
        entry = find_word(a, query);
        if (entry == NULL) {
            printf("\"%s\" not found; using the first entry instead.\n", query);
            entry = a->stats;
        }
    }

    printf("\nMemory Inspection\n");
    printf("--------------------------\n");
    printf("Text buffer address: %p\n", (void *)a->text.data);
    printf("Word statistics array: %p\n", (void *)a->stats);
    printf("Selected word-stat entry: %p  (\"%s\", entry #%ld)\n", (void *)entry, entry->name, (long)(entry - a->stats) + 1);
    printf("Analyzer struct (stack): %p\n\n", (void *)a);

    printf("What each address is:\n");
    printf("- Text buffer: heap block holding the paragraph; %lu bytes allocated, %lu used. Allocated or grown %u time(s).\n",
           (unsigned long)a->text.cap, (unsigned long)a->text.len, a->text.grows);
    printf("- Word statistics: heap block of %lu entries x %lu bytes (%lu in use). Grown %u time(s) after the first allocation.\n",
           (unsigned long)a->stats_cap, (unsigned long)sizeof(struct wordStat), (unsigned long)a->distinct, a->stats_grows);
    printf("- Selected entry: sits %ld bytes after the start of the array (index x sizeof(struct wordStat)).\n", (long)((const char *)entry - (const char *)a->stats));
    printf("- Analyzer struct: a local variable in main() on the stack. It stores the two heap pointers above.\n");
}

void custom_analysis(Analyzer *a) {
    double lds;
    size_t once;

    printf("\n=== Lexical Diversity Score (LDS) ===\n");
    printf("Purpose: shows how varied the vocabulary of the text is.\n");
    printf("Formula: LDS = distinct words / total words x 100\n");
    printf("Levels: 70+ Very high | 50-69 High | 30-49 Moderate | below 30 Low\n");
    printf("Reading: a low score means the same words keep coming back; a high\n");
    printf("score means most words appear only once. Short texts score\n");
    printf("higher because they have less room to repeat.\n\n");

    if (a->total_words == 0) {
        printf("No words to analyse.\n");
        return;
    }

    lds = lexical_diversity(a);
    once = count_hapax(a);

    printf("Total words: %lu\n", (unsigned long)a->total_words);
    printf("Distinct words: %lu\n", (unsigned long)a->distinct);
    printf("LDS: %.1f%% (%s)\n", lds, diversity_level(lds));
    printf("Words used once: %lu (%.1f%% of distinct words)\n", (unsigned long)once, 100.0 * (double)once / (double)a->distinct);
    printf("Words repeated: %lu\n", (unsigned long)(a->distinct - once));
}

void generate_text_summary(Analyzer *a) {
    const struct wordStat *longest, *top;
    size_t length, ties;
    double lds;

    printf("\n========== TEXT SUMMARY ==========\n");
    printf("Source: %s\n", a->source);
    printf("Characters: %lu\n", (unsigned long)a->text.len);

    if (a->total_words == 0) {
        printf("No words found in the text.\n");
        printf("==================================\n");
        return;
    }

    longest = find_longest(a, &length, &ties);
    top = find_most_frequent(a, &ties);
    lds = lexical_diversity(a);

    printf("Total words: %lu\n", (unsigned long)a->total_words);
    printf("Distinct words: %lu\n", (unsigned long)a->distinct);
    printf("Average word length: %.2f characters\n", average_word_length(a));
    printf("Longest word: %s (%lu characters)\n", longest->name, (unsigned long)length);
    printf("Most frequent: %s (%d times)\n", top->name, top->count);

    if (a->truncated_words > 0) {
        printf("Words cut to %d  : %lu\n", WORD_MAX - 1, (unsigned long)a->truncated_words);
    }

    printf("\n-- Custom analysis: Lexical Diversity Score --\n");
    printf("LDS: %.1f%% (%s)\n", lds, diversity_level(lds));
    printf("Words used once: %lu\n", (unsigned long)count_hapax(a));
    printf("==================================\n");
}