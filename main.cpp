#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include "spellchecker.h"
using namespace std;

int main()
{
    string word;
    Word *wordRoot = NULL;
    wordRoot = insertWord(wordRoot, "a");
    initDatas(wordRoot);
    int menu;
    do{
        system("cls");
        cout << "1. Spell check\n"
             << "2. Keluar\n\n";
        cout << "Pilih      : "; cin >> menu;
        system("cls");

        if(menu==1){
            cout << "Masukkan kata : ";
            cin.ignore();
            getline(cin, word);
            cout << endl;
            split(wordRoot, word);
        }else{
            cout << "We love you <3";
            menu = 0;
        }
        getch();
    }while(menu != 0);
    return 0;
}
