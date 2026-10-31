# Project 2: Dynamic Text Mining and Linguistic Pattern Analyzer

## Build and run
```
cd project2-text-mining
gcc -Wall -Wextra -std=c99 -o textmine src/*.c
./textmine
```

## Files
- src/textmine.h: structs (wordStat, TextBuffer, Analyzer), prototypes
- src/main.c: header, menu, function-pointer dispatch table
- src/text.c: dynamic text buffer, built-in and pasted paragraph
- src/words.c: pointer-based word extraction, word stats, count_words
- src/analysis.c: longest, most frequent, display, sort, search, memory
  inspection, Lexical Diversity Score, summary
- src/sample_text.h: the paragraph (Thomas Paine, Common Sense, 1776)
- docs/source_paragraph.txt: paragraph and source details
- docs/text_rules.md: punctuation, apostrophe, hyphen and case rules
- docs/custom_analysis.md: explanation of the custom analysis
- docs/demo_input.txt, docs/sample_output.txt: scripted demo run

## Evidence
Reproduce the sample output with: ./textmine < docs/demo_input.txt
It shows every menu operation, the memory addresses, punctuation and
repeated words (9 total, 7 distinct for a test sentence), a 45-letter word
cut to 39 letters, empty input rejected, and the buffers growing.