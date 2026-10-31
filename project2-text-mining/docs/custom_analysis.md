# Custom Analysis: Lexical Diversity Score (LDS)

## What it measures
How varied the vocabulary of a text is. If a text keeps reusing the same
words, the score is low. If most words appear only once, the score is high.

## Formula
LDS = (distinct words / total words) x 100

Example: the text "The the THE" has 3 words but only 1 distinct word
("the"), so LDS = 1 / 3 x 100 = 33.3.

## How the program works it out
1. Walk through the text with pointers and pull out each word, lowercased
   and without punctuation.
2. Look each word up in the word-statistics array with strcmp(). A new word
   gets a new entry, a repeated one gets its count increased.
3. Total words = words read. Distinct words = entries in the array.
4. Apply the formula. As a second figure the program counts the words that
   appear only once.

## Reading the result
| LDS | Level | Meaning |
|---|---|---|
| 70 or more | Very high | Almost every word is different (or the text is very short) |
| 50 to 69 | High | Varied vocabulary, some repetition |
| 30 to 49 | Moderate | Noticeable repetition |
| below 30 | Low | The same words are used again and again |

## Limitation
The score drops as a text gets longer, because common words like "the" keep
coming back. It is only fair to compare texts of similar length.