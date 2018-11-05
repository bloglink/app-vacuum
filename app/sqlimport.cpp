/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导入
*******************************************************************************/
#include "sqlimport.h"

SqlImport::SqlImport(QObject *parent) : QObject(parent)
{
    loop = 0;
}

void SqlImport::initUpload()
{
    int hall = tmpSet.value(3000 + Qt::Key_B).toInt();  // 霍尔结果地址
    for (int item=0; item < 4; item++) {  // 高电平,低电平,占空比,频率
        keys.append(hall + item*0x10);
    }
    int load = tmpSet.value(3000 + Qt::Key_C).toInt();  // 负载结果地址
    for (int item=0; item < 5; item++) {  // 电流,功率,Icc,rpm,ccw
        keys.append(load + item*0x10);
    }
    int bemf = tmpSet.value(3000 + Qt::Key_E).toInt();  // 反电动势结果地址
    for (int item=0; item < 5; item++) {  // 电压,常数,相位,相序,不平衡度
        keys.append(bemf + item*0x10);
    }
}

void SqlImport::saveRecord(QTmpMap msg)
{
    tmpMap = msg;
    QSqlDatabase::database("record").transaction();
    QSqlQuery query(QSqlDatabase::database("record"));
    SqlSnowId snow;
    quint64 uuid = snow.getId();
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    tmpMap.remove(Qt::Key_0);
    tmpMap.remove(Qt::Key_1);
    tmpMap.insert(0xFFFF, 0xFFFF);
    for (int i=0; i < tmpMap.keys().size(); i++) {
        int numb = tmpMap.keys().at(i);
        query.prepare("insert into aip_record values(?,?,?,?)");
        query.addBindValue(uuid);
        query.addBindValue(numb);
        query.addBindValue(tmpMap.value(addr + TEMPTYPE).toString());
        query.addBindValue(tmpMap.value(numb));
        if (!query.exec())
            qDebug() << query.lastError();
    }
    query.prepare("insert into aip_sqlite values(?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(uuid);
    query.addBindValue(tmpMap.value(addr + TEMPDATE).toString());  // 日期
    query.addBindValue(tmpMap.value(addr + TEMPPLAY));  // 启动时间
    query.addBindValue(tmpMap.value(addr + TEMPSTOP));  // 完成时间
    query.addBindValue(tmpMap.value(addr + TEMPTYPE).toString());  // 型号
    query.addBindValue(tmpMap.value(addr + TEMPCODE).toString());  // 条码
    query.addBindValue(tmpMap.value(addr + TEMPUSER).toString());  // 用户
    query.addBindValue(tmpMap.value(addr + TEMPWORK).toString());  // 工位
    query.addBindValue(tmpMap.value(addr + TEMPTEMP).toString());  // 温度
    query.addBindValue(tmpMap.value(addr + TEMPISOK).toString());  // 结果
    if (!query.exec())
        qDebug() << query.lastError();
    QSqlDatabase::database("record").commit();

    int conf = tmpSet.value(2000 + Qt::Key_3).toInt();
    int mode = tmpSet.value(conf + 0x00).toInt();
    if (mode >= 3) {
        saveUpload(tmpMap);
    }
    tmpMap.clear();
}

void SqlImport::saveUpload(QTmpMap msg)
{
    int conf = tmpSet.value(2000 + Qt::Key_4).toInt();  // 登录信息地址
    QString numb = tmpSet.value(conf + 2).toString();  // 产线号
    QString work = tmpSet.value(conf + 3).toString();  // 工位号
    QString cmd = "insert into WIP_ID_PPARAM (ORG_ID,EVENT_ID,EVENT_NO,TRAY_ID,";
    cmd += "PPN,LINE_ID,WL_ID,ITEM_ID,PPARAM_ID,PPARAM_VALUE,JUDGE_RESULT,TEST_SDT,";
    cmd += "TEST_EDT,TEST_BY,CREATE_BY) ";
    cmd += "values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    QSqlQuery query(QSqlDatabase::database("upload"));
    QString uuid = QUuid::createUuid().toString();
    uuid.remove("{");
    uuid.remove("}");
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    QString code = msg.value(addr + TEMPCODE).toString();
    loop = (prev == code) ? loop + 1 : 0;
    prev = code;
    QString type = tmpSet.value(DataType).toString();

    QDateTime play;
    play.setDate(QDate::currentDate());
    play.setTime(msg.value(addr + TEMPPLAY).toTime());
    QDateTime stop = QDateTime::currentDateTime();
    QList<int> ckey = msg.keys();
    for (int i=0; i < ckey.size(); i++) {
        int key = ckey.at(i);
        if (keys.contains(key)) {
            for (int quan=0; quan < 5; quan++) {
                if (ckey.contains(key + quan*3 + 1)) {
                    QString str = msg.value(key + quan*3 + 2).toString();
                    query.prepare(cmd);
                    query.addBindValue("10101");
                    query.addBindValue(uuid);
                    query.addBindValue(QString::number(loop + 1));
                    query.addBindValue(code.toUpper());
                    query.addBindValue(type);
                    query.addBindValue(numb);  // 产线号
                    query.addBindValue(work);  // 工位号
                    query.addBindValue(msg.value(key).toString());  // 参数内容
                    query.addBindValue(key + quan*3);  // 参数代码
                    query.addBindValue(msg.value(key + quan*3 + 1).toString());  // 结果
                    query.addBindValue((str == "OK") ? "0" : "1");  // 判定
                    query.addBindValue(play.toString("yyyy/MM/dd hh:mm:ss"));
                    query.addBindValue(stop.toString("yyyy/MM/dd hh:mm:ss"));
                    query.addBindValue("AIP");
                    query.addBindValue("KONGSHI");
                    if (!query.exec())
                        qDebug() << query.lastError();
                }
            }
        }
    }
    query.clear();
}

void SqlImport::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        initUpload();
        break;
    case Qt::Key_Save:
        if (msg.value(Qt::Key_1).toString() == "aip_record") {
            saveRecord(msg);
        }
        break;
    default:
        break;
    }
}

