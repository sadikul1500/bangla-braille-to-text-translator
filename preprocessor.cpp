#include "preprocessor.h"
#include<iostream>
#include<string>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core/ocl.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/photo.hpp>

using namespace  std;
using namespace cv;
PreProcessor::PreProcessor()
{

}

QImage PreProcessor::mainImageToBinImage(QString fileName)
{
    //return QImage(imageFile);
    string file = fileName.toStdString(); //if you need std c++ string

    Mat image, gray, blur, median, blackWhite, erosion, dilation;

    image = imread(file, IMREAD_COLOR);
    Rect crop_region(25, 25, image.cols-25, image.rows-25);
    //Rect crop_region(25, 100, image.cols-30, image.rows-130);
    image = image(crop_region);

    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0, 0);
    //medianBlur(blur, median, 5);
    //threshold(blur, blackWhite, 0, 255, THRESH_BINARY | THRESH_OTSU);
    //threshold(blur, blackWhite, 0, 255, THRESH_OTSU);
    adaptiveThreshold(blur, blackWhite, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 13, 6);
    //threshold( blur, blackWhite, 230, 255, THRESH_BINARY );
    erode(blackWhite, erosion, getStructuringElement(MORPH_RECT, Size(5, 5)));
    dilate(erosion, dilation, getStructuringElement(MORPH_RECT, Size(5, 5)));

    string out = "out_bin.png";

    imwrite(out, dilation);
    //QImage imgOut= QImage((uchar*) blackWhite.data, blackWhite.cols, blackWhite.rows, blackWhite.step, QImage::Format_Grayscale8);
    //cout<<endl<<blackWhite<<endl;
    //string outFile = "out_" + file;
    QString str = QString::fromUtf8(out.c_str());
    QImage qimg;
    qimg.load(str, "png");
    //cout<<"qimage loaded"<<endl;

    return qimg;  //QImage(str); // you must convert your image to QImage then return it
}
