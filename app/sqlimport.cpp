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
    com = NULL;
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
    query.addBindValue(tmpMap.value(addr + TEMPPLAY).toTime().toString("hh:mm:ss"));  // 启动时间
    query.addBindValue(tmpMap.value(addr + TEMPSTOP).toTime().toString("hh:mm:ss"));  // 完成时间
    query.addBindValue(tmpMap.value(addr + TEMPTYPE).toString());  // 型号
    query.addBindValue(tmpMap.value(addr + TEMPCODE).toString());  // 条码
    query.addBindValue(tmpMap.value(addr + TEMPUSER).toString());  // 用户
    query.addBindValue(tmpMap.value(addr + TEMPWORK).toString());  // 工位
    query.addBindValue(tmpMap.value(addr + TEMPTEMP).toString());  // 温度
    query.addBindValue(tmpMap.value(addr + TEMPISOK).toString());  // 结果
    if (!query.exec())
        qDebug() << query.lastError();
    QSqlDatabase::database("record").commit();

    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    if (!tmpSet.value(back + 0x20 + 0x0C).toString().isEmpty()) {  // 常州多维SPC上传
        saveSerial(tmpMap);
    }
    int conf = tmpSet.value(2000 + Qt::Key_3).toInt();
    int mode = tmpSet.value(conf + 0x00).toInt();
    if (mode == 3) {
        saveUpload(tmpMap);
    }
    if (mode == 4) {
        saveOracle(tmpMap);
    }
    tmpMap.clear();
}

void SqlImport::saveSerial(QTmpMap msg)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    if (com == NULL) {
        QString name = tmpSet.value(back + 0x20 + 0x0C).toString();
        com = new QSerialPort(name);
        if (com->open(QIODevice::ReadWrite)) {
            com->setBaudRate(9600);
            com->setRequestToSend(false);
            com->setDataTerminalReady(true);
            com->setDataBits(QSerialPort::Data8);
            com->setParity(QSerialPort::NoParity);
            com->setStopBits(QSerialPort::OneStop);
            com->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    QString str = "{";
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    str += msg.value(addr + TEMPCODE).toString();

    addr = tmpSet.value(3000 + Qt::Key_1).toInt();  // 电阻结果地址
    for (int i=0; i < 3; i++) {
        str += ",";
        if (!msg.value(addr + i*0x10 + 2).isNull()) {
            QString tmp = msg.value(addr + i*0x10 + 2).toString();
            QStringList xxx = tmp.split(" ", QString::SkipEmptyParts);
            tmp = xxx.last();
            tmp = tmp.remove(">").remove("m").remove("k").remove("Ω");
            tmp = tmp.remove(tr("折算前:")).remove(tr("折算后:"));
            str += tmp;
        }
    }

    addr = tmpSet.value(3000 + Qt::Key_3).toInt();  // 绝缘结果地址
    str += ",";
    if (!msg.value(addr + 3).isNull()) {
        QString curr = msg.value(addr+2).toString().remove(">").remove("M").remove("Ω");
        str += curr;
    }

    addr = tmpSet.value(3000 + Qt::Key_4).toInt();  // 交耐结果地址
    str += ",";
    if (!msg.value(addr + 3).isNull()) {
        QString curr = msg.value(addr+2).toString().remove(">").remove("mA");
        str += curr;
    }

    addr = tmpSet.value(3000 + Qt::Key_5).toInt();  // 直耐结果地址
    str += ",";
    if (!msg.value(addr + 3).isNull()) {
        QString curr = msg.value(addr+2).toString().remove(">").remove("mA");
        str += curr;
    }

    addr = tmpSet.value(3000 + Qt::Key_6).toInt();  // 匝间结果地址
    for (int i=0; i < 6; i++) {
        str += ",";
        if (!msg.value(addr + i*0x10 + 0x02).isNull()) {
            str += "," + msg.value(addr + i*0x10 + 0x08).toString();  // 电晕
            str += "," + msg.value(addr + i*0x10 + 0x0B).toString();  // 相位
            str += "," + msg.value(addr + i*0x10 + 0x02).toString();  // 面积
            str += "," + msg.value(addr + i*0x10 + 0x05).toString();  // 差积
        }
    }
    str += "}";
    com->write(str.toUtf8());
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
                    query.addBindValue("100101");
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

void SqlImport::saveOracle(QTmpMap msg)
{
    int addr = tmpSet.value(2000 + Qt::Key_4).toInt();  // 登录信息地址
    QString code = tmpSet.value(addr + 0).toString();  // 设备号
    QString numb = tmpSet.value(addr + 2).toString();  // 产线号
    QString work = tmpSet.value(addr + 3).toString();  // 工位号
    QString type = tmpSet.value(DataType).toString();  // 电机型号
    addr = tmpSet.value(DataUser).toInt();
    QString user = tmpSet.value(addr).toString();  // 当前用户
    addr = tmpSet.value(3000 + Qt::Key_0).toInt();
    QString isok = msg.value(addr + TEMPISOK).toString();
    QString cmd = "insert into hwms.HME_PERFORMANCE_MACHINE_TEST (";
    cmd += "CODE,PROD_LINE,WORK_LOC,TEST_TIME,";
    cmd += "MOTOR_TYPE,OPERATOR,TOTAL_RESULT,";
    cmd += "RESISTANCE_1,RESISTANCE_2,RESISTANCE_3,RESISTANCE_4,RESISTANCE_5,RESISTANCE_6,";
    cmd += "RES_RESULT,";
    cmd += "ISOL_VOLT,ISOL_RESIS,ISOL_RESULT,";
    cmd += "WT_VOLT,WT_CURR,PRES_ARC_DETEC,WT_VOLT_RESULT,";
    cmd += "ZJZS_VOLT,ZJZS_COR,ZJZS_PHASE,ZJZS_AREA,ZJZS_DIFFPROD,";
    cmd += "ZJFX_VOLT,ZJFX_COR,ZJFX_PHASE,ZJFX_AREA,ZJFX_DIFFPROD,";
    cmd += "ZJZF_VOLT,ZJZF_COR,ZJZF_PHASE,ZJZF_AREA,ZJZF_DIFFPROD,";
    cmd += "FXZJZX_VOLT,FXZJZX_COR,FXZJZX_PHASE,FXZJZX_AREA,XFZJZX_DIFFPROD,";
    cmd += "FXZJFX_VOLT,FXZJFX_COR,FXZJFX_PHASE,FXZJFX_AREA,FXZJFX_DIFFPROD,";
    cmd += "FXZJZF_VOLT,FXZJZF_COR,FXZJZF_PHASE,FXZJZF_AREA,FXZJZF_DIFFPROD,";
    cmd += "ZJ_RESULT,RATATION,ROTA_SETTING,ROTA_RESULT,";
    cmd += "CREATOR,CREATE_TIME,UPDATOR,UPDATE_TIME) ";
    cmd += "values(";
    cmd += "?,?,?,?,?,?,?,?,?,?,";
    cmd += "?,?,?,?,?,?,?,?,?,?,";
    cmd += "?,?,?,?,?,?,?,?,?,?,";
    cmd += "?,?,?,?,?,?,?,?,?,?,";
    cmd += "?,?,?,?,?,?,?,?,?,?,";
    cmd += "?,?,?,?,?,?,?,?,?)";

    QSqlQuery query(QSqlDatabase::database("upload"));
    query.prepare(cmd);
    query.addBindValue(code);
    query.addBindValue(numb);  // 产线号
    query.addBindValue(work);  // 工位号
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(type);
    query.addBindValue(user);
    query.addBindValue((isok == "NG") ? "NG" : "PASS");

    addr = tmpSet.value(3000 + Qt::Key_1).toInt();  // 电阻结果地址
    isok = "OK";
    for (int i=0; i < 6; i++) {
        if (!msg.value(addr + i*0x10 + 2).isNull()) {
            QString tmp = msg.value(addr + i*0x10 + 2).toString();
            QStringList xxx = tmp.split(" ", QString::SkipEmptyParts);
            tmp = xxx.last();
            tmp = tmp.remove(">").remove("m").remove("k").remove("Ω");
            tmp = tmp.remove(tr("折算前:")).remove(tr("折算后:"));
            query.addBindValue(tmp);
            isok = (msg.value(addr + i*0x10 + 3).toString() == "NG") ? "NG" : isok;
        } else {
            query.addBindValue("");
        }
    }
    if (!msg.value(addr+2).isNull())
        query.addBindValue((isok == "OK") ? "PASS" : "NG");  // 7+7=14
    else
        query.addBindValue("");

    addr = tmpSet.value(3000 + Qt::Key_3).toInt();  // 绝缘结果地址
    if (!msg.value(addr + 3).isNull()) {
        QStringList volts = msg.value(addr+1).toString().split(" ");
        QString volt = QString(volts.at(0)).remove("V");
        QString curr = msg.value(addr+2).toString().remove(">").remove("M").remove("Ω");
        query.addBindValue(volt.toInt());
        query.addBindValue(curr);
        query.addBindValue(msg.value(addr + 3).toString() == "NG" ? "NG" : "PASS");
    } else {
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
    }
    addr = tmpSet.value(3000 + Qt::Key_4).toInt();  // 交耐结果地址
    if (!msg.value(addr + 3).isNull()) {
        QStringList volts = msg.value(addr+1).toString().split(" ");
        QString volt = QString(volts.at(0)).remove("V");
        QString curr = msg.value(addr+2).toString().remove(">").remove("mA");
        query.addBindValue(volt.toInt());
        query.addBindValue(curr);
        query.addBindValue(msg.value(addr + 3).toString() == "ARC" ? "NG" : "PASS");
        query.addBindValue(msg.value(addr + 3).toString() == "NG" ? "NG" : "PASS");
    } else {
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
    }

    addr = tmpSet.value(3000 + Qt::Key_6).toInt();  // 匝间结果地址
    isok = "OK";
    for (int i=0; i < 6; i++) {
        if (!msg.value(addr + i*0x10 + 0x02).isNull()) {
            int conf = tmpSet.value(4000 + Qt::Key_6).toInt();  // 匝间配置地址
            int volt = tmpSet.value(conf + 0x40 + i).toInt();
            query.addBindValue(QString::number(volt));
            query.addBindValue(msg.value(addr + i*0x10 + 0x08).toString());
            query.addBindValue(msg.value(addr + i*0x10 + 0x0B).toString());
            query.addBindValue(msg.value(addr + i*0x10 + 0x02).toString());
            query.addBindValue(msg.value(addr + i*0x10 + 0x05).toString());
            for (int t=0; t < 4; t++) {
                QString tmp = msg.value(addr + i*0x10 + t*0x03 + 3).toString();
                isok = (tmp == "NG") ? "NG" : isok;
            }
        } else {
            query.addBindValue("");
            query.addBindValue("");
            query.addBindValue("");
            query.addBindValue("");
            query.addBindValue("");
        }
    }
    query.addBindValue((isok == "NG") ? "NG" : "PASS");

    addr = tmpSet.value(3000 + Qt::Key_2).toInt();  // 反嵌结果地址
    if (!msg.value(addr + 0x80 + 2).isNull()) {
        int conf = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
        int turn = tmpSet.value(conf + 0).toInt();
        QString tmp = msg.value(addr + 0x80 + 2).toString();
        tmp = (tmp == "NULL") ? "STATIC" : tmp;
        query.addBindValue(turn == 0 ? "CW" : "CCW");
        query.addBindValue(tmp);
        query.addBindValue(msg.value(addr + 0x80 + 3).toString() == "NG" ? "NG" : "PASS");
    } else {
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue("");
    }

    query.addBindValue(user);
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(user);
    query.addBindValue(QDateTime::currentDateTime());

    if (!query.exec()) {
        QVariantMap tmp;
        tmp.insert("enum", Qt::Key_Word);
        tmp.insert("text", query.lastError().text());
        emit sendAppMap(tmp);
        qDebug() << query.lastError();
    }
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

