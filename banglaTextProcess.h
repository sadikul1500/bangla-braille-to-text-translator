#ifndef BanglaTextProcess_H
#define BanglaTextProcess_H
#include<iostream>
#include<unordered_map>
#include "double_map.h"
#include "bangla.h"
#include "english.h"

using namespace std;

class BanglaTextProcess
{
    private:
        Bangla bangla;
        English english;
        string numeral_sign = "001111";
        string operator_sign = "000011";
        string english_sign = "000001";
        int bracket_count = 0;

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
            return "";//letters[i];
        }


        string checkJointLetter(unordered_map<string, string> dd, vector<string> letters, int *i, int length)
        {
            if(letters[*i] == "000100" && *i+2 < length)
            {
                *i += 2;
                //cout<<bangla.getHosonto().begin()->second<<" "<<bangla.getHosonto().begin()->first<<endl;
                //cout<<2222<<endl;
                return dd[letters[*i+1-2]] + bangla.getHosonto()["001011"] + dd[letters[*i+2-2]];
            }

            else if(letters[*i] == "000101" && *i+4 < length)
            {
                //cout<<4444444444<<endl;
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
                //cout<<2333333<<endl;
                *i += 3;
                return dd[letters[*i + 1-3]] + bangla.getHosonto()["001011"] + dd[letters[*i + 2-3]] + bangla.getHosonto()["001011"] +
                       dd[letters[*i + 3-3]];

            }
            //cout<<"nn0ooo"<<endl;
            return "";

        }


        string englishProcess(vector<string> letters, int *bracket_count, int *i, int* length)
        {
            //cout<<"okkk0"<<endl;
            //cout<<*i+1<<endl;
            //cout<<*length<<endl;
            if(english.getOperator().find(letters[*i]) != english.getOperator().end())
            {
                 //cout<<"okkk1"<<endl;

                 if(letters[*i] == "011011" and *bracket_count == 0)
                 {
                     //cout<<"first"<<endl;
                     *bracket_count += 1;
                     return "(";
                 }

                 else if(letters[*i] == "011011" and *bracket_count == 1)
                 {
                     //cout<<"second"<<endl;
                     *bracket_count = 0;
                     return ")";
                 }

                 else
                 {
                     //cout<<"okkk2"<<endl;
                     return english.getOperator()[letters[*i]];
                 }
            }


            else if(english.getEnglishAlphabets().find(letters[*i]) != english.getEnglishAlphabets().end())
            {
                //cout<<"okkk4"<<endl;
                return english.getEnglishAlphabets()[letters[*i]];
            }

            else if(english.getPunctuation().find(letters[*i]) != english.getPunctuation().end())
            {
                //cout<<"okkk4"<<endl;
                return english.getPunctuation()[letters[*i]];
            }

            else if(*i+1 < *length && english.getOperator().find(letters[*i]+letters[*i+1]) != english.getOperator().end())
            {
                //cout<<"okkk3"<<endl;
                *i++;
                return english.getOperator()[letters[*i-1] + letters[*i+1-1]];
            }

            else return "";

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
//                    return letters[*i];
                    return "";
            }

            else if(*i+1 < *length && bangla.getOperator().find(letters[*i]+letters[*i+1]) != bangla.getOperator().end())
            {
                *i++;
                return bangla.getOperator()[letters[*i-1] + letters[*i+1-1]];
            }


            else
			{
                //cout<<"digit not found\n";
//				return letters[*i];
                return "";
			}
        }


        vector<string> textProcess(vector<string> letters)
        {
            int length = letters.size();//sizeof(letters) / sizeof(*letters);
            bool num = false;
            bool eng = false;
            int i = 0;
            vector<string> text;
            //unordered_map<string, string> hos = bangla.getHosonto();
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

                else if(eng)
                {
                    //cout<<"english!!!"<<endl;
                    text.push_back(englishProcess(letters, &bracket_count, &i, &length));
                }

                else
                {
                    if(i == 0 && letters[i] == numeral_sign)
                    {
                        //cout<<"numeral sign found in bangla"<<endl;
                        num = true;
                    }

                    else if(letters[i] == numeral_sign && i-1 >= 0 && (bangla.getPunctuation().find(letters[i-1]) != bangla.getPunctuation().end()
                            || bangla.getDot().find(letters[i]) != bangla.getDot().end()))
                    {
                        //cout<<"numeral sign found"<<endl;
                        num = true;
                    }

                    else if(i == 0 && letters[i] == english_sign)
                    {
                        //cout<<"english sign found in bangla"<<endl;
                        eng = true;
                    }

                    else if(letters[i] == english_sign && i-1 >= 0 && bangla.getPunctuation().find(letters[i-1]) != bangla.getPunctuation().end()
                            )
                    {
                        //cout<<"english sign found"<<endl;
                        eng = true;
                    }

                    else
                    {
                        if(i + 1 < length && bangla.getTwelveDots().find(letters[i] + letters[i + 1])!= bangla.getTwelveDots().end())
                        {
                            //cout<<"twelve dots"<<endl;
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
                            //cout<<"operator sign"<<endl;
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
                        eng = false;

                    }
                }
                i += 1;
            }
            text.push_back(" ");
            return text;
        }
};
#endif // BanglaTextProcess
