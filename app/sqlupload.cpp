/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据上传
*******************************************************************************/
#include "sqlupload.h"

SqlUpload::SqlUpload(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SqlUpload::initUI()
{
    initLayout();
    initBoxText();
    initBoxCtrl();
}

void SqlUpload::initLayout()
{
    layout = new QHBoxLayout(this);
}

void SqlUpload::initBoxText()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("上传配置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("上传模式") << tr("网络地址") << tr("登录用户")<< tr("登录密码")
          << tr("数据库名") << tr("网络端口");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        box->addWidget(new QLabel(names.at(i), this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        texts.append(input);
        if (i == 3) {
            input->setEchoMode(QLineEdit::Password);
        }
    }
    boxLayout->addStretch();

    QHBoxLayout *btn = new QHBoxLayout;

    text = new QLabel(this);
    btn->addWidget(text);
    QPushButton *btnOpen = new QPushButton(this);
    connect(btnOpen, SIGNAL(clicked(bool)), this, SLOT(recvOpen()));
    btnOpen->setText(tr("连接"));
    btnOpen->setFixedSize(97, 44);
    btn->addStretch();
    btn->addWidget(btnOpen);
    boxLayout->addLayout(btn);
}

void SqlUpload::initBoxCtrl()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("备用配置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("上传模式") << tr("网络地址") << tr("登录用户")<< tr("登录密码")
          << tr("数据库名") << tr("网络端口");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        box->addWidget(new QLabel(names.at(i), this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        texts.append(input);
        if (i == 3) {
            input->setEchoMode(QLineEdit::Password);
        }
    }
    boxLayout->addStretch();
    QHBoxLayout *btn = new QHBoxLayout;
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    btn->addStretch();
    btn->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    boxLayout->addLayout(btn);
}

void SqlUpload::initSettings()
{
    int addr = tmpSet.value(2000 + Qt::Key_3).toInt();
    for (int i=0; i < texts.size(); i++) {  // 上传配置存放在0x0120~0x0127
        texts.at(i)->setText(tmpSet.value(addr + i).toString());
    }
}

void SqlUpload::saveSettings()
{
    int addr = tmpSet.value(2000 + Qt::Key_3).toInt();
    for (int i=0; i < texts.size(); i++) {  // 上传配置存放在0x0120~0x0127
        tmpSet.insert(addr + i, texts.at(i)->text());
    }
    tmpSet.insert(Qt::Key_0, Qt::Key_Save);
    tmpSet.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpSet);
}

void SqlUpload::recvOpen()
{
    int addr = tmpSet.value(2000 + Qt::Key_3).toInt();
    int mode = tmpSet.value(addr + 0x00).toInt();
    QString host = tmpSet.value(addr + 0x01).toString();
    QString user = tmpSet.value(addr + 0x02).toString();
    QString pswd = tmpSet.value(addr + 0x03).toString();
    QString base = tmpSet.value(addr + 0x04).toString();
    QString port = tmpSet.value(addr + 0x05).toString();
    QString dsn;
    QString driver;
    if (mode == 0) {
        // 存本地,不上传
        return;
    }
    if (mode == 1) {  // QMYSQL3
        driver = "QMYSQL3";
    }
    if (mode == 2 || mode ==3) {  // QODBC3
        driver = "QODBC3";
        dsn = QString("DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2;").arg(host).arg(base);
    }
    if (mode == 4) {
        driver = "QODBC3";
        dsn = "Oracle";
    }
    qDebug() << "sql open:" << driver;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, "upload");
    db.setHostName(host);
    db.setPort(port.toInt());
    db.setUserName(user);
    db.setPassword(pswd);
    db.setDatabaseName(dsn);
    if (mode == 2 || mode == 3)
        db.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=2;SQL_ATTR_CONNECTION_TIMEOUT=2");
    if (!db.open()) {
        QMessageBox::warning(this, "", db.lastError().text(), QMessageBox::Ok);
        qDebug() << db.lastError();
    } else {
        if (mode == 2 || mode == 3) {
            isConnected = true;
            QTimer *timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(recvRead()));
            timer->start(5000);
            recvRead();
        }
        text->setText("连接成功");
    }
}

void SqlUpload::recvRead()
{
    int addr = tmpSet.value(2000 + Qt::Key_4).toInt();
    QString hostline = tmpSet.value(addr + 0x02).toString();
    QSqlQuery query(QSqlDatabase::database("upload"));
    QString ppn;
    if (!query.exec(tr("select PPN from V_WIP_ID_LINE where LINE_ID = '%1'").arg(hostline))) {
        qDebug() << "sql read:" << query.lastError();
        isConnected = false;
        return;
    }
    if (query.next()) {
        ppn = query.value(0).toString();
    }
    if (ppn.isEmpty())
        return;
    if (ppn != tmpSet.value(DataType).toString()) {  // 切换型号
        qDebug() << ppn;
        reload(ppn);
    }
}

void SqlUpload::reload(QString name)
{
    QSqlQuery query(QSqlDatabase::database("config"));
    query.exec("select name from sqlite_master where type='table' order by name");
    while (query.next()) {
        QString t = query.value(0).toString();
        QString numb = t.mid(1, 4);
        QString type = t.mid(6, 50);
        if (type == name) {
            tmpMsg.insert(DataFile, numb);
            tmpMsg.insert(DataType, name);
            tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
            tmpMsg.insert(Qt::Key_1, "aip_reload");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
            break;
        }
    }
}

void SqlUpload::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Game:
        QTimer::singleShot(5000, this, SLOT(recvOpen()));
        break;
    default:
        break;
    }
}

void SqlUpload::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}


