/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理界面
*******************************************************************************/
#ifndef TYPCONFIG_H
#define TYPCONFIG_H

#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QListView>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QColorDialog>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QSqlDatabase>

#include "main.h"
#include "appbackup.h"
#include "apptester.h"
#include "boxqitems.h"
#include "boxqmodel.h"

#define NAME_SIZE 50

#define ADDRITEM 0x00
#define ADDRTYPE 0x01
#define ADDRWIRE 0x02
#define ADDRAUTO 0x03
#define ADDRDRIV 0x04

class TypConfig : public QWidget
{
    Q_OBJECT
public:
    explicit TypConfig(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initItemBar();
    void initConfigBar();
    void initButtonBar();
    void initSettings();
    void initModelBar();
    void initItemsBar();
    void initOtherBar();
    void saveSettings();
    void confSettings();
    void createModel();
    void selectModel();
    void removeModel();
    void autoPixmap(QString name);
    void selectColor();
    void clickMove();
    void clickSave();
    void clickButtons();
    void clickViewBar();
    void change();
    void recvAppMsg(QTmpMap msg);
    void recvShow();
    virtual void showEvent(QShowEvent *e);
private:
    QGridLayout *blayout;
    QVBoxLayout *hlayout;
    QTableWidget *view;
    QTableView *item;
    BoxQModel *mView;

    QLineEdit *page;
    QLabel *type;
    QLineEdit *name;

    QPushButton *btnSave;

    QComboBox *testTypeBox;
    QCheckBox *testKeepBox;
    QCheckBox *testAutoBox;
    QCheckBox *testDrivBox;
    QLabel *typePixmap;

    QList<QPushButton *> buttons;
    QList<QPushButton *> colors;

    QStringList itemNams;

    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpTyp;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isRemove;
    bool isInit;
};

#endif // TYPCONFIG_H
