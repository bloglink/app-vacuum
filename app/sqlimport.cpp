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
    QString cmd = "insert into WIP_ID_PPARAM (ORG_ID,EVENT_ID,EVENT_NO,TRAY_ID,";
    cmd += "PPN,LINE_ID,WL_ID,ITEM_ID,PPARAM_ID,PPARAM_VALUE,JUDGE_RESULT,TEST_SDT,";
    cmd += "TEST_EDT,TEST_BY,CREATE_BY) ";
    cmd += "values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    QSqlQuery query(QSqlDatabase::database("upload"));
    QString uuid = QUuid::createUuid().toString();
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    QString code = msg.value(addr + TEMPCODE).toString();
    loop = (prev == code) ? loop + 1 : 0;
    prev = code;
    QString type = tmpSet.value(DataType).toString();
    query.prepare(cmd);
    query.addBindValue("100101");
    query.addBindValue(uuid);
    query.addBindValue(QString::number(loop + 1));
    query.addBindValue(code.toUpper());
    query.addBindValue(type);
    query.addBindValue("");  // 产线号
    query.addBindValue("");  // 工位号
    query.addBindValue("");  // 参数内容
    query.addBindValue("");  // 参数代码
    query.addBindValue("");  // 结果
    query.addBindValue("");  // 判定
    query.addBindValue(msg.value(addr + TEMPPLAY));  // 启动时间
    query.addBindValue(msg.value(addr + TEMPSTOP));  // 完成时间
    query.addBindValue("AIP");
    query.addBindValue("KONGSHI");
    query.exec();
}

void SqlImport::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
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

