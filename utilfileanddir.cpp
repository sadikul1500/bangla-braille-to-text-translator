#include "utilfileanddir.h"

#include <qdir.h>
#include <qsettings.h>
#include <QDebug>
UtilFileAndDir::UtilFileAndDir()
{

}

QString UtilFileAndDir::makeDirec(QDir dir, QString foldNameToBeCrt)
{
//    const QString DEFAULT_DIR_KEY("OPEN_FILE_KEY");
//    QSettings setting("mySoft","braille");
//    setting.beginGroup("Open_File");
//        QFileInfo info(setting.value(DEFAULT_DIR_KEY).toString());
//    setting.endGroup();
   // QString foldNameToBeCrt = "result";
    //QDir dir(info.dir());
    int i=0;
    if (dir.exists())
    {
        bool isDirCreated = dir.mkdir(foldNameToBeCrt);
        if(!isDirCreated)
            for(i=1;!isDirCreated;i++)
                isDirCreated = dir.mkdir(foldNameToBeCrt+QString::number(i));
    }

    if(i)
        dir.cd(foldNameToBeCrt+QString::number(i-1));
    else
        dir.cd(foldNameToBeCrt);

    return dir.filePath("");;
}

QFileInfoList UtilFileAndDir::getAllImage(QString folderPath)
{
    //const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Image folder"));
    QDir dir(folderPath);
    QFileInfoList filelistinfo;
    if(!dir.isEmpty())
    {

        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpeg");
        filter << QLatin1String("*.jpg");
        dir.setNameFilters(filter);
        filelistinfo = dir.entryInfoList();
        /*QStringList fileList;
        foreach (const QFileInfo &fileinfo, filelistinfo) {
            QString imageFile = fileinfo.absoluteFilePath();
            //imageFile is the image path, just put your load image code here
        }*/

    }
    return filelistinfo;
}

QString UtilFileAndDir::getFileName(QString dir, QString name)
{
    if (!QDir(dir).exists()) return "";

    int i=0;
    QRegExp rx("([a-zA-Z0-9_ ])+");
    if(rx.indexIn(name)!=-1)
        name = rx.capturedTexts().at(0);
    QString file = dir + "/" + name + ".txt";
    while(QFile(file).exists()){
        i++;
        file = dir + "/" + name + "_"+QString::number(i) + ".txt";
    }
    return file;
}
