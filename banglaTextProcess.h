#ifndef BANGLATEXTPROCESS_H
#define BANGLATEXTPROCESS_H
#include<iostream>
#include<unordered_map>
#include "double_map.h"
#include "bangla.h"

using namespace std;

class BanglaTextProcess
{
    private:
        Bangla bangla;
        string numeral_sign = "001111";
        string operator_sign = "000011";

    public:
        BanglaTextProcess()
        {
            //cout<<"Bangla text process"<<endl;
        }


        string getText(unordered_map<string, string> dd, vector<string> letters, int i)
        {
            for (pair<string, string> element : dd)
            {
                if(letters[i] == element.first)
                {
                    //cout<<"dd "<<letters[i]<<endl;
                    return element.second;
                }
            }
            //cout<<"missed "<<letters[i]<<endl;
            return letters[i];
        }


        string checkJointLetter(unordered_map<string, string> dd, vector<string> letters, int *i, int length)
        {
            if(letters[*i] == "000100" && *i+2 < length)
            {
                *i += 2;
                //cout<<bangla.getHosonto().begin()->second<<" "<<bangla.getHosonto().begin()->first<<endl;
                return dd[letters[*i+1-2]] + bangla.getHosonto()["001011"] + dd[letters[*i+2-2]];
            }

            else if(letters[*i] == "000101" && *i+4 < length)
            {
                string joint = dd[letters[*i+1]] + dd[letters[*i+2]] + dd[letters[*i+3]] + dd[letters[*i+4]];
                if(bangla.getFourLetters().find(joint) != bangla.getFourLetters().end())
                {
                    for (pair<string, string> element : bangla.getFourLetters())
                    {
                        if(joint == element.first)
                        {
                            *i += 4;
                            return dd[letters[*i+1-4]] + bangla.getHosonto()["001011"] + dd[letters[*i+2-4]] +
                            bangla.getHosonto()["001011"] + dd[letters[*i+3-4]] + bangla.getHosonto()["001011"] + dd[letters[*i+4-4]];

                        }
                    }
                }
            }

            if(letters[*i] == "000101" && *i+3 < length)
            {
                *i += 3;
                return dd[letters[*i + 1-3]] + bangla.getHosonto()["001011"] + dd[letters[*i + 2-3]] + bangla.getHosonto()["001011"] +
                       dd[letters[*i + 3-3]];

            }

            return "";

        }


        string numberProcess(vector<string> letters, int *bracket_count, int *i, int* length)
        {
            if(bangla.getOperator().find(letters[*i]) != bangla.getOperator().end())
            {
                 if(letters[*i] == "011011" and *bracket_count == 0)
                 {
                     *bracket_count += 1;
                     return "(";
                 }

                 else if(letters[*i] == "011011" and *bracket_count == 1)
                 {
                     *bracket_count = 0;
                     return ")";
                 }

                 else
                 {
                     return bangla.getOperator()[letters[*i]];
                 }
            }

            else if(bangla.getNumbers().find(letters[*i]) != bangla.getNumbers().end())
            {
                return bangla.getNumbers()[letters[*i]];
            }

            else if(letters[*i] == operator_sign && *i+1 < *length)
            {
                *i++;
                if(bangla.getMathOperator().find(letters[*i]) != bangla.getMathOperator().end())
                {
                    return bangla.getMathOperator()[letters[*i]];
                }

                else
                    return letters[*i];
            }

            else if(bangla.getOperator().find(letters[*i]+letters[*i+1]) != bangla.getOperator().end() && *i+1 < *length)
            {
                *i++;
                return bangla.getOperator()[letters[*i-1] + letters[*i+1-1]];
            }


            else return "";
        }


        vector<string> textProcess(vector<string> letters)
        {
            int length = letters.size();//sizeof(letters) / sizeof(*letters);
            bool num = false;
            int i = 0, bracket_count = 0;
            vector<string> text;
            unordered_map<string, string> hos = bangla.getHosonto();
            unordered_map<string, string> dd = bangla.getBanglaDictionary();
            unordered_map<string, string> characters = bangla.getAllAlphabets();

            while(i < length)
            {
                //cout<<"btp "<<letters[i]<<endl;
                if(num)
                {
                    //cout<<"number process"<<endl;
                    text.push_back(numberProcess(letters, &bracket_count, &i, &length));
                }

                else
                {
                    if(i == 0 && letters[i] == numeral_sign)
                    {
                        //cout<<"numeral sign found in bangla"<<endl;
                        num = true;
                    }

                    else if(letters[i] == numeral_sign && (bangla.getPunctuation().find(letters[i-1]) != bangla.getPunctuation().end()
                            || bangla.getDot().find(letters[i]) != bangla.getDot().end()))
                    {
                        //cout<<"numeral sign found"<<endl;
                        num = true;
                    }

                    else
                    {
                        if(i + 1 < length && bangla.getTwelveDots().find(letters[i] + letters[i + 1])!= bangla.getTwelveDots().end())
                        {
                            text.push_back(bangla.getTwelveDots()[letters[i] + letters[i + 1]]);
                            i += 1;
                        }

                        else if(bangla.getDouble_mapping().find(letters[i]) != bangla.getDouble_mapping().end())
                        {
                            //cout<<"double mapping"<<endl;
                            DoubleMap doubleMap(letters, i, bracket_count);
                            text.push_back(doubleMap.getCharFromDoubleMap());
                            bracket_count = doubleMap.getBracket_count();
                            //i += 1;
                        }

                        else if(i == 0 && letters[i] == operator_sign && i+1 < length)
                        {
                            text.push_back(bangla.getMathOperator()[letters[i+1]]);
                            i += 1;
                        }

                        else
                        {
                            string txt = checkJointLetter(characters, letters, &i, length);
                            //text += txt;

                            if(txt == "")
                            {
                                //cout<<"last"<<endl;
                                text.push_back(getText(dd, letters, i));
                            }
                            else text.push_back(txt);

                        }
                        num = false;

                    }
                }
                i += 1;
            }
            text.push_back(" ");
            return text;
        }
};
#endif // BANGLATEXTPROCESS_H
