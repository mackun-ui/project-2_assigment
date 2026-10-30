#ifndef TEXTMINE_H
#define TEXTMINE_H

#include <stddef.h>

#define OWNER_ID "0355272820"
#define WORD_MAX 40

struct wordStat {
    char name[WORD_MAX];
    int count;
};

typedef struct {
    char *data;
    size_t len;
    size_t cap;
    unsigned grows;
} TextBuffer;

typedef struct {
    TextBuffer text;
    char source[200];
    struct wordStat *stats;
    size_t distinct;
    size_t stats_cap;
    unsigned stats_grows;
    size_t total_words;
    size_t truncated_words;
} Analyzer;

typedef void (*Operation)(Analyzer *a);

int read_line(char *buf, size_t size);
int read_int(const char *prompt, int *out);

void analyzer_init(Analyzer *a);
void analyzer_free(Analyzer *a);
int  text_append(TextBuffer *t, const char *s, size_t n);
void text_free(TextBuffer *t);
int  load_builtin_text(Analyzer *a);
void enter_paragraph(Analyzer *a);
void reload_builtin_text(Analyzer *a);

int  build_word_stats(Analyzer *a);
void count_words(Analyzer *a);

void longest_word(Analyzer *a);
void most_frequent(Analyzer *a);
void display_word_statistics(Analyzer *a);
void sort_word_statistics(Analyzer *a);
void search_word(Analyzer *a);
void generate_text_summary(Analyzer *a);
void custom_analysis(Analyzer *a);
void memory_inspection(Analyzer *a);

#endif