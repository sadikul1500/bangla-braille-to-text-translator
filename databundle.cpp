#include "databundle.h"

DataBundle::DataBundle(QImage img)
{
    this->image = img;
    this->binChar = "";
    this->isValidDot = true;  // false means it is a black dot but may be noise bcz width and height is too small
    this->numberOfDotInCh = 0;
    this->isLargeDot = false;
    this->shouldInCharIdentification = true;
    this->isVeryLarge = false;
}

DataBundle::DataBundle()
{
    this->binChar = "";
    this->isValidDot = false;  // false means it is a black dot but may be noise bcz width and height is too small
    this->numberOfDotInCh = 0;
    this->isLargeDot = false;
    this->shouldInCharIdentification = true;
    this->isVeryLarge = false;
}

bool DataBundle::isValidCh()
{
    //if(binChar[0]=='1' && binChar[5]=='1') return true; //diagonal two dots
    //if(binChar[2]=='1' && binChar[3]=='1') return true; //diagonal two dots
    if(countOne(0,2)>1 && countOne(3,5)) return true; // from both side extract three dots
    if(countOne(0,2) && countOne(3,5)>1) return true; // from both side extract three dots
    return  false;
}

int DataBundle::countOne(int startIdx, int endIdx)
{
    int cnt = 0;
    for(int i=startIdx;i<=endIdx;i++)
    {
        if(binChar[i]=='1') cnt++;
    }
    return  cnt;
}
