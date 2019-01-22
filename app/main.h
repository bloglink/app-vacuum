/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       宏定义
*******************************************************************************/
#ifndef APPDEFINE
#define APPDEFINE

#include <QMap>
#include <QString>
#include <QVariant>
#include <QMetaType>

typedef QMap<int, QVariant> QTmpMap;

#define WORKL 0x13
#define WORKR 0x14

const int DataFree = 0x00;
const int DataTest = 0x01;

#define DATAOK  0x00
#define DATANG  0x01
#define DATAON  0x02
#define DATADC  0x03
#define DATADD  0x04
#define DERROR  0xFF

const int DataFile = 22000;  // 当前型号
const int DataUser = 22001;  // 当前用户
const int DataAuto = 22002;  // 自动保存
const int DataSave = 22003;  // 日志输出
const int DataType = 22004;  // 当前型号
const int DataSoft = 22015;  // 软件版本

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
#define Y01 0x000008  // 右抽气
#define Y02 0x000002  // 右回气
#define Y03 0x000010  // 左启动
#define Y04 0x000004  // 左抽气
#define Y05 0x000020  // 左回气
#define Y06 0x000040  // 右罩上
#define Y07 0x000200  // 右罩下

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

const int mName = 0x00;
const int mPass = 0x01;
const int mRole = 0x02;
const int mLast = 0x03;
const int mSave = 0x04;

#endif // APPDEFINE

