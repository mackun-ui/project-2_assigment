/* How words are defined (the rules the whole program follows):
   - Case: every letter is converted to lowercase, so "The" = "the".
   - Letters: only A-Z / a-z make up a word.
   - Apostrophe ('): kept INSIDE a word when a letter follows it, so
     "don't" is one word. Quote marks at the start or end of a word, and
     trailing ones like dogs', are dropped.
   - Hyphen (-): kept inside a word when a letter follows it, so
     "well-known" is one word. A dash like "--" separates words.
   - Digits, other punctuation, whitespace and non-ASCII bytes all separate
     words. Numbers are therefore ignored.
   - Words longer than 39 letters are TRUNCATED to their first 39 letters
     (the size of wordStat.name) and counted in truncated_words.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "textmine.h"

#define INITIAL_STATS_CAP 16 

static int next_word(const char **cursor, char *out, size_t out_size, int *was_truncated) {
    const char *p = *cursor;
    size_t n = 0;

    *was_truncated = 0;

    while (*p != '\0' && !isalpha((unsigned char)*p)){
        p++;
    }
    if (*p == '\0') {
        *cursor = p;
        return 0;
    }

    while (*p != '\0') {
        int is_letter = isalpha((unsigned char)*p);
        int is_joiner = (*p == '\'' || *p == '-') && n > 0 && isalpha((unsigned char)p[1]);

        if (!is_letter && !is_joiner) {
            break;
        }

        if (n < out_size - 1) {
            out[n++] = (char)tolower((unsigned char)*p);
        } else {
             *was_truncated = 1;
        }
        p++;
    }

    out[n] = '\0';
    *cursor = p;
    return 1;
}

static int add_word(Analyzer *a, const char *word) {
    struct wordStat *p;
    struct wordStat *end = a->stats + a->distinct;

    for (p = a->stats; p < end; p++) {
        if (strcmp(p->name, word) == 0) {
            p->count++;
            return 0;
        }
    }

    if (a->distinct == a->stats_cap) {
        size_t new_cap = a->stats_cap * 2;
        struct wordStat *bigger = realloc(a->stats, new_cap * sizeof *bigger);

        if (bigger == NULL) {
            return -1;
        }
        a->stats = bigger;
        a->stats_cap = new_cap;
        a->stats_grows++;
    }

    strcpy(a->stats[a->distinct].name, word);
    a->stats[a->distinct].count = 1;
    a->distinct++;
    return 0;
}

/* Rebuild the word statistics from the current text. Returns 0 or -1. */
int build_word_stats(Analyzer *a) {
    const char *cursor;
    char word[WORD_MAX];
    int cut;

    free(a->stats);
    a->stats = malloc(INITIAL_STATS_CAP * sizeof *a->stats);
    a->stats_cap = (a->stats != NULL) ? INITIAL_STATS_CAP : 0;
    a->stats_grows = 0;
    a->distinct = 0;
    a->total_words = 0;
    a->truncated_words = 0;

    if (a->stats == NULL) {
        return -1;
    }
    if (a->text.data == NULL) {
        return 0;
    }

    cursor = a->text.data;
    while (next_word(&cursor, word, sizeof word, &cut)) {
        if (add_word(a, word) != 0) {
            return -1;
        }
        a->total_words++;
        if (cut) {
            a->truncated_words++;
        }
    }
    return 0;
}

void count_words(Analyzer *a) {
    const char *cursor = a->text.data;
    char word[WORD_MAX];
    int cut;
    size_t n = 0;

    if (cursor == NULL) {
        printf("No text loaded.\n");
        return;
    }

    while (next_word(&cursor, word, sizeof word, &cut)) {
        n++;
    }

    printf("Total words: %lu\n", (unsigned long)n);
    printf("Distinct words: %lu\n", (unsigned long)a->distinct);
    if (a->truncated_words > 0) {
        printf("Words cut to %d letters: %lu\n", WORD_MAX - 1, (unsigned long)a->truncated_words);
    }
}