#ifndef BRAILLETOTEXT_H
#define BRAILLETOTEXT_H
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include "splitText.h"
//#include<unordered_map>

using namespace std;

class BrailleToText
{
    //private:
    //    string* fileList;
    public:
        virtual string getBrailleToText(vector<string> text) =0;
        virtual string postProcess(string text) =0;

        string getText(string file, BrailleToText *brailleToText)
        {
            ifstream iFile(file);
            string line;
            SplitText splitText;
            string text = "";

            while(getline(iFile, line))
            {
                vector<string> word = splitText.split(line, " space ");
                for(int i=0; i<word.size(); i++)
                {
                    vector<string> letters = splitText.split(word[i], " ");
                    text += postProcess(brailleToText->getBrailleToText(letters));
                }
                text += "\n";
            }
            iFile.close();
            return text;
        }




};
#endif // BRAILLETOTEXT_H
