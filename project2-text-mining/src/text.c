#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "textmine.h"
#include "sample_text.h"

#define INITIAL_TEXT_CAP 64

void analyzer_init(Analyzer *a) {
    memset(a, 0, sizeof *a);
}

void analyzer_free(Analyzer *a) {
    free(a->text.data);
    free(a->stats);
    memset(a, 0, sizeof *a);
}

int text_append(TextBuffer *t, const char *s, size_t n) {
    size_t needed = t->len + n + 1;

    if (needed > t->cap) {
        size_t new_cap = t->cap ? t->cap : INITIAL_TEXT_CAP;
        char *bigger;

        while (new_cap < needed) {
            new_cap *= 2;
        }

        bigger = realloc(t->data, new_cap);
        if (bigger == NULL) {
            return -1;
        }

        t->data = bigger;
        t->cap = new_cap;
        t->grows++;
    }

    memcpy(t->data + t->len, s, n);
    t->len += n;
    t->data[t->len] = '\0';
    return 0;
}

void text_free(TextBuffer *t) {
    free(t->data);
    memset(t, 0, sizeof *t);
}

static int install_text(Analyzer *a, TextBuffer *fresh, const char *source) {
    text_free(&a->text);
    a->text = *fresh;
    memset(fresh, 0, sizeof *fresh);
    snprintf(a->source, sizeof a->source, "%s", source);
    return build_word_stats(a);
}

static int has_visible_text(const TextBuffer *t) {
    const char *p;

    if (t->data == NULL) {
        return 0;
    }
    for (p = t->data; *p != '\0'; p++) {
        if (!isspace((unsigned char)*p)) {
            return 1;
        }
    }
    return 0;
}

int load_builtin_text(Analyzer *a) {
    TextBuffer fresh;
    const char *src = SAMPLE_TEXT;
    size_t len = strlen(src);
    size_t pos;

    memset(&fresh, 0, sizeof fresh);

    for (pos = 0; pos < len; pos += 40) {
        size_t n = (len - pos < 40) ? len - pos : 40;
        if (text_append(&fresh, src + pos, n) != 0) {
            text_free(&fresh);
            return -1;
        }
    }
    return install_text(a, &fresh, SAMPLE_TITLE " by " SAMPLE_AUTHOR);
}

static void print_load_result(const Analyzer *a) {
    printf("Loaded %lu characters. Buffer: %lu bytes allocated, grew %u time(s).\n", (unsigned long)a->text.len, (unsigned long)a->text.cap, a->text.grows);
    printf("Words: %lu total, %lu distinct.\n", (unsigned long)a->total_words, (unsigned long)a->distinct);
}

void reload_builtin_text(Analyzer *a) {
    if (load_builtin_text(a) != 0) {
        printf("Error: memory allocation failed. Text not loaded.\n");
        return;
    }
    printf("Built-in paragraph loaded: %s\n", a->source);
    print_load_result(a);
}

void enter_paragraph(Analyzer *a) {
    TextBuffer fresh;
    char chunk[256];
    int at_line_start = 1;

    memset(&fresh, 0, sizeof fresh);
    printf("Paste or type your paragraph. Finish with an empty line.\n");

    while (fgets(chunk, sizeof chunk, stdin) != NULL) {
        size_t n = strlen(chunk);
        int ends_line = (n > 0 && chunk[n - 1] == '\n');

        if (at_line_start && (chunk[0] == '\n' || (chunk[0] == '\r' && chunk[1] == '\n'))) {
            break;    
        }                     

        if (ends_line) {
            chunk[n - 1] = ' ';
        }

        if (text_append(&fresh, chunk, n) != 0) {
            printf("Error: memory allocation failed. Text not loaded.\n");
            text_free(&fresh);
            return;
        }
        at_line_start = ends_line;
    }

    if (!has_visible_text(&fresh)) {
        printf("Empty input: keeping the current text.\n");
        text_free(&fresh);
        return;
    }

    if (install_text(a, &fresh, "Text entered by the user") != 0) {
        printf("Error: memory allocation failed while building word statistics.\n");
        return;
    }
    printf("New paragraph loaded.\n");
    print_load_result(a);
}