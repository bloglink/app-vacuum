/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统设置
*******************************************************************************/
#include "appsystem.h"

AppSystem::AppSystem(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppSystem::initUI()
{
    initLayout();
    initSystem();
    initDevice();
    initButton();
    initDelegate();
}

void AppSystem::initLayout()
{
    layout = new QHBoxLayout;

    boxLayout = new QVBoxLayout(this);
    boxLayout->addLayout(layout);
}

void AppSystem::initSystem()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("系统设置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("语言设置") << tr("维护提示") << tr("启动方式") << tr("亮度设定")
          << tr("音量设定") << tr("条码起始") << tr("条码长度") << tr("合格报警")
          << tr("报警提示") << tr("测试延时") << tr("产品检测") << tr("条码检测")
          << tr("匝间报警") << tr("启用防呆");
    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QComboBox *input = new QComboBox(this);
        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        input->setView(new QListView);
        input->setFixedHeight(35);
        if (i > names.indexOf(tr("音量设定")))
            input->setEditable(true);
        box->addWidget(input);
        texts.append(input);
    }
    boxLayout->addStretch();
}

void AppSystem::initDevice()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("目标主机"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("自动地址") << tr("默认网关") << tr("网络地址") << tr("子网掩码")
          << tr("挂载地址") << tr("挂载路径") << tr("自动执行") << tr("执行参数");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        ctrls.append(input);
    }
    boxLayout->addStretch();
}

void AppSystem::initButton()
{
    QHBoxLayout *btn = new QHBoxLayout;
    boxLayout->addLayout(btn);
#ifdef __arm__
    btn->addWidget(new QLabel(tr("系统时间"), this));

    time = new QDateTimeEdit(this);
    time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    time->setFixedSize(200, 40);
    btn->addWidget(time);

    QPushButton *btnTime = new QPushButton(this);
    btnTime->setText(tr("设置"));
    btnTime->setFixedSize(97, 44);
    btn->addWidget(btnTime);
    connect(btnTime, SIGNAL(clicked(bool)), this, SLOT(setLocalTime()));
#endif
    btn->addStretch();

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    btn->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppSystem::initDelegate()
{
    QStringList tmp0;
    tmp0 << tr("中文");
    texts.at(0)->addItems(tmp0);

    QStringList tmp1;
    tmp1 << tr("开启");
    texts.at(1)->addItems(tmp1);

    QStringList tmp2;
    tmp2 << tr("按钮启动") << tr("滑罩启动") << tr("脚踏启动") << tr("网络启动");
    texts.at(2)->addItems(tmp2);

    QStringList tmp3;
    tmp3 << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9";
    texts.at(3)->addItems(tmp3);
    texts.at(4)->addItems(tmp3);
}

void AppSystem::initSettings()
{
    int addr = tmpSet.value(2000 + Qt::Key_1).toInt();
    for (int i=0; i < texts.size(); i++) {  // 系统配置存放在0x0020
        QString str = tmpSet.value(addr + i).toString();
        if (i < 5 && i != 1) {
            texts.at(i)->setCurrentIndex(str.toInt());
        }
        if (i >= 5) {
            texts.at(i)->lineEdit()->setText(str);
        }
    }
    int ctrl = tmpSet.value(2000 + Qt::Key_2).toInt();
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        ctrls.at(i)->setText(tmpSet.value(ctrl + i).toString());
    }
#ifdef __arm__
    time->setDateTime(QDateTime::currentDateTime());
#endif
}

void AppSystem::saveSettings()
{
    int addr = tmpSet.value(2000 + Qt::Key_1).toInt();
    for (int i=0; i < texts.size(); i++) {  // 系统配置存放在0x0020
        if (i < 5 && i != 1) {
            tmpMsg.insert(addr + i, QString::number(texts.at(i)->currentIndex()));
        }
        if (i >= 5) {
            tmpMsg.insert(addr + i, texts.at(i)->currentText());
        }
    }
    int ctrl = tmpSet.value(2000 + Qt::Key_2).toInt();
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        tmpMsg.insert(ctrl + i, ctrls.at(i)->text());
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    QSettings *set = new QSettings("./nandflash/userinfo.txt", QSettings::IniFormat);
    set->beginGroup("LOCAL_MACHINE");
    set->setValue("DHCP", ctrls.at(0)->text());
    set->setValue("DefaultGateway", ctrls.at(1)->text());
    set->setValue("IPAddress", ctrls.at(2)->text());
    set->setValue("SubnetMask", ctrls.at(3)->text());
    set->endGroup();
    set->beginGroup("NFS_SERVER");
    set->setValue("IPAddress", ctrls.at(4)->text());
    set->setValue("Mountpath", ctrls.at(5)->text());
    set->endGroup();
    set->beginGroup("USER_EXE");
    set->setValue("Name", ctrls.at(6)->text());
    set->setValue("Parameters", ctrls.at(7)->text());
    set->endGroup();
}

void AppSystem::recvWarnning()
{
    quint64 back = tmpSet.value(1000 + Qt::Key_0).toInt();
    quint64 mode = tmpSet.value(back + backMode).toInt();
    quint64 addr = tmpSet.value(2000 + Qt::Key_1).toInt();
    quint64 curr = QDate::currentDate().toJulianDay();
    quint64 save = tmpSet.value(addr + SystCare).toInt();
    quint64 last = (save / 100);
    quint64 time = (save % 100);
    QString sty = "<p style='font:11pt;color:#FFFFFF;' align='left'>%1</p>";
    QString str;
    if (mode == 1) {
        str += tr("真空测试设备中真空泵的油雾过滤器、油过滤器、粉尘过滤器、气镇滤芯到维护时间，");
        str += tr("请及时清洁或更换");
        str += tr("维护方法请参考<真空泵维护保养方法>");
        str += tr("如果备件不足，请及时与青岛艾普智能仪器有限公司销售人员联系购买");
        str += tr("或直接拨打购买热线：0532-87973318");
        str = sty.arg(str);
    }
    if (mode == 2) {
        str += sty.arg(tr("测试设备中轴承,夹头已到保养时间;"));
        str += sty.arg(tr("请在轴承室中加入润滑油并拆下夹头进行防锈保养"));
    }
    int ret = 0;
    if (time > 60) {
        ret = QMessageBox::warning(this, tr("警告"), str, QMessageBox::Reset | QMessageBox::Ok);
    }
    time = (ret == QMessageBox::Reset) ? 0 : time;
    time = time + ((curr == last) ? 0 : 1);
    save = time + (curr * 100);
    tmpMsg.insert(addr + SystCare, QString::number(save));
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    qDebug() << curr << save << last << time;
}

void AppSystem::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Game:
        recvWarnning();
        break;
    default:
        break;
    }
}

void AppSystem::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

