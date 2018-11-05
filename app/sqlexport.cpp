/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导出
*******************************************************************************/
#include "sqlexport.h"

SqlExport::SqlExport(QObject *parent) : QObject(parent)
{
}

int SqlExport::exportFile(QTmpMap msg)
{
    QElapsedTimer t;
    t.start();
    if (createFile(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (selectNumb(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (selectItem(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (exportHead(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    if (exportData(msg) == Qt::Key_Stop)
        return Qt::Key_Stop;
    file->close();
    showText(tr("导出成功,实际用时%1ms").arg(t.elapsed()));
    return 0;
}

int SqlExport::createFile(QTmpMap msg)
{
    QString path = msg.value(Qt::Key_1).toString();
    file = new QFile(QString("%1").arg(path));
    if (!file->open(QFile::WriteOnly)) {
        showText(tr("文件打开失败"));
        return Qt::Key_Stop;
    }
    return Qt::Key_Meta;
}

int SqlExport::selectNumb(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_4).toString();
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();
    QString type = msg.value(Qt::Key_B).toString();

    QSqlQuery query(QSqlDatabase::database(name));
    QString cmd = tr("select count(*) from aip_record where R_ITEM=65535");
    cmd += tr(" and R_UUID > %1 and R_UUID < %2").arg(from).arg(stop);
    if (!type.isEmpty()) {
        cmd += tr(" and R_TYPE = '%1'").arg(type);
    }

    if (!query.exec(cmd)) {
        showText(tr("查询数据失败,%1").arg(query.lastError().text()));
        return Qt::Key_Stop;
    }
    if (!query.next()) {
        showText(tr("查询数据失败,%1").arg(query.lastError().text()));
        return Qt::Key_Stop;
    }
    quan = query.value(0).toInt();
    showText(tr("共找到%1条数据").arg(quan));
    query.clear();

    return (quan == 0) ? Qt::Key_Stop : Qt::Key_Meta;
}

int SqlExport::selectItem(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_4).toString();
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();
    QString type = msg.value(Qt::Key_B).toString();

    QSqlQuery query(QSqlDatabase::database(name));
    QString cmd = tr("select distinct R_ITEM from aip_record where R_ITEM < %1").arg(0xFFFF);
    cmd += tr(" and R_UUID > %1 and R_UUID < %2").arg(from).arg(stop);
    if (!type.isEmpty()) {
        cmd += tr(" and R_TYPE = '%1'").arg(type);
    }
    if (!query.exec(cmd)) {
        showText(tr("查询数据失败,%1").arg(query.lastError().text()));
        return Qt::Key_Stop;
    }
    numbs.clear();
    while (query.next()) {
        numbs.append(query.value(0).toInt());
    }
    return Qt::Key_Meta;
}

int SqlExport::exportHead(QTmpMap msg)
{
    qDebug() << msg.value(Qt::Key_2).toString();
    QStringList title;

    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    tmpMsg.insert(addr + TEMPDATE, tr("日期"));
    tmpMsg.insert(addr + TEMPPLAY, tr("启动"));
    tmpMsg.insert(addr + TEMPSTOP, tr("完成"));
    tmpMsg.insert(addr + TEMPTYPE, tr("型号"));
    tmpMsg.insert(addr + TEMPCODE, tr("条码"));
    tmpMsg.insert(addr + TEMPUSER, tr("用户"));
    tmpMsg.insert(addr + TEMPWORK, tr("工位"));
    tmpMsg.insert(addr + TEMPTEMP, tr("温度"));
    tmpMsg.insert(addr + TEMPISOK, tr("判定"));

    QStringList names;
    names << tr("电阻") << tr("反嵌") << tr("绝缘") << tr("交耐") << tr("直耐")
          << tr("匝间") << tr("电参") << tr("电感") << tr("堵转") << tr("低启")
          << tr("霍尔") << tr("负载") << tr("空载") << tr("BEMF") << tr("缺相");
    QList<int> index;
    index << Qt::Key_1 << Qt::Key_2 << Qt::Key_3 << Qt::Key_4 << Qt::Key_5
          << Qt::Key_6 << Qt::Key_7 << Qt::Key_8 << Qt::Key_9 << Qt::Key_A
          << Qt::Key_B << Qt::Key_C << Qt::Key_D << Qt::Key_E;
    for (int i=0; i < names.size(); i++) {
        int addr = tmpSet.value(3000 + index.at(i)).toInt();
        if (names.at(i) == tr("电阻")) {
            int addr = tmpSet.value(3000 + Qt::Key_1).toInt();
            tmpMsg.insert(addr + 0x80 + 0x02, tr("电阻平衡结果"));
            tmpMsg.insert(addr + 0x80 + 0x03, tr("电阻平衡判定"));
        }
        if (names.at(i) == tr("反嵌")) {
            tmpMsg.insert(addr + 0x80 + 0x02, tr("转向结果"));
            tmpMsg.insert(addr + 0x80 + 0x03, tr("转向判定"));
        }
        if (names.at(i) == tr("匝间")) {
            for (int numb=0; numb < 0x08; numb++) {
                tmpMsg.insert(addr + numb*0x10 + 0x00, names.at(i) + tr("项目%1").arg(numb + 1));
                tmpMsg.insert(addr + numb*0x10 + 0x01, tr("面积上限%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x02, tr("面积结果%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x03, tr("面积判定%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x04, tr("差积上限%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x05, tr("差积结果%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x06, tr("差积判定%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x07, tr("电晕上限%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x08, tr("电晕结果%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x09, tr("电晕判定%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x0A, tr("相位上限%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x0B, tr("相位结果%1").arg(numb+1));
                tmpMsg.insert(addr + numb*0x10 + 0x0C, tr("相位判定%1").arg(numb+1));
            }
            continue;
        }
        if (names.at(i) == tr("电感")) {
            int addr = tmpSet.value(3000 + Qt::Key_8).toInt();
            tmpMsg.insert(addr + 0x80 + 0x02, tr("电感平衡结果"));
            tmpMsg.insert(addr + 0x80 + 0x03, tr("电感平衡判定"));
        }
        if (names.at(i) == tr("霍尔")) {
            QStringList names;
            names << tr("霍尔高电平") << tr("霍尔低电平") << tr("霍尔占空比") << tr("霍尔频率");
            for (int item=0; item < names.size(); item++) {
                tmpMsg.insert(addr + 0x10*item, names.at(item));
                for (int n=0; n < 3; n++) {
                    tmpMsg.insert(addr + 0x10*item + n*3 + 0x01, tr("结果%1").arg(n+1));
                    tmpMsg.insert(addr + 0x10*item + n*3 + 0x02, tr("判定%1").arg(n+1));
                }
            }
            continue;
        }
        if (names.at(i) == tr("负载")) {
            tmpMsg.insert(addr + 0x00, tr("负载电流"));
            tmpMsg.insert(addr + 0x10, tr("负载功率"));
            tmpMsg.insert(addr + 0x20, tr("负载Icc"));
            tmpMsg.insert(addr + 0x30, tr("负载转速"));
            tmpMsg.insert(addr + 0x40, tr("负载转向"));
            continue;
        }
        if (names.at(i) == tr("空载")) {
            tmpMsg.insert(addr + 0x00, tr("空载电流"));
            tmpMsg.insert(addr + 0x10, tr("空载功率"));
            tmpMsg.insert(addr + 0x20, tr("空载Icc"));
            tmpMsg.insert(addr + 0x30, tr("空载转速"));
            tmpMsg.insert(addr + 0x40, tr("空载转向"));
            continue;
        }
        if (names.at(i) == tr("缺相")) {
            tmpMsg.insert(addr + 0x00, tr("缺相电流"));
            tmpMsg.insert(addr + 0x10, tr("缺相功率"));
            tmpMsg.insert(addr + 0x20, tr("缺相Icc"));
            tmpMsg.insert(addr + 0x30, tr("缺相转速"));
            tmpMsg.insert(addr + 0x40, tr("缺相转向"));
            continue;
        }
        if (names.at(i) == tr("BEMF")) {
            QStringList names;
            names << tr("BEMF电压") << tr("BEMF常数") << tr("BEMF相位");
            for (int item=0; item < names.size(); item++) {
                tmpMsg.insert(addr + 0x10*item, names.at(item));
                for (int n=0; n < 3; n++) {
                    tmpMsg.insert(addr + 0x10*item + n*3 + 0x01, tr("结果%1").arg(n+1));
                    tmpMsg.insert(addr + 0x10*item + n*3 + 0x02, tr("判定%1").arg(n+1));
                }
            }
            tmpMsg.insert(addr + 0x30, tr("BEMF不平衡度"));
            tmpMsg.insert(addr + 0x40, tr("BEMF相序"));
            continue;
        }
        for (int numb=0; numb < 0x08; numb++) {
            tmpMsg.insert(addr + numb*0x10 + 0x00, names.at(i) + tr("项目%1").arg(numb + 1));
            tmpMsg.insert(addr + numb*0x10 + 0x01, names.at(i) + tr("参数%1").arg(numb + 1));
            tmpMsg.insert(addr + numb*0x10 + 0x02, names.at(i) + tr("结果%1").arg(numb + 1));
            tmpMsg.insert(addr + numb*0x10 + 0x03, names.at(i) + tr("判定%1").arg(numb + 1));
        }
    }
    qSort(numbs.begin(), numbs.end());

    title.clear();
    for (int i=0; i < numbs.size(); i++) {
        title.append(tmpMsg.value(numbs.at(i)).toString());
    }
    file->write(ToGbk(title.join(",")));
    file->write("\n");
    return 0;
}

int SqlExport::exportData(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_4).toString();
    quint64 from = msg.value(Qt::Key_9).toLongLong();
    quint64 stop = msg.value(Qt::Key_A).toLongLong();
    QString type = msg.value(Qt::Key_B).toString();
    volatile quint64 uuid = from;

    QSqlQuery query(QSqlDatabase::database(name));
    quint64 numb = 0;
    QStringList lineBuffer;
    QTmpMap bufstd;
    for (int i=0; i < numbs.size(); i++) {
        bufstd[numbs.at(i)] = QString(" ");
    }
    QTmpMap buftmp = bufstd;
    int t = 0;
    while (1) {
        QString cmd = tr("select * from aip_record where ");
        cmd += tr(" R_UUID > %1 and R_UUID < %2").arg(uuid).arg(stop);
        if (!type.isEmpty()) {
            cmd += tr(" and R_TYPE = '%1'").arg(type);
        }
        if (!query.exec(cmd)) {
            showText(tr("查询数据失败,%1").arg(query.lastError().text()));
            return Qt::Key_Stop;
        }
        while (query.next()) {
            numb = query.value(1).toInt();
            buftmp.insert(numb, query.value(3).toString());
            if (numb == 0xFFFF) {
                uuid = query.value(0).toLongLong();
                foreach(int n, numbs) {
                    lineBuffer.append(buftmp.value(n).toString());
                }
                file->write(ToGbk(lineBuffer.join(",")));
                file->write("\n");
                lineBuffer.clear();
                buftmp = bufstd;
                t++;
                if (t % LENTH == 0)
                    break;
            }
        }
        tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
        tmpMsg.insert(Qt::Key_2, t*100/quan);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        if (t >= quan || !query.next())
            break;
    }
    return Qt::Key_Meta;
}

QByteArray SqlExport::ToGbk(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->fromUnicode(inStr);
}

void SqlExport::showText(QString err)
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
    tmpMsg.insert(Qt::Key_1, err);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void SqlExport::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Book:
        if (exportFile(msg) == Qt::Key_Stop) {
            tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
            tmpMsg.insert(Qt::Key_2, 100);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
            showText(tr("导出失败"));
        }
        file->close();
        break;
    default:
        break;
    }
}

