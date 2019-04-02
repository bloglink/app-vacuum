/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       反嵌配置界面
*******************************************************************************/
#include "typsetmag.h"

TypSetMag::TypSetMag(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetMag::initUI()
{
    initLayout();
    initViewBar();
    initWaveBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetMag::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetMag::initViewBar()
{
    wView = new QTableWidget(this);
    wView->setRowCount(4);
    wView->setColumnCount(2);
    wView->verticalHeader()->setVisible(false);
    wView->horizontalHeader()->setVisible(false);
    wView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(wView);
}

void TypSetMag::initWaveBar()
{
    QStringList port;
    for (int i=0; i < 8; i++) {
        port << QString::number(i+1);
    }
    for (int i=0; i < 8; i++) {
        QGridLayout *box = new QGridLayout;
        box->setColumnStretch(0, 1);
        box->setSpacing(12);

        BoxQImage *wave = new BoxQImage(this);
        box->addWidget(wave, 0, 0, 4, 1);
        waves.append(wave);

        QCheckBox *check = new QCheckBox(tr("反嵌%1").arg(i+1), this);
        box->addWidget(check, 0, 1, 1, 2);
        checks.append(check);
        connect(check, SIGNAL(clicked(bool)), this, SLOT(change()));

        box->addWidget(new QLabel(tr("端一:")), 1, 1);
        QComboBox *port1 = new QComboBox(this);
        port1->setView(new QListView);
        port1->addItems(port);
        port1->setFixedSize(97, 40);
        box->addWidget(port1, 1, 2);
        ports1.append(port1);
        connect(port1, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));

        box->addWidget(new QLabel(tr("端二:")), 2, 1);
        QComboBox *port2 = new QComboBox(this);
        port2->setView(new QListView);
        port2->addItems(port);
        port2->setFixedSize(97, 40);
        box->addWidget(port2, 2, 2);
        ports2.append(port2);
        connect(port2, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));

        box->addWidget(new QLabel(tr("上限:")), 3, 1);
        QSpinBox *upper = new QSpinBox(this);
        upper->setButtonSymbols(QAbstractSpinBox::NoButtons);
        upper->setFixedSize(97, 40);
        box->addWidget(upper, 3, 2);
        uppers.append(upper);
        connect(upper, SIGNAL(valueChanged(int)), this, SLOT(change()));
        if (i == 0)
            connect(upper, SIGNAL(valueChanged(int)), this, SLOT(autoInput()));

        QFrame *frm = new QFrame(this);
        frm->setLayout(box);
        wView->setCellWidget(i%4, i/4, frm);
    }
}

void TypSetMag::initButtonBar()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    QStringList turns;
    turns << "CW" << "CCW" << "NULL";

    turnBox = new QComboBox(this);
    turnBox->addItems(turns);
    turnBox->setMinimumSize(80, 44);
    turnBox->setView(new QListView);
    connect(turnBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("旋向"), this));
    btnLayout->addWidget(turnBox);

    turnBox2 = new QComboBox(this);
    turnBox2->addItems(turns);
    turnBox2->setMinimumSize(80, 44);
    turnBox2->setView(new QListView);
    connect(turnBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("旋向2"), this));
    btnLayout->addWidget(turnBox2);

    modeBox = new QComboBox(this);
    modeBox->addItem(tr("反嵌"));
    modeBox->addItem(tr("感应"));
    modeBox->addItem(tr("混合"));
    modeBox->setMinimumSize(80, 44);
    modeBox->setView(new QListView);
    connect(modeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    connect(modeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(visibleChange()));
    modeLbl = new QLabel(tr("旋向方式"));
    btnLayout->addWidget(modeLbl);
    btnLayout->addWidget(modeBox);

    modeBox2 = new QCheckBox(tr("Y型切换"), this);
    btnLayout->addWidget(modeBox2);

    voltBox = new QDoubleSpinBox(this);
    voltBox->setDecimals(1);
    voltBox->setFixedSize(80, 40);
    voltBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(voltBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    voltLbl = new QLabel(tr("电压"));
    btnLayout->addWidget(voltLbl);
    btnLayout->addWidget(voltBox);

    freqBox = new QDoubleSpinBox(this);
    freqBox->setDecimals(1);
    freqBox->setFixedSize(80, 40);
    freqBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(freqBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    freqLbl = new QLabel(tr("频率"));
    btnLayout->addWidget(freqLbl);
    btnLayout->addWidget(freqBox);

    btnLayout->addStretch();

    btnWorkL = new QRadioButton(tr("左工位"), this);
    btnWorkL->setChecked(true);
    btnWorkL->setMinimumSize(80, 44);
    btnLayout->addWidget(btnWorkL);

    btnWorkR = new QRadioButton(tr("右工位"), this);
    btnWorkR->setMinimumSize(80, 44);
    btnLayout->addWidget(btnWorkR);

    QPushButton *btnWaveL = new QPushButton(tr("采集"), this);
    btnWaveL->setMinimumSize(97, 44);
    connect(btnWaveL, SIGNAL(clicked(bool)), this, SLOT(sample()));
    btnLayout->addWidget(btnWaveL);

    QPushButton *btnSave = new QPushButton(tr("保存"), this);
    btnSave->setMinimumSize(97, 44);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    btnLayout->addWidget(btnSave);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(btnLayout);
    layout->addWidget(box);
}

void TypSetMag::initItemDelegate()
{
    isInit = false;
    isSave = false;
}

void TypSetMag::initSettings()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int works = tmpSet.value(back + 0x05).toInt();
    btnWorkL->setVisible((works == 2) ? true : false);
    btnWorkR->setVisible((works == 2) ? true : false);
    int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
    int wmag = tmpSet.value(4000 + Qt::Key_G).toInt();  // 波形存储地址
    turnBox->setCurrentIndex(tmpSet.value(addr + 0).toInt());
    modeBox->setCurrentIndex(tmpSet.value(addr + 1).toInt());
    voltBox->setValue(tmpSet.value(addr + 2).toDouble());
    freqBox->setValue(tmpSet.value(addr + 3).toDouble());
    turnBox2->setCurrentIndex(tmpSet.value(addr + 4).toInt());
    modeBox->setVisible(tmpSet.value(back + 12).toInt() == 3 ? true : false);
    modeLbl->setVisible(tmpSet.value(back + 12).toInt() == 3 ? true : false);
    modeBox2->setChecked((tmpSet.value(addr + 5).toInt() == 0) ? false : true);

    addr += CACHEMAG;
    for (int i=0; i < checks.size(); i++) {
        int t = tmpSet.value(addr + i).toInt();
        checks.at(i)->setChecked(t == 0 ? false : true);
        QStringList mPoint;
        if (t != 0) {
            for (int k=0; k < MAG_SIZE; k++) {
                int p = tmpSet.value(wmag + MAG_SIZE*i + k).toInt() * 100 / 256;
                mPoint.append(QString::number(p));
            }
        }
        QVariantMap tmp;
        tmp.insert("width", 2);
        tmp.insert("index", 0);
        tmp.insert("frame", 1);
        tmp.insert("shade", 1);
        tmp.insert("color", int(Qt::green));
        tmp.insert("point", mPoint);
        waves.at(i)->setLines(tmp);
        waves.at(i)->update();
    }

    addr += CACHEMAG;
    for (int i=0; i < checks.size(); i++) {
        int t = tmpSet.value(addr + i).toInt();
        ports1.at(i)->setCurrentText(QString::number(t));
    }

    addr += CACHEMAG;
    for (int i=0; i < checks.size(); i++) {
        int t = tmpSet.value(addr + i).toInt();
        ports2.at(i)->setCurrentText(QString::number(t));
    }

    addr += CACHEMAG;
    for (int i=0; i < checks.size(); i++) {
        int t = tmpSet.value(addr + i).toInt();
        uppers.at(i)->setValue(t);
    }

    addr += CACHEMAG;
    for (int i=0; i < 8; i++) {
        areasL.insert(i, tmpSet.value(addr + i).toString());
    }
    addr += CACHEMAG;
    for (int i=0; i < 8; i++) {
        freqsL.insert(i, tmpSet.value(addr + i).toString());
    }
    addr += CACHEMAG;
    for (int i=0; i < 8; i++) {
        areasR.insert(i, tmpSet.value(addr + i).toString());
    }
    addr += CACHEMAG;
    for (int i=0; i < 8; i++) {
        freqsR.insert(i, tmpSet.value(addr + i).toString());
    }
    isInit = (this->isHidden()) ? false : true;

    visibleChange();
}

void TypSetMag::saveSettings()
{
    confSettings();
    int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
    int wmag = tmpSet.value(4000 + Qt::Key_G).toInt();  // 波形存储地址

    tmpMsg.insert(addr + 0, turnBox->currentIndex());
    tmpMsg.insert(addr + 1, modeBox->currentIndex());
    tmpMsg.insert(addr + 2, voltBox->value());
    tmpMsg.insert(addr + 3, freqBox->value());
    tmpMsg.insert(addr + 4, turnBox2->currentIndex());
    tmpMsg.insert(addr + 5, modeBox2->isChecked() ? 1 : 0);

    addr += CACHEMAG;
    for (int i=0; i < checks.size(); i++) {
        tmpMsg.insert(addr + i, checks.at(i)->isChecked() ? "1" : "0");
        if (checks.at(i)->isChecked()) {
            for (int t=0; t < MAG_SIZE; t++) {
                int w = tmpSet.value(wmag + MAG_SIZE*i + t).toInt();
                tmpMsg.insert(wmag + MAG_SIZE*i + t, w);
            }
        }
    }
    addr += CACHEMAG;
    for (int i=0; i < ports1.size(); i++) {
        tmpMsg.insert(addr + i, ports1.at(i)->currentText());
    }
    addr += CACHEMAG;
    for (int i=0; i < ports2.size(); i++) {
        tmpMsg.insert(addr + i, ports2.at(i)->currentText());
    }
    addr += CACHEMAG;
    for (int i=0; i < uppers.size(); i++) {
        tmpMsg.insert(addr + i, QString::number(uppers.at(i)->value()));
    }
    addr += CACHEMAG;
    for (int i=0; i < areasL.keys().size(); i++) {
        tmpMsg.insert(addr + i, areasL.value(i));
    }
    addr += CACHEMAG;
    for (int i=0; i < freqsL.keys().size(); i++) {
        tmpMsg.insert(addr + i, freqsL.value(i));
    }
    addr += CACHEMAG;
    for (int i=0; i < areasR.keys().size(); i++) {
        tmpMsg.insert(addr + i, areasR.value(i));
    }
    addr += CACHEMAG;
    for (int i=0; i < freqsR.keys().size(); i++) {
        tmpMsg.insert(addr + i, freqsR.value(i));
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    isSave = false;
}

void TypSetMag::confSettings()
{
    int wmag = tmpSet.value(4000 + Qt::Key_G).toInt();  // 波形存储地址
    tmpMap.insert("dir", turnBox->currentIndex());
    tmpMap.insert("mode", modeBox->currentIndex());
    tmpMap.insert("volt", voltBox->value());
    tmpMap.insert("freq", freqBox->value());
    tmpMap.insert("dir2", turnBox2->currentIndex());
    tmpMap.insert("mode2", modeBox2->isChecked() ? 1 : 0);

    QStringList tmp;
    QStringList wl;
    for (int i=0; i < checks.size(); i++) {
        tmp.append(checks.at(i)->isChecked() ? "1" : "0");
        QString ww = QString::number(i);
        if (checks.at(i)->isChecked()) {
            for (int t=0; t < MAG_SIZE; t++) {
                int w = tmpSet.value(wmag + MAG_SIZE*i + t).toInt();
                ww.append(" " + QString::number(w));
            }
        }
        wl.append(ww);
    }
    tmpMap.insert("test", tmp.join(","));
    tmpMap.insert("wave", wl.join(","));
    tmp.clear();
    for (int i=0; i < ports1.size(); i++) {
        tmp.append(ports1.at(i)->currentText());
    }
    tmpMap.insert("port1", tmp.join(","));
    tmp.clear();
    for (int i=0; i < ports2.size(); i++) {
        tmp.append(ports2.at(i)->currentText());
    }
    tmpMap.insert("port2", tmp.join(","));
    tmp.clear();
    for (int i=0; i < uppers.size(); i++) {
        tmp.append(QString::number(uppers.at(i)->value()));
    }
    tmpMap.insert("max", tmp.join(","));
    tmp.clear();
    for (int i=0; i < areasL.keys().size(); i++) {
        tmp.append(areasL.value(i).toString());
    }
    tmpMap.insert("areal", tmp.join(","));
    tmp.clear();
    for (int i=0; i < freqsL.keys().size(); i++) {
        tmp.append(freqsL.value(i).toString());
    }
    tmpMap.insert("freql", tmp.join(","));
    tmp.clear();
    for (int i=0; i < areasR.keys().size(); i++) {
        tmp.append(areasR.value(i).toString());
    }
    tmpMap.insert("arear", tmp.join(","));
    tmp.clear();
    for (int i=0; i < freqsR.keys().size(); i++) {
        tmp.append(freqsR.value(i).toString());
    }
    tmpMap.insert("freqr", tmp.join(","));

    config.insert("MAG", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetMag::autoInput()
{
    if (isInit) {
        int s = uppers.at(0)->value();
        for (int i=1; i < uppers.size(); i++)
            uppers.at(i)->setValue(s);
    }
}

void TypSetMag::sample()
{  // 采集标准波形,命令为6038+dat,dat为工位0x13/0x14
    if (isSave)
        saveSettings();
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6038 %1").arg(btnWorkL->isChecked() ? 0x13 : 0x14));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void TypSetMag::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        isSave = true;
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetMag::visibleChange()
{
    if (modeBox->currentText() == tr("反嵌")) {
        voltBox->setVisible(false);
        freqBox->setVisible(false);
        voltLbl->setVisible(false);
        freqLbl->setVisible(false);
    } else {
        voltBox->setVisible(true);
        freqBox->setVisible(true);
        voltLbl->setVisible(true);
        freqLbl->setVisible(true);
    }
}

void TypSetMag::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 MAG"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetMag::recvNewMsg(QTmpMap msg)
{
    int cmd = msg.value(Qt::Key_1).toInt();
    QString dat = msg.value(Qt::Key_5).toString();
    QStringList ws = dat.split(" ");
    if (cmd == 6035) {  // 反嵌参数,第一位表示第几路反嵌,第45位表示面积,第6位表示频率
        int row = ws.at(0).toInt();
        if (btnWorkL->isChecked()) {
            areasL.insert(row, QString::number(ws.at(3).toInt()*256 + ws.at(4).toInt()));
            freqsL.insert(row, QString::number(ws.at(5).toInt()));
        } else {
            areasR.insert(row, QString::number(ws.at(3).toInt()*256 + ws.at(4).toInt()));
            freqsR.insert(row, QString::number(ws.at(5).toInt()));
        }
        change();
    }
    if (cmd == 6039) {
        if (ws.size() < 2)
            return;
        int row = ws.at(0).toInt();
        ws.removeFirst();
        int addr = tmpSet.value(4000 + Qt::Key_G).toInt();
        QStringList mPoint;
        for (int i=0; i < qMin(MAG_SIZE, ws.size()); i++) {
            double p  = ws.at(i).toInt() * 100.0 / 256;
            mPoint.append(QString::number(p));
            tmpSet.insert(addr + row*MAG_SIZE + i, ws.at(i));
        }
        QVariantMap tmp;
        tmp.insert("width", 2);
        tmp.insert("index", 0);
        tmp.insert("frame", 1);
        tmp.insert("shade", 1);
        tmp.insert("color", int(Qt::green));
        tmp.insert("point", mPoint);
        waves.at(row)->setLines(tmp);
        waves.at(row)->update();
    }
    if (cmd == 6086 && dat.toInt() < 3 && turnBox->currentIndex() != 2) {  // 0不转1反转2正转
        int index = dat.toInt();
        index = (index == 0) ? 2 : index;
        index = (index == 2) ? 0 : index;
        turnBox->setCurrentIndex(index);
    }
}

void TypSetMag::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_News:
        if (this->isHidden())
            break;
        recvNewMsg(msg);
        break;
    case Qt::Key_Down:
        if (msg.value(Qt::Key_1).toString() == this->objectName()) {
            recvShowEvent();
            confSettings();
        }
        break;
    case Qt::Key_Save:
        if (!this->isHidden())
            saveSettings();
        break;
    default:
        break;
    }
}

void TypSetMag::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetMag::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}

