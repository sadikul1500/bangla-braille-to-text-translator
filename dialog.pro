QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET = Application
INCLUDEPATH += C:\OpenCV\opencv\build\include

LIBS += C:\OpenCV\opencv\Release\bin\libopencv_core430.dll
LIBS += C:\OpenCV\opencv\Release\bin\libopencv_highgui430.dll
LIBS += C:\OpenCV\opencv\Release\bin\libopencv_imgcodecs430.dll
LIBS += C:\OpenCV\opencv\Release\bin\libopencv_imgproc430.dll
LIBS += C:\OpenCV\opencv\Release\bin\libopencv_calib3d430.dll
LIBS += C:\OpenCV\opencv\Release\bin\libopencv_objdetect430.dll


SOURCES += \
    databundle.cpp \
    imagelabel.cpp \
    main.cpp \
    dialog.cpp \
    mainwindow.cpp \
    olistview.cpp \
    progressWidget/progresswidget.cpp \
    ulbrofdot.cpp \
    utilfileanddir.cpp \
    utilimageproc.cpp \
    utilimageprocfordialog.cpp

HEADERS += \
    CommonSymbols.h \
    bangla.h \
    banglaTextProcess.h \
    brailleToBangla.h \
    brailleToText.h \
    databundle.h \
    dialog.h \
    double_map.h \
    imagelabel.h \
    mainwindow.h \
    olistview.h \
    progressWidget/progresswidget.h \
    splitText.h \
    ulbrofdot.h \
    utilfileanddir.h \
    utilimageproc.h \
    utilimageprocfordialog.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
