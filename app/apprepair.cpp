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
    initParmBar();
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
    time.setHMS(0, 0, 15);
    timeLess->display(time.toString("mm:ss"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
}

void AppRepair::initParmBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);

    boxSpeed = new QSpinBox(this);
    boxSpeed->setFixedHeight(44);
    boxSpeed->setMinimum(200);
    boxSpeed->setMaximum(2500);
    boxSpeed->setValue(1500);
    blayout->addWidget(new QLabel(tr("转速"), this));
    blayout->addWidget(boxSpeed);

    disperse = new QSpinBox(this);
    disperse->setFixedHeight(44);
    disperse->setMinimum(20);
    disperse->setMaximum(500);
    disperse->setValue(50);
    blayout->addWidget(new QLabel(tr("离散"), this));
    blayout->addWidget(disperse);

    blayout->addStretch();

    frm = new QFrame(this);
    blayout->addWidget(frm);
    frm->hide();

    QHBoxLayout *flayout = new QHBoxLayout;
    frm->setLayout(flayout);

    boxTorque = new QDoubleSpinBox(this);
    boxTorque->setFixedHeight(44);
    boxTorque->setDecimals(4);
    flayout->addWidget(new QLabel(tr("转矩"), this));
    flayout->addWidget(boxTorque);

    boxRatio = new QDoubleSpinBox(this);
    boxRatio->setFixedHeight(44);
    boxRatio->setDecimals(4);
    boxRatio->setMinimum(0.2);
    flayout->addWidget(new QLabel(tr("系数"), this));
    flayout->addWidget(boxRatio);
}

void AppRepair::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);
    layout->addStretch();

    boxAuto = new QCheckBox(tr("自动进入"), this);
    boxAuto->setFixedHeight(44);
    blayout->addWidget(boxAuto);
    connect(boxAuto, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    blayout->addStretch();

    QPushButton *btnTest = new QPushButton(tr("设备预热"), this);
    btnTest->setFixedSize(80, 44);
    blayout->addWidget(btnTest);
    connect(btnTest, SIGNAL(clicked(bool)), this, SLOT(testRepair()));

    QPushButton *btnStop = new QPushButton(tr("停止预热"), this);
    btnStop->setFixedSize(80, 44);
    blayout->addWidget(btnStop);
    connect(btnStop, SIGNAL(clicked(bool)), this, SLOT(stopRepair()));
}

void AppRepair::initSettings()
{
    count = 0;
    int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
    int care = tmpSet.value(conf + 0x00).toInt();
    int speed = tmpSet.value(conf + 0x01).toInt();
    double torque = tmpSet.value(conf + 0x02).toDouble();
    boxAuto->setChecked((care == 1) ? true : false);
    boxSpeed->setValue(speed);
    boxTorque->setValue(torque);
    disperse->setValue(tmpSet.value(conf + 0x03).toInt());
    boxRatio->setValue(tmpSet.value(conf + 0x04).toDouble());
}

void AppRepair::saveSettings()
{
    int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
    tmpMsg.insert(conf + 0x00, boxAuto->isChecked() ? 1 : 0);
    tmpMsg.insert(conf + 0x01, boxSpeed->value());
    tmpMsg.insert(conf + 0x02, boxTorque->value());
    tmpMsg.insert(conf + 0x03, disperse->value());
    tmpMsg.insert(conf + 0x04, boxRatio->value());
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppRepair::testRepair()
{
    saveSettings();
    int m = time.minute();
    int s = time.second();
    if (m == 0 && s == 0)
        return;
    count = 0;
    taskvolt = 0;
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

    if (disperse->value() == 21)
        frm->show();
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
        boxTorque->setValue(taskvolt);
        time.setHMS(0, 0, 15);
        timeLess->display(time.toString("mm:ss"));
        saveSettings();
    }
}

void AppRepair::recvLedMsg(QTmpMap msg)
{
    if (msg.value(Qt::Key_2).isNull())
        return;
    if (this->isHidden())
        return;
    int c = msg.value(Qt::Key_2).toInt();
    if (c == DATAON) {
        count++;
        QString strLR = "<p style='font:48pt;color:#FF0000;'><b>%1</b></p>";  // 大号红,NG,中断
        warn->setText(strLR.arg(tr("第") + QString::number(count) + tr("次测试")));
    }
}

void AppRepair::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Call:
        recvLedMsg(msg);
        break;
    case Qt::Key_Away:
        taskvolt = msg.value(Qt::Key_2).toDouble();
        updateTime();
        break;
    default:
        break;
    }
}

void AppRepair::showEvent(QShowEvent *e)
{
    frm->hide();
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
