#ifndef SPELLCHECKER_H_INCLUDED
#define SPELLCHECKER_H_INCLUDED

#include <iostream>
using namespace std;

typedef struct Word{
    string word;
    Word *Parent;
    Word *Left;
    Word *Right;
};

int LevenshteinDistance(string word1, string word2);

Word *newWord(string word);
Word *insertWord(Word *parent, string word);
Word *searchWord(Word *parent, string word);
bool isWrong(Word *parent, string word);

void spellCheck(Word *WRoot, string word);
void initDatas(Word *parent);

int wordCount(string word);
void split(Word *parent, string word);

#endif // SPELLCHECKER_H_INCLUDED
