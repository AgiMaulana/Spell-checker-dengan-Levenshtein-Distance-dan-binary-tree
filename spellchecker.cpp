#include <iostream>
#include <string>
#include <string.h>
#include <windows.h>
#include <sstream>
#include <stack>
#include "spellchecker.h"
using namespace std;

int LevenshteinDistance(string word1, string word2){
    if(word1 == word2) return 0;
    const int m(word1.size());
    const int n(word2.size());

      if( m==0 ) return n;
      if( n==0 ) return m;

      int *costs = new int[n + 1];

      for( int k=0; k<=n; k++ ) costs[k] = k;

      int i = 0;
      for ( std::string::const_iterator it1 = word1.begin(); it1 != word1.end(); ++it1, ++i )
      {
        costs[0] = i+1;
        int corner = i;

        int j = 0;
        for ( std::string::const_iterator it2 = word2.begin(); it2 != word2.end(); ++it2, ++j )
        {
          int upper = costs[j+1];
          if( *it1 == *it2 )
          {
              costs[j+1] = corner;
          }
          else
          {
            int t(upper<corner?upper:corner);
            costs[j+1] = (costs[j]<t?costs[j]:t)+1;
          }

          corner = upper;
        }
      }

      int result = costs[n];
      delete [] costs;

      return result;
}

//----------------------------------------------------


Word *newWord(string word){
    Word *newWord = new Word;
    newWord->word = word;
    newWord->Parent = NULL;
    newWord->Left = NULL;
    newWord->Right = NULL;

    return newWord;
}

//----------------------------------------------------

Word *insertWord(Word *parent, string word){
    static Word *w;
    Word *nWord;

    if(parent == NULL){
        nWord = newWord(word);
        nWord->Parent = w;
        return nWord;
    }

    int distance = LevenshteinDistance(parent->word, word);
    w = parent;
    if(distance < parent->word.length()/2)
        parent->Left = insertWord(parent->Left, word);
    else
        parent->Right = insertWord(parent->Right, word);

    return parent;
}

Word *searchWord(Word *parent, string word){
    if(parent == NULL) return NULL;

    string wParent = parent->word;
    int distance = LevenshteinDistance(wParent,word);

    if (distance <= 2) return parent;

    if(word.length() <= wParent.length())
        parent->Left = searchWord(parent->Left, word);
    else
        parent->Right = searchWord(parent->Right, word);

    return parent;

}

//----------------------------------------------------

void spellCheck(Word *wRoot, string word){
    if(wRoot == NULL) return;

    //Word *w = searchWord(wRoot, word);

    int distance = LevenshteinDistance(word,wRoot->word);

    if(distance > 0 && distance <= wRoot->word.length()/2)
        if(wRoot->word.length() != 1)
            cout << wRoot->word << endl;

        spellCheck(wRoot->Left, word);
        spellCheck(wRoot->Right, word);
}

void initDatas(Word *parent){

    string subjek[] = {"aku","dia","saya","kamu","kami","kita","kalian","mereka"};
    for(int i=0; i<8; i++)
        insertWord(parent, subjek[i]);

   string kata_kerja[] = {"mau","makan","minum","belajar","tulis","menulis","baca","membaca","makan","belanja","berbelanja","membelanjakan"};
   for(int i=0; i<12; i++)
        insertWord(parent, kata_kerja[i]);

   string kata_tanya[] = {"apa","apakah","siapa","siapakah","dimana","dimanakah","kenapa","mengapa"};
   for(int i=0; i<8; i++)
        insertWord(parent, kata_tanya[i]);

   string objek[] = {"bola","sepeda","layang-layang","mainan","kertas"};
   for(int i=0; i<5; i++)
        insertWord(parent, objek[i]);

   string kata_sambung[] = {"di","dan","atau","yang","jika","kalau","pada"};
   for(int i=0; i<7; i++)
        insertWord(parent, kata_sambung[i]);

    insertWord(parent, "berenang");
    /*const int max_item = 5;
    string a[max_item] = {"ayam","asam","aman","anak","ampun"};
    string b[max_item] = {"bawang","bakwan","balon","batu","bacan"};
    string c[max_item] = {"cendol","cicak","cemara","curang","cuaca"};
    string d[max_item] = {"dagang","daging","daun","dahan","dorong"};
    string e[max_item] = {"enak","empat","encer","ember","empang"};

    for(int i=0; i < max_item; i++){
        insertWord(parent, a[i]);
    }
    for(int i=0; i < max_item; i++){
        insertWord(parent, b[i]);
    }
    for(int i=0; i < max_item; i++){
        insertWord(parent, c[i]);
    }
    for(int i=0; i < max_item; i++){
        insertWord(parent, d[i]);
    }
    for(int i=0; i < max_item; i++){
        insertWord(parent, e[i]);
    }*/
}

bool isWrong(Word *parent, string word){

    // Create an empty stack and push root to it
    stack<Word *> nodeStack;
    nodeStack.push(parent);
    bool iswrong = false;

    /* Pop all items one by one. Do following for every popped item
       a) print it
       b) push its right child
       c) push its left child
    Note that right child is pushed first so that left is processed first */
    while (!nodeStack.empty())
    {
        // Pop the top item from stack and print it
        struct Word *node = nodeStack.top();
        if(LevenshteinDistance(word, node->word) == 0)
            return false;
        nodeStack.pop();

        // Push right and left children of the popped node to stack
        if (node->Right)
            nodeStack.push(node->Right);
        if (node->Left)
            nodeStack.push(node->Left);
    }
    return true;
}

int wordCount(string word){
    string str = word;
    string delimiter = " ";
    int i = 0;
    size_t pos = 0;
    string token;
    while ((pos = word.find(delimiter)) != string::npos) {
        token = word.substr(0, pos);
        i++;
        word.erase(0, pos + delimiter.length());
    }
    i++;
    int j = str.length()-1;
    while(str.at(j) == ' '){
        if(str.at(j) != ' ')
            break;
        i--;
        j--;
    }

    return i;
}

void split(Word *parent, string word){
    string str = word;
    string delimiter = " ";

    string strArray[wordCount(word)];
    int i = 0;

    size_t pos = 0;
    string token;
    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
        strArray[i] = token;
        i++;
        str.erase(0, pos + delimiter.length());
    }
    strArray[i] = str;
    int wrongspell = 0;
    for(int j = 0 ; j<=i; j++){
        if(isWrong(parent, strArray[j]))
            wrongspell++;
    }

    if(wrongspell == 0){
        cout << "Ejaan benar.";
    }else{
        cout << "Terdeteksi " << wrongspell << " kata salah ejaan : \n\n";
        for(int j = 0 ; j<=i; j++)
            if(isWrong(parent, strArray[j])){
                spellCheck(parent, strArray[j]);
                cout << "\n";
            }
        cout << "\nJika tidak tersedia kata yang mirip\n"
             << "mungkin kata yang Anda input tidak tersedia dalam sistem kami\n"
             << "atau Anda menginput bukan kata.";
    }
}
