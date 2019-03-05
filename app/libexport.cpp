#include "libexport.h"

libExport::libExport(QWidget *parent) : QWidget(parent)
{
    initExport();
}

void libExport::initExport()
{
    QDir dir;
    dir.mkdir("lib");
    fileExport("libSocket");
    fileExport("libSerial");
    fileExport("libOutput");
    fileExport("libMaster");
    fileExport("libLogger");
}

void libExport::fileExport(QString name)
{
#ifndef __linux__
    name += ".dll";
#else
    name += ".so";
#endif
    QString source = ":/" + name;
    QString target = "./lib/" + name;
#ifndef __linux__
    QFile::remove(target);
    QFile::copy(source, target);
#endif
}

