/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据上传
*******************************************************************************/
#ifndef SQLUPLOAD_H
#define SQLUPLOAD_H

#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QPushButton>
#include <QMessageBox>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "main.h"

class SqlUpload : public QWidget
{
    Q_OBJECT
public:
    explicit SqlUpload(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxText();
    void initBoxCtrl();
    void initSettings();
    void saveSettings();
    void recvOpen();
    void recvRead();
    void reload(QString name);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *layout;
    QList<QLineEdit*> texts;
    QTmpMap tmpSet;
    bool isConnected;
    QTmpMap tmpMsg;
    QLabel *text;
};

#endif // SQLUPLOAD_H
