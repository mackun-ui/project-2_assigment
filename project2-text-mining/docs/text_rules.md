# How the program treats punctuation, apostrophes, hyphens and case

| Situation | Rule | Example |
|---|---|---|
| Upper/lower case | Everything is lowercased | "The", "the", "THE" count as one word: the (3) |
| Apostrophe inside a word | Kept, one word | don't is one word |
| Quote marks at the start or end | Dropped | 'hello' becomes hello, dogs' becomes dogs |
| Hyphen inside a word | Kept, one word | well-known is one word |
| Dash or double hyphen | Separates words | stop--it's gives stop and it's |
| Punctuation (, ; . ? ! :) | Separates words | fact. The gives fact and the |
| Digits | Not part of words, so they separate words | abc123def gives abc and def; 456 gives nothing |
| Repeated spaces, tabs, newlines | Treated as one separator | "a    b" gives a and b |
| Non-ASCII characters (curly quotes, long dashes) | Separate words | Replace them with plain ' and - before pasting |
| Word longer than 39 letters | Cut to the first 39 letters and counted as "cut" | A 45-letter word is stored as 39 letters |

## Memory rules
- The text buffer starts at 64 bytes and doubles with realloc when full, so
  the text can be any length. Pasted input is read in 255-character chunks.
- The word-statistics array starts at 16 entries and doubles the same way.
- If realloc fails, the old data is kept and the program reports an error.
- Everything is freed before the program exits.