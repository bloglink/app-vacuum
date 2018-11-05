/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       日常维护
*******************************************************************************/
#include "apprepair.h"

AppRepair::AppRepair(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppRepair::initUI()
{
    initLayout();
    initTimeBar();
    initButtonBar();
}

void AppRepair::initLayout()
{
    layout = new QVBoxLayout;
    layout->addStretch();

    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addStretch();
    mlayout->addLayout(layout);
    mlayout->addStretch();
}

void AppRepair::initTimeBar()
{
    warn = new QLabel(this);
    QString strLR = "<p style='font:48pt;color:#FF0000;'><b>%1</b></p>";  // 大号红,NG,中断
    warn->setText(strLR.arg(tr("请预热设备")));
    layout->addWidget(warn);

    timeLess = new QLCDNumber(this);
    timeLess->setFixedSize(500, 200);
    layout->addWidget(timeLess);
    time.setHMS(0, 5, 0);
    timeLess->display(time.toString("mm:ss"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
}

void AppRepair::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);
    layout->addStretch();

    boxAuto = new QCheckBox(tr("自动进入"), this);
    boxAuto->setFixedSize(97, 44);
    blayout->addWidget(boxAuto);
    connect(boxAuto, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    boxSpeed = new QSpinBox(this);
    boxSpeed->setFixedSize(97, 44);
    boxSpeed->setMinimum(500);
    boxSpeed->setMaximum(2000);
    boxSpeed->setValue(1500);
    blayout->addWidget(boxSpeed);

    blayout->addStretch();

    QPushButton *btnTest = new QPushButton(tr("设备预热"), this);
    btnTest->setFixedSize(97, 44);
    blayout->addWidget(btnTest);
    connect(btnTest, SIGNAL(clicked(bool)), this, SLOT(testRepair()));

    QPushButton *btnStop = new QPushButton(tr("停止预热"), this);
    btnStop->setFixedSize(97, 44);
    blayout->addWidget(btnStop);
    connect(btnStop, SIGNAL(clicked(bool)), this, SLOT(stopRepair()));
}

void AppRepair::initSettings()
{
    int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
    int care = tmpSet.value(conf + 0x00).toInt();
    boxAuto->setChecked((care == 1) ? true : false);
}

void AppRepair::saveSettings()
{
    int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
    tmpMsg.insert(conf, boxAuto->isChecked() ? 1 : 0);
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppRepair::testRepair()
{
    int m = time.minute();
    int s = time.second();
    if (m == 0 && s == 0)
        return;
    timer->start(1000);
    tmpMap.insert("enum", Qt::Key_Shop);
    tmpMap.insert("mode", 0);  // 转速模式
    tmpMap.insert("turn", 0);
    tmpMap.insert("data", boxSpeed->value());
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    QString strLR = "<p style='font:48pt;color:#FF0000;'><b>%1</b></p>";  // 大号红,NG,中断
    warn->setText(strLR.arg(tr("预热中,注意安全")));
}

void AppRepair::stopRepair()
{
    timer->stop();
    tmpMap.insert("enum", Qt::Key_Shop);
    tmpMap.insert("mode", 0);  // 转速模式
    tmpMap.insert("turn", 0);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppRepair::updateTime()
{
    int m = time.minute();
    int s = time.second();
    m = (s == 0) ? (m - 1) : m;
    s = (s != 0) ? (s - 1) : 59;
    time.setHMS(0, m, s);
    timeLess->display(time.toString("mm:ss"));
    if (m == 0 && s == 0) {
        stopRepair();
        QString strLR = "<p style='font:48pt;color:#FF0000;'><b>%1</b></p>";  // 大号红,NG,中断
        warn->setText(strLR.arg(tr("预热完成")));
    }
}

void AppRepair::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void AppRepair::showEvent(QShowEvent *e)
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", "6058 1");
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    this->setFocus();
    initSettings();
    e->accept();
}

void AppRepair::hideEvent(QHideEvent *e)
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", "6058 0");
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    e->accept();
}
