#-------------------------------------------------
#
# Project created by QtCreator 2018-04-17T11:00:53
#
#-------------------------------------------------

QT       += core gui sql network xml

CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = app-vacuum
TEMPLATE = app

RC_FILE += qrc/appsource.rc

RESOURCES += \
    qrc/appsource.qrc

HEADERS += \
    app/appauthor.h \
    app/appbackup.h \
    app/applogger.h \
    app/appmaster.h \
    app/appsignin.h \
    app/appsystem.h \
    app/apptester.h \
    app/appwindow.h \
    app/boxdialog.h \
    app/boxdouble.h \
    app/boxqitems.h \
    app/boxqlabel.h \
    app/boxqmodel.h \
    app/main.h \
    app/sqlcreate.h \
    app/sqlrecord.h \
    app/sqlsnowid.h \
    app/sqlupload.h \
    app/tcpsocket.h \
    app/typconfig.h \
    app/typsetacw.h \
    app/typsetdcr.h \
    app/typsetimp.h \
    app/sqlexport.h \
    app/sqlimport.h \
    app/typsetmag.h \
    app/typsetinr.h \
    app/udpsocket.h \
    app/typsetlod.h \
    app/boxqimage.h \
    app/typsethal.h \
    app/typsetbmf.h \
    app/appioctrl.h \
    app/devplctrl.h \
    app/devinvert.h \
    app/devdriver.h \
    app/apppermit.h \
    app/typsetind.h \
    app/apprepair.h

SOURCES += \
    app/appauthor.cpp \
    app/appbackup.cpp \
    app/applogger.cpp \
    app/appmaster.cpp \
    app/appsignin.cpp \
    app/appsystem.cpp \
    app/apptester.cpp \
    app/appwindow.cpp \
    app/boxdialog.cpp \
    app/boxdouble.cpp \
    app/boxqitems.cpp \
    app/boxqlabel.cpp \
    app/boxqmodel.cpp \
    app/main.cpp \
    app/sqlcreate.cpp \
    app/sqlrecord.cpp \
    app/sqlsnowid.cpp \
    app/sqlupload.cpp \
    app/tcpsocket.cpp \
    app/typconfig.cpp \
    app/typsetacw.cpp \
    app/typsetdcr.cpp \
    app/typsetimp.cpp \
    app/sqlexport.cpp \
    app/sqlimport.cpp \
    app/typsetmag.cpp \
    app/typsetinr.cpp \
    app/udpsocket.cpp \
    app/typsetlod.cpp \
    app/boxqimage.cpp \
    app/typsethal.cpp \
    app/typsetbmf.cpp \
    app/appioctrl.cpp \
    app/devplctrl.cpp \
    app/devinvert.cpp \
    app/devdriver.cpp \
    app/apppermit.cpp \
    app/typsetind.cpp \
    app/apprepair.cpp


