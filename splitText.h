#ifndef SPLITTEXT_H
#define SPLITTEXT_H
#include<bits/stdc++.h>

using namespace std;

class SplitText
{
    private:

    public:
        vector<string> split(const string& input, const string& regx)
        {
            regex re(regx);
            sregex_token_iterator first{input.begin(), input.end(), re, -1}, last;

            return {first, last};
        }


};


#endif // SPLITTEXT_H
