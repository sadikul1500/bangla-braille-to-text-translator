#ifndef english_H
#define english_H
#include<iostream>
#include<unordered_map>
#include "CommonSymbols.h"

using namespace std;

class English: public CommonSymbols //inherits common symbols
{
    private:
        unordered_map<string, string> englishDictionary;
        unordered_map<string, string> english_numbers = {
        {"010110", "0"}, {"100000", "1"},
        {"110000", "2"}, {"100100", "3"},
        {"100110", "4"}, {"100010", "5"},
        {"110100", "6"}, {"110110", "7"},
        {"110010", "8"}, {"010100", "9"}
        };
        unordered_map<string, string> english_alphabet = {
        {"100000", "a"}, {"110000", "b"}, {"100100", "c"}, {"100110", "d"}, {"100010", "e"},
        {"110100", "f"}, {"110110", "g"}, {"110010", "h"}, {"010100", "i"}, {"010110", "j"},
        {"101000", "k"}, {"111000", "l"}, {"101100", "m"}, {"101110", "n"}, {"101010", "o"},
        {"111100", "p"}, {"111110", "q"}, {"111010", "r"}, {"011100", "s"}, {"011110", "t"},
        {"101001", "u"}, {"111001", "v"}, {"010111", "w"}, {"101101", "x"}, {"101111", "y"},
        {"101011", "z"}
    };

    public:
        English()
        {
            //cout<<"English"<<endl;
            setEnglishDictionary();
        }

        unordered_map<string, string> getEnglishNumbers()
        {
            return english_numbers;
        }

        unordered_map<string, string> getEnglishAlphabets()
        {
            return english_alphabet;
        }

        void setEnglishDictionary()
        {
            englishDictionary.insert(english_alphabet.begin(), english_alphabet.end());
            englishDictionary.insert(getPunctuation().begin(), getPunctuation().end());
            englishDictionary.insert(getDot().begin(), getDot().end());
            englishDictionary.insert(getOperator().begin(), getOperator().end());
        }

        unordered_map<string, string> getEnglishDictionary()
        {
            return englishDictionary;
        }



};
#endif // english_H
