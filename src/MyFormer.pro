#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T14:30:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyFormer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    gotocelldialog.cpp \
    sortdialog.cpp \
    spreadsheet.cpp \
    cell.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    gotocelldialog.h \
    sortdialog.h \
    spreadsheet.h \
    cell.h

FORMS    += mainwindow.ui \
    finddialog.ui \
    gotocelldialog.ui \
    sortdialog.ui

OTHER_FILES +=

RESOURCES += \
    myformer.qrc
