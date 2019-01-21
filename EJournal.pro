#-------------------------------------------------
#
# Project created by QtCreator 2018-02-09T12:20:43
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EJournal
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


SOURCES += \
        main.cpp \
    userwindow.cpp \
    form.cpp \
    ftp.cpp \
    dbhelper.cpp \
    addlesson.cpp \
    changemark.cpp \
    changedate.cpp \
    student.cpp \
    studentabout.cpp \
    subject.cpp \
    rating.cpp

HEADERS += \
    userwindow.h \
    form.h \
    ftp.h \
    dbhelper.h \
    addlesson.h \
    changemark.h \
    changedate.h \
    student.h \
    studentabout.h \
    subject.h \
    rating.h

FORMS += \
    userwindow.ui \
    form.ui \
    addlesson.ui \
    changemark.ui \
    changedate.ui \
    studentabout.ui \
    rating.ui

win32:RC_ICONS += icon.ico

RESOURCES += \
    res.qrc
