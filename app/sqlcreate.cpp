/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据初始化
*******************************************************************************/
#include "sqlcreate.h"

SqlCreate::SqlCreate(QObject *parent) : QObject(parent)
{
}

void SqlCreate::initSqlDir()
{
    QDir dir;
    bool ok = !dir.exists("nandflash") ? dir.mkdir("nandflash") : true;
    dir.setPath("nandflash");
    QStringList files = dir.entryList();
    for (int i=0; i < files.size(); i++) {
        QString str = files.at(i);
        if (str.contains("db-journal"))
            dir.remove(str);
    }
    if (ok) {
        initTmpDat();
        openBackup(dir.exists("backup.db"));
        openSystem(dir.exists("system.db"));
        openConfig(dir.exists("config.db"));
        openRecord(dir.exists("record.db"));
    }
}

void SqlCreate::initTmpDat()
{
    tmpSet.insert((1000 + Qt::Key_0), 10000 + 0x0000);  // 后台数据地址
    tmpSet.insert((1000 + Qt::Key_1), 10000 + 0x0100);  // 电阻后台地址
    tmpSet.insert((1000 + Qt::Key_4), 10000 + 0x0200);  // 绝缘后台地址
    tmpSet.insert((1000 + Qt::Key_8), 10000 + 0x0300);  // 匝间后台地址

    tmpSet.insert((2000 + Qt::Key_0), 22000 + 0000);  // 零散数据地址
    tmpSet.insert((2000 + Qt::Key_1), 20000 + 2120);  // 系统设置存放在2120
    tmpSet.insert((2000 + Qt::Key_2), 20000 + 2140);  // 系统网络存放在2140
    tmpSet.insert((2000 + Qt::Key_3), 20000 + 2160);  // 数据上传存放在2160
    tmpSet.insert((2000 + Qt::Key_4), 20000 + 2180);  // 登录信息存放在2180
    tmpSet.insert((2000 + Qt::Key_6), 20000 + 2200);  // 界面信息存入在2200
    tmpSet.insert((2000 + Qt::Key_5), 20000 + 2300);  // 用户信息存放在2300

    tmpSet.insert((3000 + Qt::Key_0), 30000 + 0x0000);  // 综合结果地址
    tmpSet.insert((3000 + Qt::Key_1), 30000 + 0x0100);  // 电阻结果地址
    tmpSet.insert((3000 + Qt::Key_2), 30000 + 0x0200);  // 反嵌结果地址
    tmpSet.insert((3000 + Qt::Key_3), 30000 + 0x0300);  // 绝缘结果地址
    tmpSet.insert((3000 + Qt::Key_4), 30000 + 0x0400);  // 交耐结果地址
    tmpSet.insert((3000 + Qt::Key_5), 30000 + 0x0500);  // 直耐结果地址
    tmpSet.insert((3000 + Qt::Key_6), 30000 + 0x0600);  // 匝间结果地址
    tmpSet.insert((3000 + Qt::Key_7), 30000 + 0x0700);  // 电参结果地址
    tmpSet.insert((3000 + Qt::Key_8), 30000 + 0x0800);  // 电感结果地址
    tmpSet.insert((3000 + Qt::Key_9), 30000 + 0x0900);  // 堵转结果地址
    tmpSet.insert((3000 + Qt::Key_A), 30000 + 0x0A00);  // 低启结果地址
    tmpSet.insert((3000 + Qt::Key_B), 30000 + 0x0B00);  // 霍尔结果地址
    tmpSet.insert((3000 + Qt::Key_C), 30000 + 0x0C00);  // 负载结果地址
    tmpSet.insert((3000 + Qt::Key_D), 30000 + 0x0D00);  // 空载结果地址
    tmpSet.insert((3000 + Qt::Key_E), 30000 + 0x0E00);  // 反电动势结果地址
    tmpSet.insert((3000 + Qt::Key_G), 30000 + 0x1000);  // 反嵌波形地址
    tmpSet.insert((3000 + Qt::Key_H), 30000 + 0x2000);  // 匝间波形地址
    tmpSet.insert((3000 + Qt::Key_I), 30000 + 0x0110);  // 电阻平衡地址

    tmpSet.insert((4000 + Qt::Key_0), 40000 + 0x0000);  // 综合配置地址
    tmpSet.insert((4000 + Qt::Key_1), 40000 + 0x0100);  // 电阻配置地址
    tmpSet.insert((4000 + Qt::Key_2), 40000 + 0x0200);  // 反嵌配置地址
    tmpSet.insert((4000 + Qt::Key_3), 40000 + 0x0300);  // 绝缘配置地址
    tmpSet.insert((4000 + Qt::Key_4), 40000 + 0x0400);  // 交耐配置地址
    tmpSet.insert((4000 + Qt::Key_5), 40000 + 0x0500);  // 直耐配置地址
    tmpSet.insert((4000 + Qt::Key_6), 40000 + 0x0600);  // 匝间配置地址
    tmpSet.insert((4000 + Qt::Key_7), 40000 + 0x0700);  // 电参配置地址
    tmpSet.insert((4000 + Qt::Key_8), 40000 + 0x0800);  // 电感配置地址
    tmpSet.insert((4000 + Qt::Key_9), 40000 + 0x0900);  // 堵转配置地址
    tmpSet.insert((4000 + Qt::Key_A), 40000 + 0x0A00);  // 低启配置地址
    tmpSet.insert((4000 + Qt::Key_B), 40000 + 0x0B00);  // 霍尔配置地址
    tmpSet.insert((4000 + Qt::Key_C), 40000 + 0x0C00);  // 负载配置地址
    tmpSet.insert((4000 + Qt::Key_D), 40000 + 0x0D00);  // 空载配置地址
    tmpSet.insert((4000 + Qt::Key_E), 40000 + 0x0E00);  // 反电动势配置地址
    tmpSet.insert((4000 + Qt::Key_G), 40000 + 0x1000);  // 反嵌波形地址
    tmpSet.insert((4000 + Qt::Key_H), 40000 + 0x2000);  // 匝间波形地址

    tmpSet.insert(Qt::Key_0, Qt::Key_Xfer);
    emit sendAppMsg(tmpSet);
}

void SqlCreate::openBackup(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/backup.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "backup");
    db.setDatabaseName("./nandflash/backup.db");
    if (!db.open()) {
        qWarning() << "backup:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_backup (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << "aip_backup:" << query.lastError();
        }
        db.transaction();
        initBack(query);
        db.commit();
        query.clear();
    }
}

void SqlCreate::openSystem(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/system.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "system");
    db.setDatabaseName("./nandflash/system.db");
    if (!db.open()) {
        qWarning() << "system:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_system (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << "aip_system:" << query.lastError();
        }
        db.transaction();
        initMisc(query);
        initSyst(query);
        initInfo(query);
        initLoad(query);
        initSign(query);
        initShow(query);
        initUser(query);
        db.commit();
        query.clear();
    }
}

void SqlCreate::openConfig(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/config.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "config");
    db.setDatabaseName("./nandflash/config.db");
    if (!db.open()) {
        qWarning() << "config:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd = "create table if not exists T0001_aip_config (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << "T0001_aip_config:" << query.lastError();
        }
        db.transaction();
        initConf(query);
        db.commit();
        query.clear();
    }
}

void SqlCreate::openRecord(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/record.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "record");
    db.setDatabaseName("./nandflash/record.db");
    if (!db.open()) {
        qWarning() << "record:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_sqlite (";
        cmd += "R_UUID bigint primary key,R_DATE text,R_PLAY text,R_STOP text,R_TYPE text,";
        cmd += "R_CODE text,R_USER text,R_WORK text,R_TEMP text,";
        cmd += "R_ISOK text)";
        if (!query.exec(cmd)) {
            qWarning() << "aip_sqlite:" << query.lastError();
        }
        cmd = "create table if not exists aip_record (";
        cmd += "R_UUID bigint, R_ITEM integer,";
        cmd += "R_TYPE text, R_TEXT text,";
        cmd += "primary key (R_UUID,R_ITEM))";
        if (!query.exec(cmd))
            qWarning() << "aip_record:" << query.lastError();
        query.clear();
    }
}

void SqlCreate::initBack(QSqlQuery query)
{  // 后台设置
    int from = tmpSet.value((1000 + Qt::Key_0)).toInt();
    QStringList parm;
    parm << "AIP8900";      // 产品型号
    parm << "189912001X";   // 产品编号
    parm << "G201801001";   // 评审编号
    parm << "2018-03";      // 出厂日期
    parm << "1";            // 语言支持
    parm << "2";            // 工位数量
    parm << "5000";         // 最高电压
    parm << "0";            // 自动测试
    parm << "2";            // 测试延时
    parm << "192.168.1.56";  // 公司网址
    parm << "0";         // 匝间接地
    parm << "1";         // 测试模式
    for (int i=parm.size(); i < 0x10; i++) {
        parm << "";
    }
    parm << "1" << "1" << "1" << "1" << "0"
         << "1" << "0" << "0" << "0" << "0"
         << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < 0x80; i++) {
        parm << "";
    }
    for (int i=parm.size(); i < 0xA0; i++) {
        parm << "1" << "0";
    }

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initACWB(QSqlQuery query)
{  // 参数校准
    int from = tmpSet.value((1000 + Qt::Key_4)).toInt();
    QStringList parm;
    parm << "500" << "1000" << "1024" << "500"
         << "100" << "500" << "1024" << "500"
         << "50" << "500" << "1024" << "500"
         << "50" << "500" << "1024" << "500"
         << "500" << "4500" << "1024" << "500"
         << "300" << "1500" << "1024" << "500"
         << "10" << "300" << "1024" << "500";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_backup" << query.lastError();
    }
}

void SqlCreate::initIMPB(QSqlQuery query)
{
    int from = tmpSet.value(1000 + Qt::Key_8).toInt();
    QStringList parm;
    parm << "500" << "1000" << "1024" << "500"
         << "1000" << "2000" << "1024" << "500"
         << "2000" << "3000" << "1024" << "500"
         << "3000" << "4000" << "1024" << "500"
         << "4000" << "5000" << "1024" << "500";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_backup" << query.lastError();
    }
}

void SqlCreate::initMisc(QSqlQuery query)
{  // 零散数据
    int from = tmpSet.value((2000 + Qt::Key_0)).toInt();
    QStringList parm;
    parm << "001"  // 当前型号
         << "22305"  // 当前用户
         << "1"  // 自动保存
         << "0";  // 日志输出

    for (int i=parm.size(); i < 10; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initSyst(QSqlQuery query)
{  // 系统设置
    int from = tmpSet.value((2000 + Qt::Key_1)).toInt();
    QStringList parm;
    parm << "0"  // 语言设置
         << "0"  // 测试模式
         << "0"  // 启动方式
         << "9"  // 亮度设定
         << "9"  // 音量设定
         << "0"  // 条码起始
         << "9"  // 条码长度
         << "0.2"  //合格提示
         << "0.5"  // 报警时间
         << "0.2"  // 测试延时
         << "0";  // 产品检测

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initInfo(QSqlQuery query)
{
    int from = tmpSet.value((2000 + Qt::Key_2)).toInt();
    QStringList parm;
    parm << "1"  // 自动地址
         << "192.168.1.1"  // 默认网关
         << "192.168.1.56"  // 网络地址
         << "255.255.255.0"  // 子网掩码
         << "192.168.1.186"  // 挂载地址
         << "/nfs"  // 挂载路径
         << ""  // 自动执行
         << "";  // 执行参数
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}


void SqlCreate::initLoad(QSqlQuery query)
{  // 数据上传
    int from = tmpSet.value((2000 + Qt::Key_3)).toInt();
    QStringList parm;
    parm << tr("0") // 上传模式
         << tr("192.168.1.2") // 网络地址
         << tr("sa") // 登录用户
         << tr("Abc++123")  // 登录密码
         << tr("aip-server") // 数据库名
         << tr("1433") // 网络端口
         << tr("0")
         << tr("192.168.1.55")
         << tr("sa")
         << tr("123")
         << tr("aip-server")
         << tr("1433");
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initSign(QSqlQuery query)
{
    int from = tmpSet.value((2000 + Qt::Key_4)).toInt();
    QStringList parm;
    for (int i=0; i < STEP; i++) {
        parm << "0";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initShow(QSqlQuery query)
{   // 界面权限 name mark role form
    int row = 0;
    int from = tmpSet.value((2000 + Qt::Key_6)).toInt();
    QStringList parm;
    parm << "4" << "4"; // author;tester;
    row++;
    for (int i=parm.size(); i < STEP*row; i++) {
        parm << "";
    }
    parm << "4" << "2" << "2" << "1";  // signin;system;online;master;
    parm << "1" << "0";  // action;logger;
    row++;
    for (int i=parm.size(); i < STEP*row; i++) {
        parm << "";
    }
    parm << "2" << "2" << "2" << "2";  // config,setdcr,setmag,setinr;
    parm << "2" << "2" << "2" << "2";  // setacw,setdcw,setimp,setpwr;
    parm << "2" << "2" << "2" << "2";  // setind,setlck,setlvs,sethal;
    parm << "2" << "2" << "2" << "2";  // setlod,setnld,setbmf;
    row++;
    for (int i=parm.size(); i < STEP*row; i++) {
        parm << "";
    }
    parm << "1" << "1" << "1" << "1";  // record,upload,sdcard,unqual;
    row++;
    for (int i=parm.size(); i < STEP*row; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initUser(QSqlQuery query)
{  // 用户管理 name pass role last save
    int from = tmpSet.value((2000 + Qt::Key_5)).toInt();
    QStringList parm;
    parm << "supper" << "aip9918" << "0" << "" << "1";
    parm << "admin" << "6" << "1" << "" << "1";
    parm << "techo" << "6" << "2" << "" << "1";
    parm << "guest" << "6" << "3" << "" << "1";
    for (int i=parm.size(); i < 100; i+=5) {
        parm << "" << "" << "" << "" << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initType(QSqlQuery query)
{  // 型号管理
    int from = tmpSet.value((2000 + Qt::Key_4)).toInt();
    QStringList parm;
    parm << tr("aip_config");
    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < STEP; i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initConf(QSqlQuery query)
{
    int addr = tmpSet.value((4000 + Qt::Key_0)).toInt();
    QStringList parm;
    parm << "4,3,1,2,6"
         << "ABC-Y"
         << "#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000";  // 综合配置
    for (int i=parm.size(); i < PSIZE; i++) {
        parm << "0";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(addr + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    initDCR(query);
    initMAG(query);
    initINR(query);
    initACW(query);
    initIMP(query);
    initHAL(query);
    initLOD(query);
    initBMF(query);
}

void SqlCreate::initDCR(QSqlQuery query)
{
    int addr = tmpSet.value((4000 + Qt::Key_1)).toInt();
    int numb = 0;
    QStringList parm;
    numb++;
    parm << "0" << "0" << "0" << "0" << "20" << "0" << "10" << "10";
    for (int i=parm.size(); i < STEP*numb; i++)   // 其他参数
        parm << "0";
    numb++;
    parm << "1" << "1" << "1" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < STEP*numb; i++)   // 是否测试
        parm << "0";
    numb++;
    parm << "1" << "2" << "1" << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 端口一
        parm << "0";
    }
    numb++;
    parm << "2" << "3" << "3" << "2" << "2" << "2" << "2" << "2";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 端口二
        parm << "0";
    }
    numb++;
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 线圈材料
        parm << "0";
    }
    numb++;
    parm << "1" << "1" << "1" << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 电阻单位
        parm << "0";
    }
    numb++;
    parm << "120" << "120" << "120" << "120" << "120" << "120" << "120" << "120";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 电阻上限
        parm << "0";
    }
    numb++;
    parm << "80" << "80" << "80" << "80" << "80" << "80" << "80" << "80";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 电阻下限
        parm << "0";
    }
    numb++;
    parm << "100" << "100" << "100" << "100" << "100" << "100" << "100" << "100";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 标准电阻
        parm << "0";
    }
    numb++;
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 线路补偿1
        parm << "0";
    }
    numb++;
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < STEP*numb; i++) {  // 线路补偿2
        parm << "0";
    }
    for (int i=parm.size(); i < PSIZE; i++) {
        parm << "0";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(addr + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initMAG(QSqlQuery query)
{
    int from = tmpSet.value((4000 + Qt::Key_2)).toInt();
    QStringList parm;
    parm << "1";
    for (int i=parm.size(); i < CACHEMAG*(CHECKMAG + 1); i++) {  // 其他参数
        parm << "";
    }
    parm << "1" << "1" << "1" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEMAG*(CHECKMAG + 2); i++) {  // 是否测试
        parm << "";
    }
    parm << "1" << "2" << "1" << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEMAG*(PORTMAG1 + 2); i++) {  // 端口一
        parm << "";
    }
    parm << "2" << "3" << "3" << "2" << "2" << "2" << "2" << "2";
    for (int i=parm.size(); i < CACHEMAG*(PORTMAG2 + 2); i++) {  // 端口二
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEMAG*(UPPERMAG + 2); i++) {  // 上限
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEMAG*(AREAMAG1 + 2); i++) {  // 左面积
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEMAG*(FREQMAG1 + 2); i++) {  // 左频率
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEMAG*(AREAMAG2 + 2); i++) {  // 右面积
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEMAG*(FREQMAG2 + 2); i++) {  // 右频率
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initINR(QSqlQuery query)
{
    int from = tmpSet.value((4000 + Qt::Key_3)).toInt();
    QStringList parm;
    parm << "0" << "0" << "0" << "0" << "1";
    for (int i=parm.size(); i < CACHEMAG*(CHECKINR+1); i++) {  // 测试
        parm << "";
    }
    parm << "1" << "2" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEMAG*(PORTINR1+1); i++) {  // 高端
        parm << "";
    }
    parm << "2" << "3" << "3" << "2" << "2";
    for (int i=parm.size(); i < CACHEMAG*(PORTINR2+1); i++) {  // 低端
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEMAG*(VOLTINR1+1); i++) {  // 电压
        parm << "";
    }
    parm << "500" << "500" << "500" << "500" << "500";
    for (int i=parm.size(); i < CACHEMAG*(LOWERINR+1); i++) {  // 上限
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEMAG*(UPPERINR+1); i++) {  // 下限
        parm << "";
    }
    parm << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEMAG*(TIMEINR1+1); i++) {  // 时间
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initACW(QSqlQuery query)
{
    int from = tmpSet.value((4000 + Qt::Key_4)).toInt();
    QStringList parm;
    parm << "0";
    for (int i=parm.size(); i < CACHEACW*(CHECKACW+1); i++) {  // 其他参数
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "1";
    for (int i=parm.size(); i < CACHEACW*(CHECKACW+2); i++) {  // 测试
        parm << "";
    }
    parm << "1" << "2" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEACW*(PORTACW1+2); i++) {  // 高端
        parm << "";
    }
    parm << "2" << "3" << "3" << "2" << "2";
    for (int i=parm.size(); i < CACHEACW*(PORTACW2+2); i++) {  // 低端
        parm << "";
    }
    parm << "500" << "500" << "500" << "500" << "500";
    for (int i=parm.size(); i < CACHEACW*(VOLTACW1+2); i++) {  // 电压
        parm << "";
    }
    parm << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEACW*(UPPERACW+2); i++) {  // 上限
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEACW*(LOWERACW+2); i++) {  // 下限
        parm << "";
    }
    parm << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEACW*(TIMEACW1+2); i++) {  // 时间
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEACW*(FREQACW1+2); i++) {  // 频率
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEACW*(ADDRACWA+2); i++) {  // ARC
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initIMP(QSqlQuery query)
{
    int from = tmpSet.value((4000 + Qt::Key_6)).toInt();
    QStringList parm;
    parm << "0";
    for (int i=parm.size(); i < CACHEIMP*(CHECKIMP + 1); i++) {  // 其他参数
        parm << "";
    }
    parm << "1" << "1" << "1" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEIMP*(CHECKIMP + 2); i++) {  // 是否测试
        parm << "";
    }
    parm << "1" << "2" << "1" << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEIMP*(PORTIMP1 + 2); i++) {  // 端口一
        parm << "";
    }
    parm << "2" << "3" << "3" << "2" << "2" << "2" << "2" << "2";
    for (int i=parm.size(); i < CACHEIMP*(PORTIMP2 + 2); i++) {  // 端口二
        parm << "";
    }
    parm << "500" << "500" << "500" << "500" << "500" << "500" << "500" << "500";
    for (int i=parm.size(); i < CACHEIMP*(VOLTIMP1 + 2); i++) {  // 电压
        parm << "";
    }
    parm << "1" << "1" << "1" << "1" << "1" << "1" << "1" << "1";
    for (int i=parm.size(); i < CACHEIMP*(TIMEIMP1 + 2); i++) {  // 次数
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEIMP*(AREAIMP1 + 2); i++) {  // 面积上限
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEIMP*(AREAIMP2 + 2); i++) {  // 面积下限
        parm << "";
    }
    parm << "10" << "10" << "10" << "10" << "10" << "10" << "10" << "10";
    for (int i=parm.size(); i < CACHEIMP*(DIFFIMP1 + 2); i++) {  // 差积上限
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEIMP*(DIFFIMP2 + 2); i++) {  // 差积下限
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEIMP*(FLUTIMP1 + 2); i++) {  // 相位
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    for (int i=parm.size(); i < CACHEIMP*(PHSEIMP1 + 2); i++) {  // 电晕
        parm << "";
    }
    parm << "7" << "7" << "7" << "7" << "7" << "7" << "7" << "7";
    for (int i=parm.size(); i < CACHEIMP*(FREQIMP1 + 2); i++) {  // 频率左
        parm << "";
    }
    parm << "7" << "7" << "7" << "7" << "7" << "7" << "7" << "7";
    for (int i=parm.size(); i < CACHEIMP*(FREQIMP2 + 2); i++) {  // 频率右
        parm << "";
    }
    parm << "30" << "30" << "30" << "30" << "30" << "30" << "30" << "30";
    for (int i=parm.size(); i < CACHEIMP*(FROMIMP1 + 2); i++) {  // 计算起点
        parm << "";
    }
    parm << "370" << "370" << "370" << "370" << "370" << "370" << "370" << "370";
    for (int i=parm.size(); i < CACHEIMP*(STOPIMP1 + 2); i++) {  // 计算终点
        parm << "";
    }

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initHAL(QSqlQuery query)
{
    int row = 0;
    int from = tmpSet.value((4000 + Qt::Key_B)).toInt();
    QStringList parm;
    parm << "15" << "0" << "5" << "0" << "60" << "0" << "500" << "0" << "3" << "0";
    row++;
    for (int i=parm.size(); i < CACHEHAL*row; i++) {
        parm << "";
    }
    parm << "1" << "1" << "0" << "0" << "0";
    row++;
    for (int i=parm.size(); i < CACHEHAL*row; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initLOD(QSqlQuery query)
{
    int row = 0;
    int from = tmpSet.value((4000 + Qt::Key_C)).toInt();
    QStringList parm;
    parm << "500" << "0" << "500" << "0" << "500" << "0" << "3000" << "0" << "0" << "0";
    row++;
    for (int i=parm.size(); i < CACHELOD*row; i++) {
        parm << "";
    }
    parm << "310" << "15" << "3.3" << "0" << "5" << "50";
    row++;
    for (int i=parm.size(); i < CACHELOD*row; i++) {
        parm << "";
    }
    parm << "0" << "0" << "0" << "0" << "0" << "1";
    row++;
    for (int i=parm.size(); i < CACHELOD*row; i++) {
        parm << "";
    }
    parm << "0.2"<< "0.2"<< "0.2"<< "0.2"<< "0.2"<< "0.2"<< "0.2"<< "0.2"<< "0.2"<< "0.2";
    row++;
    for (int i=parm.size(); i < CACHELOD*row; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

void SqlCreate::initBMF(QSqlQuery query)
{
    int row = 0;
    int from = tmpSet.value((4000 + Qt::Key_E)).toInt();
    QStringList parm;
    parm << "50" << "0" << "50" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    row++;
    for (int i=parm.size(); i < CACHEBMF*row; i++) {
        parm << "";
    }
    parm << "1000" << "0" << "12" << "5" << "0" << "0";
    row++;
    for (int i=parm.size(); i < CACHELOD*row; i++) {
        parm << "";
    }

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into T0001_aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

