#-------------------------------------------------
#
# Project created by QtCreator 2022-01-06T14:01:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = seawave
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        FT.cpp \
        GLCube.cpp \
        GLCylinder.cpp \
        GLItem.cpp \
        GLOrigin.cpp \
        GLSeaSurface.cpp \
        GLSphere.cpp \
        PhillipSpectrum.cpp \
        RenderWidget.cpp \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        FT.h \
        GLCube.h \
        GLCylinder.h \
        GLDrawConfig.h \
        GLItem.h \
        GLOrigin.h \
        GLSeaSurface.h \
        GLSphere.h \
        LightProperty.h \
        MainWindow.h \
        MaterialProperty.h \
        PhillipSpectrum.h \
        RenderWidget.h

FORMS += \
        MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    reserve_code.txt
