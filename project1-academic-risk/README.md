# Project 1: Dynamic Academic Performance and Risk Analysis Engine

## Build and run
gcc -Wall -Wextra -std=c99 -o academic src/*.c
./academic

## Files
- src/student.h: structs, constants, prototypes
- src/main.c: header, menu, function-pointer dispatch table
- src/records.c: add, display, sort, search, remove, memory growth/shrink
- src/analysis.c: average, top student, Academic Risk Index, summary
- docs/custom_analysis.md: explanation of the custom analysis
- docs/demo_input.txt, docs/sample_output.txt: scripted demo run

## Evidence
Reproduce the sample output with:
./academic < docs/demo_input.txt
It shows memory growing from 12 to 24 when a student is added, shrinking from
24 to 12 when removed, duplicate-ID and invalid-grade rejection, and
all memory released on exit.
