#ifndef DOUBLE_MAP_H
#define DOUBLE_MAP_H
#include<iostream>
#include<unordered_map>
#include<vector>
#include "CommonSymbols.h"

using namespace std;

class DoubleMap
{
    private:
        CommonSymbols commonSymbols;
        vector<string> letters;
        int position, bracket_count;

    public:
        DoubleMap(vector<string> letters, int position, int bracket_count)
        {
            this->letters = letters;
            this->position = position;
            this->bracket_count = bracket_count;
            //cout<<"double map"<<endl;
        }

        string getCharFromDoubleMap()
        {
            string character = letters[position];
            int length = letters.size();//sizeof(letters) / sizeof(*letters);
            //cout<<"ok1"<<endl;
            if(character == "011001")
            {
                if (position + 1 == length)
                {
                    return "?";
                }
                return "\“";
            }

             else if(character == "001011")
             {
                if (position + 1 == length ||
                    commonSymbols.getPunctuation().find(letters[position]) != commonSymbols.getPunctuation().end())
                    {
                        return "\"";
                    }
                return "্";
             }

             else if(character == "011011")
             {
                if (bracket_count == 1)
                {
                    bracket_count = 0;
                    return ")";
                }
                bracket_count += 1;
                return "(" ;
             }

             else if(character == "010000")
             {
                // cout<<"ok2"<<endl;
                 if(position + 1 == length || length > 3)
                 {
                     //cout<<"HHoise"<<endl;
                     return ",";
                 }
                 return ".";
             }



        }

        int getBracket_count()
        {
            return bracket_count;
        }
};
#endif // DOUBLE_MAP_H
