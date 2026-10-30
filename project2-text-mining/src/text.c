#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textmine.h"

void analyzer_init(Analyzer *a) {
    memset(a, 0, sizeof *a);
}

void analyzer_free(Analyzer *a) {
    free(a->text.data);
    free(a->stats);
    memset(a, 0, sizeof *a);
}

int load_builtin_text(Analyzer *a) {
    snprintf(a->source, sizeof a->source, "(not loaded yet)");
    return 0;
}

void enter_paragraph(Analyzer *a) {
    (void)a;
    printf("[paste: not implemented yet]\n");
}

void reload_builtin_text(Analyzer *a) {
    (void)a; 
    printf("[reload: not implemented yet]\n");
}