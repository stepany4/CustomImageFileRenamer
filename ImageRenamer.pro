#-------------------------------------------------
#
# Project created by QtCreator 2012-04-21T23:22:35
#
#-------------------------------------------------

QT       += core gui

TARGET = ImageRenamer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    textdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    textdialog.h \
    version.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    textdialog.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    resources.rc


RC_FILE = resources.rc








