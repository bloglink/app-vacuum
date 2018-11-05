/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#ifndef SQLRECORD_H
#define SQLRECORD_H

#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QFileDialog>
#include <QProcess>
#include <QGroupBox>
#include <QDateEdit>
#include <QListView>
#include <QComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QShowEvent>
#include <QDateTimeEdit>

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>

#include "main.h"
#include "boxqlabel.h"
#include "boxqmodel.h"

class SqlRecord : public QWidget
{
    Q_OBJECT
public:
    explicit SqlRecord(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTextBar();
    void initSettings();
    void clickSelect();
    void clickExport();
    QString getSaveFileName();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *tView;
    QSqlTableModel *mView;
    QTableView *item;
    QSqlTableModel *mItem;
    QComboBox *type;
    QDateTimeEdit *from;
    QDateTimeEdit *stop;
    QVBoxLayout *boxLayout;

    QVariantMap tmpMap;
    QTmpMap tmpTyp;
    QTmpMap tmpSet;
    QTmpMap tmpQuan;
    QTmpMap tmpOKNG;
    quint32 timeOut;
    QElapsedTimer t;
    QString path;
    QTmpMap tmpMsg;
};

#endif // SQLRECORD_H
