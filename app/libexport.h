/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       库文件部署模块
*******************************************************************************/
#ifndef LIBEXPORT_H
#define LIBEXPORT_H

#include <QDir>
#include <QFile>
#include <QWidget>

class libExport : public QWidget
{
    Q_OBJECT
public:
    explicit libExport(QWidget *parent = 0);

signals:

public slots:
    void initExport();
    void fileExport(QString name);
};

#endif // LIBEXPORT_H
