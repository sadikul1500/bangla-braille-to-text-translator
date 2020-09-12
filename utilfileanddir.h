#ifndef UTILFILEANDDIR_H
#define UTILFILEANDDIR_H

#include <QFileInfoList>
#include <QString>



class UtilFileAndDir
{
public:
    UtilFileAndDir();

public:
    static QString makeDirec(QDir dir,QString foldNameToBeCrt);
    static QFileInfoList getAllImage(QString dir);
    static QString getFileName(QString dir,QString name);
};

#endif // UTILFILEANDDIR_H
