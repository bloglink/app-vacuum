/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       后台管理
*******************************************************************************/
#include "appbackup.h"

AppBackup::AppBackup(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppBackup::initUI()
{
    initLayout();
    initDevText();
    initBoxText();
    initMacText();
    initBoxItem();
    initActText();
    initButtons();
}

void AppBackup::initLayout()
{
    QVBoxLayout *mlayout = new QVBoxLayout(this);
    QGroupBox *box = new QGroupBox(this);
    mlayout->addWidget(box);
    layout = new QHBoxLayout;
    box->setLayout(layout);

    blayout = new QHBoxLayout;
    mlayout->addLayout(blayout);
}

void AppBackup::initBoxText()
{
    QVBoxLayout *blayout = new QVBoxLayout;

    QGroupBox *boxGroup = new QGroupBox(this);
    boxGroup->setTitle(tr("后台配置"));
    boxGroup->setFixedWidth(240);
    boxGroup->setLayout(blayout);
    layout->addWidget(boxGroup);

    QStringList names;
    names << tr("产品型号") << tr("产品编号") << tr("评审编号") << tr("电感模式")
          << tr("语言支持") << tr("工位设置") << tr("最高电压") << tr("自动测试")
          << tr("测试延时") << tr("目标网络") << tr("匝间模式") << tr("测试模式")
          << tr("反嵌模式") << tr("波形比例") << tr("耐压模式") << tr("感应启动");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        blayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        txts.append(input);
    }
    blayout->addStretch();
}

void AppBackup::initBoxItem()
{
    QStringList names;
    names << "电阻" << "反嵌" << "绝缘" << "交耐" << "直耐"
          << "匝间" << "电参" << "电感" << "堵转" << "低启"
          << "霍尔" << "负载" << "空载" << "BEMF"  << "缺相"
          << "交耐2";

    mView = new BoxQModel(this);
    mView->setColumnCount(1);
    mView->setRowCount(names.size());
    for (int i=0; i < names.size(); i++) {
        mView->setData(mView->index(i, 0), "", Qt::DisplayRole);
        mView->item(i, 0)->setCheckable(true);
    }
    mView->setVerticalHeaderLabels(names);

    view = new QTableView(this);
    view->setModel(mView);
    view->setFixedHeight(560);
    view->verticalHeader()->setFixedWidth(48);
    view->horizontalHeader()->setVisible(false);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->setItemDelegate(new BoxQItems);

    QVBoxLayout *blayout = new QVBoxLayout;
    blayout->addWidget(view);
    blayout->addStretch();
    QGroupBox *box = new QGroupBox(this);
    box->setTitle(tr("可用项目"));
    box->setFixedWidth(126);
    box->setLayout(blayout);
    layout->addWidget(box);
}

void AppBackup::initDevText()
{
    QStringList com;
#ifndef __linux__
    QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
    QSettings *settings = new QSettings(path, QSettings::NativeFormat);
    QStringList key = settings->allKeys();
    HKEY hKey;
    int ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
                             0, KEY_READ, &hKey);
    if (ret != 0) {
        qDebug() << "Cannot open regedit!";
    } else {
        for (int i=0; i < key.size(); i++) {
            wchar_t name[256];
            DWORD ikey = sizeof(name);
            char kvalue[256];
            DWORD t = sizeof(kvalue);
            DWORD type;
            QString tmpName;
            ret = ::RegEnumValue(hKey, i, LPWSTR(name), &ikey, 0, &type,
                                 reinterpret_cast<BYTE*>(kvalue), &t);
            if (ret == 0) {
                for (int j = 0; j < static_cast<int>(t); j++) {
                    if (kvalue[j] != 0x00) {
                        tmpName.append(kvalue[j]);
                    }
                }
                com << tmpName;
            }
        }
        RegCloseKey(hKey);
    }
#else
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        com << info.portName();
    }
#endif
    com.sort();
    for (int i=0; i < com.size(); i++) {
        if (com.at(i).size() > 4) {
            com.move(i, com.size()-1);
        }
    }
    com.insert(0, "NULL");

    QVBoxLayout *dlayout = new QVBoxLayout;
    QGroupBox *devGroup = new QGroupBox(this);
    devGroup->setTitle(tr("串口设备"));
    devGroup->setFixedWidth(240);
    devGroup->setLayout(dlayout);
    layout->addWidget(devGroup);

    QStringList names;
    names << tr("一代气缸左") << tr("一代气缸右") << tr("横河电参左") << tr("横河电参右");
    names << tr("松下伺服左") << tr("松下伺服右") << tr("松下PLC左") << tr("松下PLC右");
    names << tr("蓝仪电源左") << tr("安川变频器") << tr("温度传感器");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *ll = new QHBoxLayout;
        ll->addWidget(new QLabel(names.at(i), this));
        QComboBox *box = new QComboBox(this);
        box->setFixedSize(125, 35);
        box->setView(new QListView);
        box->addItems(com);
        ll->addWidget(box);
        dlayout->addLayout(ll);
        cmbs.append(box);
    }
    dlayout->addStretch();
}

void AppBackup::initActText()
{
    QVBoxLayout *lay = new QVBoxLayout;
    QGroupBox *actGroup = new QGroupBox(this);
    actGroup->setTitle(tr("动作设置"));
    actGroup->setFixedWidth(200);
    actGroup->setLayout(lay);
    layout->addWidget(actGroup);

    QStringList names;
    names << tr("下压动作左") << tr("下压到位左") << tr("下压归位左")
          << tr("下压动作右") << tr("下压到位右") << tr("下压归位右")
          << tr("上升动作左") << tr("上升到位左") << tr("上升归位左")
          << tr("上升动作右") << tr("上升到位右") << tr("上升归位右")
          << tr("夹紧动作左") << tr("夹紧到位左") << tr("夹紧归位左")
          << tr("夹紧动作右") << tr("夹紧到位右") << tr("夹紧归位右")
          << tr("弹开动作左") << tr("弹开动作右") << tr("启动信号左")
          << tr("启动信号右") << tr("停止信号左") << tr("停止信号右");

    actView = new QTableWidget(this);
    actView->setColumnCount(1);
    actView->setRowCount(names.size());
    actView->setVerticalHeaderLabels(names);
    actView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    actView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    actView->verticalHeader()->setFixedWidth(90);
    actView->horizontalHeader()->setVisible(false);
    for (int i=0; i < names.size(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        actView->setItem(i, 0, item);
    }
    lay->addWidget(actView);
}

void AppBackup::initMacText()
{
    offView = new QTableWidget(this);
    QStringList headers;
    headers << "K" << "b";
    QStringList items;
    items << tr("电流") << tr("电压") << tr("功率") << tr("Icc") << tr("转速")
          << tr("Isp") << tr("转向") << tr("电压") << tr("常数") << tr("相位")
          << tr("高电平") << tr("低电平") << tr("占空比") << tr("频率");
    offView->setRowCount(items.size());
    offView->setColumnCount(headers.size());
    offView->setHorizontalHeaderLabels(headers);
    offView->setVerticalHeaderLabels(items);
    offView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    offView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    offView->verticalHeader()->setFixedWidth(60);
    for (int i=0; i < items.size()*headers.size(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        offView->setItem(i/2, i%2, item);
    }
    offView->setFixedHeight(480);

    QVBoxLayout *blayout = new QVBoxLayout;
    QGroupBox *box = new QGroupBox(this);
    box->setTitle(tr("本机信息"));
    box->setLayout(blayout);
    layout->addWidget(box);

    blayout->addWidget(offView);
    blayout->addStretch();
}

void AppBackup::initButtons()
{
    QPushButton *btnReset = new QPushButton(this);
    btnReset->setText(tr("恢复出厂"));
    btnReset->setFixedSize(97, 44);
    blayout->addWidget(btnReset);
    connect(btnReset, SIGNAL(clicked(bool)), this, SLOT(recovery()));
    blayout->addStretch();

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    blayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppBackup::initSettings()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    for (int i=0; i < txts.size(); i++) {
        txts.at(i)->setText(tmpSet.value(back + i).toString());
    }
    for (int i=0; i < mView->rowCount(); i++) {
        int c = tmpSet.value(back + 0x10 + i).toInt();
        mView->setData(mView->index(i, 0), c, Qt::CheckStateRole);
    }
    for (int i=0; i < cmbs.size(); i++) {
        QString p = tmpSet.value(back + 0x20 + i).toString();
        if (!p.isEmpty())
            cmbs.at(i)->setCurrentText(p);
    }
    for (int i=0; i < actView->rowCount(); i++) {
        QString p = tmpSet.value(back + 0x40 + i).toString();
        if (!p.isEmpty())
            actView->item(i, 0)->setText(p);
    }
    for (int i=0; i < offView->rowCount()*2; i++) {
        QString p = tmpSet.value(back + 0x80 + i).toString();
        if (p.isEmpty() && i % 2 == 0)
            p = "1";
        if (p.isEmpty() && i % 2 == 1)
            p = "0";
        offView->item(i/2, i%2)->setText(p);
    }
}

void AppBackup::saveSettings()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    for (int i=0; i < txts.size(); i++) {
        tmpMsg.insert(back + 0x00 + i, txts.at(i)->text());
    }
    for (int i=0; i < mView->rowCount(); i++) {
        tmpMsg.insert(back + 0x10 + i, mView->data(mView->index(i, 0), Qt::CheckStateRole).toInt());
    }
    for (int i=0; i < cmbs.size(); i++) {
        QString str = cmbs.at(i)->currentText();
        if (str == "NULL")
            str.clear();
        tmpMsg.insert(back + 0x20 + i, str);
    }
    for (int i=0; i < actView->rowCount(); i++) {
        tmpMsg.insert(back + 0x40 + i, actView->item(i, 0)->text());
    }
    for (int i=0; i < offView->rowCount()*2; i++) {
        QString str = offView->item(i/2, i%2)->text();
        tmpMsg.insert(back + 0x80 + i, str);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_backup");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppBackup::recovery()
{
    QSqlDatabase::database("system").close();
    QSqlDatabase::database("config").close();
    QSqlDatabase::database("record").close();
    QDir dir;
    dir.setPath("nandflash");
    dir.remove("system.db");
    dir.remove("config.db");
    dir.remove("record.db");
    QApplication::closeAllWindows();
}

void AppBackup::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void AppBackup::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

