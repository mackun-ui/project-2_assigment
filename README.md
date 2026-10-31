# C Assignment: Three Dynamic Projects

Three C programs that use structs, dynamic memory (`malloc`/`calloc`/`realloc`),
pointers and function pointers. Each project is menu-driven and frees all
its memory before exiting.

| # | Folder | Project |
|---|---|---|
| 1 | `project1-academic-risk` | Dynamic Academic Performance and Risk Analysis Engine |
| 2 | `project2-text-mining` | Dynamic Text Mining and Linguistic Pattern Analyzer |
| 3 | `project3-iot-callbacks` | Dynamic IoT Event Monitoring and Callback Engine |

Student ID: 0355272820

## Requirements
A C compiler (gcc) with C99 support. The commands below were tested with
gcc on Windows (MSYS2 / Git Bash).

## Build and run

### Project 1: Academic Risk
```
cd project1-academic-risk
gcc -Wall -Wextra -std=c99 -o academic src/*.c
./academic
```

### Project 2: Text Mining
```
cd project2-text-mining
gcc -Wall -Wextra -std=c99 -o textmine src/*.c
./textmine
```

### Project 3: IoT Callbacks
```
cd project3-iot-callbacks
gcc -Wall -Wextra -std=c99 -o iot src/*.c
./iot
```

Each program can also be run with the scripted demo input, which reproduces
the sample output:
```
./academic < docs/demo_input.txt
./textmine < docs/demo_input.txt
./iot < docs/demo_input.txt
```
Project 3 uses random readings, so its output differs on every run.

## Custom features
- Project 1: Academic Risk Index (`docs/custom_analysis.md`)
- Project 2: Lexical Diversity Score (`docs/custom_analysis.md`)
- Project 3: Sensor Drift Detector callback (`docs/custom_callback.md`)

## Submission evidence
| Project | Sample output | Explanation |
|---|---|---|
| 1 | [sample_output.txt](project1-academic-risk/docs/sample_output.txt) | [custom_analysis.md](project1-academic-risk/docs/custom_analysis.md) |
| 2 | [sample_output.txt](project2-text-mining/docs/sample_output.txt) | [custom_analysis.md](project2-text-mining/docs/custom_analysis.md), [text_rules.md](project2-text-mining/docs/text_rules.md), [source_paragraph.txt](project2-text-mining/docs/source_paragraph.txt) |
| 3 | [sample_output.txt](project3-iot-callbacks/docs/sample_output.txt) | [custom_callback.md](project3-iot-callbacks/docs/custom_callback.md) |