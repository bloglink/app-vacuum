/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       动作设置
*******************************************************************************/
#include "appioctrl.h"

AppIoCtrl::AppIoCtrl(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppIoCtrl::initUI()
{
    QHBoxLayout *com_layout = new QHBoxLayout;
    QGridLayout *btn_layout = new QGridLayout;

    QStringList btnNames;
    btnNames << "Y00右工位" << "Y02右抽气" << "Y04右回气" << "Y01左工位"
             << "Y03左抽气" << "Y05左回气" << "Y06右罩上" << "Y08右罩下"
             << "Y10待添加" << "Y07左罩上" << "Y09左罩下" << "Y11待添加"
             << "Y12真空泵" << "Y14水平汽缸" << "Y20待添加" << "Y13垂直气缸"
             << "Y15待添加" << "Y21待添加" << "Y22待添加" << "Y24待添加"
             << "Y23待添加" << "Y25待添加" << "待添加" << "待添加";

    for (int i=0; i < 24; i++) {
        btns.append(new QToolButton(this));
        btns.at(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btns.at(i)->setCheckable(true);
        btns.at(i)->setText(btnNames.at(i));
        btns.at(i)->setStyleSheet("color:black;border-image:url(:/icon_circle_5.png)");
        btn_layout->addWidget(btns.at(i), i/8, i%8);
        connect(btns.at(i), SIGNAL(clicked(bool)), this, SLOT(readButton()));
    }

    QGridLayout *led_layout = new QGridLayout;
    QStringList names;
    names << "X00右启动" << "X02右停止" << "X04右光幕" << "X01左启动"
          << "X03左停止" << "X05左光幕" << "X06右气吸" << "X08右气放"
          << "X10右缸下" << "X07左气吸" << "X09左气放" << "X11左缸下"
          << "X12相序错" << "X13继电器" << "X14电机到位" << "X15垂直到位"
          << "X20垂直复位" << "X21水平到位" << "X22水平复位" << "X23待添加"
          << "待添加" << "待添加" << "待添加" << "待添加";

    for (int i=0; i < 24; i++) {
        leds.append(new QLabel(this));
        leds.at(i)->setText(names.at(i));
        leds.at(i)->setAlignment(Qt::AlignCenter);
        leds.at(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        leds.at(i)->setStyleSheet("color:black;border-image:url(:/icon_circle_5.png)");
        led_layout->addWidget(leds.at(i), i/8, i%8);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(com_layout);
    layout->addLayout(btn_layout);
    layout->addLayout(led_layout);
    this->setLayout(layout);
}

void AppIoCtrl::readButton()
{
    QToolButton *btn = qobject_cast<QToolButton*>(sender());
    if (btn->isChecked()) {
        btn->setStyleSheet("color:white;border-image:url(:/icon_circle_6.png)");
    } else {
        btn->setStyleSheet("color:black;border-image:url(:/icon_circle_5.png)");
    }
    quint32 hex = 0x00;
    for (int i=0; i < btns.size(); i++) {
        if (btns.at(i)->isChecked())
            hex += quint32(0x01 << i);
    }

    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6036 %1").arg(hex));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppIoCtrl::recvNewMsg(QTmpMap msg)
{
    int cmd = msg.value(Qt::Key_2).toInt();
    QString dat = msg.value(Qt::Key_1).toString();
    if (cmd == 6037) {
        quint32 hex = dat.toInt();
        for (int i=0; i < 24; i++) {
            if (hex & (0x00000001 << i))
                leds.at(i)->setStyleSheet("color:white;border-image:url(:/icon_circle_6.png)");
            else
                leds.at(i)->setStyleSheet("color:black;border-image:url(:/icon_circle_5.png)");
        }
    }
    if (cmd == 6059) {
        quint32 hex = dat.toInt();
        for (int i=0; i < 24; i++) {
            if (hex & (0x00000001 << i)) {
                btns.at(i)->setStyleSheet("color:white;border-image:url(:/icon_circle_6.png)");
                btns.at(i)->setChecked(true);
            } else {
                btns.at(i)->setStyleSheet("color:black;border-image:url(:/icon_circle_5.png)");
                btns.at(i)->setChecked(false);
            }
        }
    }
}

void AppIoCtrl::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_News:
        recvNewMsg(msg);
        break;
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void AppIoCtrl::showEvent(QShowEvent *e)
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", "6058 1");
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    e->accept();
}

void AppIoCtrl::hideEvent(QHideEvent *e)
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", "6058 0");
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    e->accept();
}

