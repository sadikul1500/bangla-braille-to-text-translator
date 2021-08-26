#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <qimage.h>



class PreProcessor
{
public:
    PreProcessor();
    QImage mainImageToBinImage(QString imageFile);
};

#endif // PREPROCESSOR_H
