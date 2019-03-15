/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       库文件部署模块
*******************************************************************************/
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

