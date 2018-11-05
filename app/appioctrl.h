/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       动作设置
*******************************************************************************/
#ifndef APPIOCTRL_H
#define APPIOCTRL_H

#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>
#include <QHideEvent>

#include "main.h"

#define YY0E 0x004000  // 总合格
#define YY07 0x000002  // 转向不合格
#define YY08 0x008000  // 测试中
#define YY09 0x000004  // RFID未读到
#define YY10 0x000001  // 总不合格
#define YY11 0x000008  // 空载不合格
#define YY12 0x000010  // PG不合格
#define YY13 0x000080  // 负载不合格
#define YY14 0x000020  // 移除连轴器
#define YY15 0x010000  // 故障输出

#define XX20 0x010000  // 启动
#define XX21 0x020000  // 轴对接完成
#define XX22 0x040000  // 停止
#define XX23 0x080000  // 轴移除完成

#define Y00 0x000001  // 右启动
#define Y01 0x000002  // 右抽气
#define Y02 0x000004  // 右回气
#define Y03 0x000008  // 左启动
#define Y04 0x000010  // 左抽气
#define Y05 0x000020  // 左回气
#define Y06 0x000040  // 右罩上
#define Y07 0x000080  // 右罩下

#define X00 0x000001  // 右启动
#define X01 0x000002
#define X02 0x000004  // 右光幕
#define X03 0x000008  // 左启动
#define X04 0x000010
#define X05 0x000020  // 左光幕
#define X06 0x000040
#define X07 0x000080
#define X08 0x000100  // 右到位
#define X11 0x000800  // 左到位
#define X14 0x004000

class AppIoCtrl : public QWidget
{
    Q_OBJECT
public:
    explicit AppIoCtrl(QWidget *parent = 0);

signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
public slots:
    void initUI();
    void readButton();
    void recvNewMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QList<QToolButton*> btns;
    QList<QLabel*> leds;
    QComboBox *udps;
    QTimer *timer;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
};

#endif // APPIOCTRL_H
