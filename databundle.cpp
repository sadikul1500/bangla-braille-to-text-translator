#include "databundle.h"

DataBundle::DataBundle(QImage img)
{
    this->image = img;
    this->binChar = "";
    this->isValidDot = true;  // false means it is a black dot but may be noise bcz width and height is too small
    this->numberOfDotInCh = 0;
    this->isLargeDot = false;
    this->isSmallDot = false;
}

DataBundle::DataBundle()
{
    this->binChar = "";
    this->isValidDot = false;  // false means it is a black dot but may be noise bcz width and height is too small
    this->numberOfDotInCh = 0;
    this->isLargeDot = false;
    this->isSmallDot = false;
}

bool DataBundle::isValidCh()
{
    if (isAllZero(0,2) || isAllZero(3,5)) return  false;
    return  true;
}

bool DataBundle::isAllZero(int startIdx, int endIdx)
{
    for(int i=startIdx;i<=endIdx;i++)
    {
        if(binChar[i]=='1') return false;
    }
    return  true;
}
