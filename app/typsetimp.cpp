/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置界面
*******************************************************************************/
#include "typsetimp.h"

TypSetImp::TypSetImp(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetImp::initUI()
{
    initLayout();
    initViewBar();
    initWaveBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetImp::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetImp::initViewBar()
{
    QStringList headers;
    headers << tr("匝间") << tr("端一") << tr("端二") << tr("电压") << tr("次数")
            << tr("面积上限") << tr("面积下限") << tr("差积上限") << tr("差积下限")
            << tr("电晕") << tr("相位") << tr("频率左") << tr("频率右") << tr("计算起点")
            << tr("计算终点");
    mView = new BoxQModel();
    mView->setRowCount(8);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, CHECKIMP)->setCheckable(true);
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKIMP, QHeaderView::Fixed);
    view->setColumnWidth(CHECKIMP, 58);
    for (int i=FREQIMP1; i <= STOPIMP1; i++)
        view->hideColumn(i);
    layout->addWidget(view);
}

void TypSetImp::initWaveBar()
{
    wView = new QTableWidget(this);
    wView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    wView->setRowCount(4);
    wView->setColumnCount(8);
    for (int i=0; i < 8; i++) {
        BoxQLabel *w = new BoxQLabel(this);
        w->setText(tr("匝间%1").arg(i+1), 1);
        waves.append(w);
        wView->setCellWidget((i/4)*2+0, (i%4)*2+0, w);
        wView->setSpan((i/4)*2+0, (i%4)*2+0, 1, 2);
        QTableWidgetItem *itemC = new QTableWidgetItem(tr("<压缩"));
        itemC->setTextAlignment(Qt::AlignCenter);
        wView->setItem((i/4)*2+1, (i%4)*2+0, itemC);
        QTableWidgetItem *itemS = new QTableWidgetItem(tr("拉伸>"));
        itemS->setTextAlignment(Qt::AlignCenter);
        wView->setItem((i/4)*2+1, (i%4)*2+1, itemS);
        connect(w, SIGNAL(clicked()), this, SLOT(lineUpdate()));
    }
    layout->addWidget(wView);

    wView->verticalHeader()->setVisible(false);
    wView->horizontalHeader()->setVisible(false);
    wView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wView->verticalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    wView->verticalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    connect(wView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickView()));
}

void TypSetImp::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;

    vacuoBox = new QCheckBox(tr("真空测试"), this);
    connect(vacuoBox, SIGNAL(clicked(bool)), this, SLOT(swapVacuo()));
    blayout->addWidget(vacuoBox);

    earthBox = new QCheckBox(tr("接地测试"), this);
    connect(earthBox, SIGNAL(clicked(bool)), this, SLOT(swapVacuo()));
    blayout->addWidget(earthBox);

    powerBox = new QCheckBox(tr("放电测试"), this);
    connect(powerBox, SIGNAL(clicked(bool)), this, SLOT(swapVacuo()));
    blayout->addWidget(powerBox);

    blayout->addStretch();

    blayout->addWidget(new QLabel(tr("缓升步长(V)"), this));
    stepBox = new QSpinBox(this);
    stepBox->setFixedSize(70, 40);
    stepBox->setMinimum(10);
    stepBox->setMaximum(3000);
    blayout->addWidget(stepBox);


    btnWorkL = new QRadioButton(this);
    btnWorkL->setChecked(true);
    btnWorkL->setText(tr("左工位"));
    blayout->addWidget(btnWorkL);
    connect(btnWorkL, SIGNAL(clicked(bool)), this, SLOT(swapWave()));

    btnWorkR = new QRadioButton(this);
    btnWorkR->setText(tr("右工位"));
    blayout->addWidget(btnWorkR);
    connect(btnWorkR, SIGNAL(clicked(bool)), this, SLOT(swapWave()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recvWarn()));

    btnWaveS = new QPushButton(this);
    btnWaveS->setText(tr("采集"));
    btnWaveS->setMinimumSize(90, 40);
    connect(btnWaveS, SIGNAL(clicked(bool)), this, SLOT(sample()));
    blayout->addWidget(btnWaveS);

    btnWaveA = new QPushButton(this);
    btnWaveA->setText(tr("添加样品"));
    btnWaveA->setMinimumSize(90, 40);
    connect(btnWaveA, SIGNAL(clicked(bool)), this, SLOT(sampleAdd()));
    blayout->addWidget(btnWaveA);

    btnWaveD = new QPushButton(this);
    btnWaveD->setText(tr("取消样品"));
    btnWaveD->setMinimumSize(90, 40);
    connect(btnWaveD, SIGNAL(clicked(bool)), this, SLOT(sampleDel()));
    blayout->addWidget(btnWaveD);

    btnWaveC = new QPushButton(this);
    btnWaveC->setText(tr("完成采集"));
    btnWaveC->setMinimumSize(90, 40);
    connect(btnWaveC, SIGNAL(clicked(bool)), this, SLOT(sampleCalc()));
    blayout->addWidget(btnWaveC);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(90, 40);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    blayout->addWidget(btnSave);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(blayout);
    layout->addWidget(box);
}

void TypSetImp::initItemDelegate()
{
    isInit = false;
    view->setItemDelegateForColumn(CHECKIMP, new BoxQItems);

    BoxDouble *port = new BoxDouble;
    port->setDecimals(0);
    port->setMininum(1);
    port->setMaxinum(8);
    view->setItemDelegateForColumn(PORTIMP1, port);
    view->setItemDelegateForColumn(PORTIMP2, port);

    BoxDouble *time = new BoxDouble;
    time->setMaxinum(15);
    time->setMininum(1);
    time->setDecimals(0);
    view->setItemDelegateForColumn(TIMEIMP1, time);

    BoxDouble *area = new BoxDouble;
    area->setMaxinum(99.9);
    area->setMininum(0);
    area->setDecimals(1);
    view->setItemDelegateForColumn(AREAIMP1, area);
    view->setItemDelegateForColumn(DIFFIMP1, area);

    BoxDouble *diff = new BoxDouble;
    diff->setMininum(-99.9);
    diff->setMaxinum(0);
    diff->setDecimals(1);
    view->setItemDelegateForColumn(AREAIMP2, diff);
    view->setItemDelegateForColumn(DIFFIMP2, diff);

    BoxDouble *flut = new BoxDouble;
    flut->setMaxinum(9999);
    flut->setDecimals(0);
    view->setItemDelegateForColumn(PHSEIMP1, flut);
    view->setItemDelegateForColumn(FLUTIMP1, flut);
}

void TypSetImp::initSettings()
{
    int setuser = tmpSet.value(DataUser).toInt();
    bool issupper = (tmpSet.value(setuser).toString() == "supper") ? true : false;
    int addr = tmpSet.value(4000 + Qt::Key_6).toInt();
    vacuoBox->setChecked(tmpSet.value(addr + 0x00).toInt() == 0 ? false : true);
    earthBox->setChecked(tmpSet.value(addr + 0x01).toInt() == 0 ? false : true);
    powerBox->setChecked(tmpSet.value(addr + 0x02).toInt() == 0 ? false : true);
    stepBox->setValue(tmpSet.value(addr + 0x03).toInt());
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vmax = tmpSet.value(back + backVolt).toInt();  // 最高电压
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int work = tmpSet.value(back + backWork).toInt();  // 工位数量
    int grnd = tmpSet.value(back + backGrnd).toInt();  // 接地测试
    BoxDouble *volt = new BoxDouble;
    volt->setMaxinum(vmax);
    volt->setMininum(300);
    volt->setDecimals(0);
    view->setItemDelegateForColumn(VOLTIMP1, volt);
    vacuoBox->setVisible(mode == 1 ? true : false);
    earthBox->setVisible(grnd == 1 ? true : false);
    earthBox->setEnabled((grnd == 1 && !issupper) ? false : true);
    powerBox->setVisible(grnd == 1 ? true : false);
    powerBox->setEnabled((grnd == 1 && !issupper) ? false : true);
    btnWorkL->setVisible(work == 2 ? true : false);
    btnWorkR->setVisible(work == 2 ? true : false);
    if (grnd == 1 && !issupper) {
        view->hideColumn(CHECKIMP);
        view->hideColumn(PORTIMP1);
        view->hideColumn(PORTIMP2);
    } else {
        view->showColumn(CHECKIMP);
        view->showColumn(PORTIMP1);
        view->showColumn(PORTIMP2);
    }
    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_6)).toInt() + CACHEIMP;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEIMP*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKIMP) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            if (t == FROMIMP1)
                waves.at(i)->setText(str, 3);
            if (t == STOPIMP1)
                waves.at(i)->setText(str, 2);
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    isInit = (this->isHidden()) ? false : true;
    timer->stop();
}

void TypSetImp::saveSettings()
{
    confSettings();
    int addr = tmpSet.value(4000 + Qt::Key_6).toInt();  // 匝间配置地址
    int wimp = tmpSet.value(4000 + Qt::Key_H).toInt();  // 波形存储地址
    tmpMsg.insert(addr + 0x00, QString::number(vacuoBox->isChecked() ? 1 : 0));
    tmpMsg.insert(addr + 0x01, QString::number(earthBox->isChecked() ? 1 : 0));
    tmpMsg.insert(addr + 0x02, QString::number(powerBox->isChecked() ? 1 : 0));
    tmpMsg.insert(addr + 0x03, QString::number(stepBox->value()));
    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_6)).toInt() + CACHEIMP;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKIMP) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
                if (k != 0) {
                    for (int y=0; y < IMP_SIZE; y++) {
                        int ll = tmpSet.value(wimp + IMP_SIZE*(i*2 + 0) + y).toInt();
                        tmpMsg.insert(wimp + IMP_SIZE*(i*2 + 0) + y, ll);
                        int rr = tmpSet.value(wimp + IMP_SIZE*(i*2 + 1) + y).toInt();
                        tmpMsg.insert(wimp + IMP_SIZE*(i*2 + 1) + y, rr);
                    }
                }
            }
            str = (t == FROMIMP1) ? QString::number(waves.at(i)->getFrom()) : str;
            str = (t == STOPIMP1) ? QString::number(waves.at(i)->getStop()) : str;
            tmpMsg.insert(addr + CACHEIMP*t + i, str);
        }
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetImp::confSettings()
{
    if (vacuoBox->isChecked()) {
        sampleOver();
    }
    btnWaveA->setEnabled((waveCopys.size() == 0) ? false : true);
    wView->setEnabled(false);
    QStringList names;
    names << "test" << "port1" << "port2" << "volt" << "time"
          << "area" << "area_min" << "diff" << "diff_min"
          << "flut" << "phase" << "freqsL" << "freqsR" << "from" << "stop";
    int wimp = tmpSet.value(4000 + Qt::Key_H).toInt();  // 波形存储地址
    QStringList tmp;
    QStringList wl, wr;
    tmpMap.insert("vacuo", QString::number(vacuoBox->isChecked() ? 1 : 0));
    tmpMap.insert("earth", QString::number(earthBox->isChecked() ? 1 : 0));
    tmpMap.insert("power", QString::number(powerBox->isChecked() ? 1 : 0));
    tmpMap.insert("step", QString::number(stepBox->value()));
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKIMP) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
                QString wwl = QString::number(i);
                QString wwr = QString::number(i);
                if (k != 0) {
                    for (int t=0; t < IMP_SIZE; t++) {
                        int ll = tmpSet.value(wimp + IMP_SIZE*(i*2 + 0) + t).toInt();
                        wwl.append(" " + QString::number(ll));
                        int rr = tmpSet.value(wimp + IMP_SIZE*(i*2 + 1) + t).toInt();
                        wwr.append(" " + QString::number(rr));
                    }
                }
                wl.append(wwl);
                wr.append(wwr);
            }
            str = (t == FROMIMP1) ? QString::number(waves.at(i)->getFrom()) : str;
            str = (t == STOPIMP1) ? QString::number(waves.at(i)->getStop()) : str;
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    tmpMap.insert("wave", wl.join(","));
    tmpMap.insert("waveR", wr.join(","));

    config.insert("IMP", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetImp::lineUpdate()
{
    change();
    BoxQLabel *w = qobject_cast<BoxQLabel*>(sender());
    w->setText(QString::number(w->getFrom()), 3);
    w->setText(QString::number(w->getStop()), 2);
}

void TypSetImp::clickView()
{  // 拉伸与压缩波形,命令6056+dat,dat十位为第几行,个位为拉伸(1)或压缩(3);
    int col = wView->currentColumn();
    int row = wView->currentRow();
    if (row == 1 || row == 3) {
        row = ((row - 1) * 2 + col / 2) * 10 + 1 + (col % 2) * 2;
    }
    waveCopys.clear();
    btnWaveC->setText(tr("完成采集"));

    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6056 %1").arg(row));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void TypSetImp::swapVacuo()
{  // 切换真空与非真空模块,命令6052+dat,dat 0为关闭真空泵,1为启动真空泵
    if (this->isHidden())
        return;
    change();
    int index = vacuoBox->isChecked() ? 1 : 0;
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6052 %1").arg(index));
    emit sendAppMap(tmpMap);
    tmpMap.clear();

    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6068 %1").arg(0));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void TypSetImp::swapWave()
{  // 切换左右工位波形显示
    int addr = tmpSet.value(4000 + Qt::Key_6).toInt();
    int wimp = tmpSet.value(4000 + Qt::Key_H).toInt();

    addr += CACHEIMP;
    for (int i=0; i < mView->rowCount(); i++) {
        int ischeck = tmpSet.value(addr + i).toInt();
        int work = (btnWorkL->isChecked()) ? 0 : 1;
        if (ischeck != 0) {
            QVector<double> y(IMP_SIZE);
            for (int k=0; k < IMP_SIZE; k++) {
                y[k] = tmpSet.value(wimp + IMP_SIZE*(i*2+work) + k).toInt() * 100 / 1024;
            }
            waves.at(i)->setWave(y, 4);
            waves.at(i)->update();
        } else {
            QVector<double> y(IMP_SIZE);
            for (int k=0; k < IMP_SIZE; k++) {
                y[k] = -1;
            }
            waves.at(i)->setWave(y, 4);
            waves.at(i)->update();
        }
    }
    btnWorkL->setEnabled(true);
    btnWorkR->setEnabled(true);
    btnWaveS->setEnabled(true);
    btnWaveA->setEnabled(false);
    btnWaveD->setEnabled(false);
    btnWaveC->setEnabled(false);
    btnWaveC->setText(tr("完成采集"));
}

void TypSetImp::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetImp::sample()
{  // 采集标准波形,命令为6041+dat1+dat2,dat1为工位0x13/0x14,dat2为是否真空
    waveCopys.clear();
    wvdata.clear();
    btnWaveC->setText(tr("完成采集"));

    int vacuo = vacuoBox->isChecked() ? 1 : 0;
    int work = btnWorkL->isChecked() ? 0x13 : 0x14;
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6041 %1 %2").arg(work).arg(vacuo));
    emit sendAppMap(tmpMap);
    tmpMap.clear();

    for (int i=0; i < waves.size(); i++) {
        waves.at(i)->setZero();
        waves.at(i)->update();
    }
    sampling();
}

void TypSetImp::sampling()
{
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    int time = tmpSet.value(syst + SystIMPS).toInt() * 1000;
    time = (time == 0) ? 12000 : time;
    timer->start(time);
    btnWaveS->setEnabled(false);
    btnWaveA->setEnabled(false);
    btnWaveC->setEnabled(false);
    btnWaveD->setEnabled(false);
    btnWorkL->setEnabled(false);
    btnWorkR->setEnabled(false);
}

void TypSetImp::sampleAdd()
{  // 添加样品
    int vacuo = vacuoBox->isChecked() ? 1 : 0;
    int work = btnWorkL->isChecked() ? 0x13 : 0x14;
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6084 %1 %2").arg(work).arg(vacuo));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    for (int i=0; i < waves.size(); i++) {
        waves.at(i)->setZero();
        waves.at(i)->update();
    }
    sampling();
}

void TypSetImp::sampleDel()
{  // 取消样品
    if (waveCopys.size() > 1) {
        waveCopys.removeLast();
        btnWaveC->setText(tr("完成采集%1").arg(waveCopys.size()));
    } else {
        waveCopys.removeLast();
        btnWaveC->setText(tr("完成采集"));
        swapWave();
        if (vacuoBox->isChecked()) {
            sampleOver();
        }
    }
}

void TypSetImp::sampleCalc()
{
    if (waveCopys.isEmpty())
        return;
    QTmpMap all = waveCopys.at(0);
    for (int i=0; i < all.keys().size(); i++) {  // 求和
        for (int j=1; j < waveCopys.size(); j++) {
            QTmpMap tmp = waveCopys.at(j);
            QString tmpNext = tmp.value(i).toString();
            QString tmpPrev = all.value(i).toString();
            QStringList tmpts = tmpNext.split(" ");
            QStringList tmpas = tmpPrev.split(" ");
            for (int k=0; k < qMin(tmpts.size(), tmpas.size()); k++) {
                int sum = QString(tmpts.at(k)).toInt() + QString(tmpas.at(k)).toInt();
                tmpas[k] = QString::number(sum);
            }
            all[i] = tmpas.join(" ");
        }
        qDebug() << all[i];
    }
    for (int i=0; i < all.keys().size(); i++) {  // 平均
        QString tmpa = all.value(i).toString();
        QStringList tmpas = tmpa.split(" ");
        for (int k=0; k < tmpas.size(); k++) {
            tmpas[k] = QString::number(QString(tmpas.at(k)).toInt() / waveCopys.size());
        }
        tmpas.insert(0, QString::number(i));
        recvWave(tmpas);
    }
    btnWaveC->setText(tr("完成采集"));
    waveCopys.clear();
    swapWave();
}

void TypSetImp::sampleOver()
{  // 升起真空罩,命令为6046,真空模式下升起真空罩
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6046"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void TypSetImp::sampleWait()
{
    btnWorkL->setEnabled(true);
    btnWorkR->setEnabled(true);
    btnWaveS->setEnabled(true);
}

void TypSetImp::autoInput()
{
    change();
    if (isInit) {
        int r = view->currentIndex().row();
        int c = view->currentIndex().column();
        qDebug() << "change" << "autoinput" << r << c;
        double t;
        switch (c) {
        case VOLTIMP1:
        case TIMEIMP1:
        case AREAIMP1:
        case AREAIMP2:
        case DIFFIMP1:
        case DIFFIMP2:
        case PHSEIMP1:
        case FLUTIMP1:
            if (r != 0)
                return;
            t = mView->item(0, c)->text().toDouble();
            for (int i=0; i < 8; i++)
                mView->item(i, c)->setText(QString::number(t));
            if (c == AREAIMP1) {
                for (int i=0; i < 8; i++)
                    mView->item(i, AREAIMP2)->setText(QString::number(-t));
            }
            if (c == DIFFIMP1) {
                for (int i=0; i < 8; i++)
                    mView->item(i, DIFFIMP2)->setText(QString::number(-t));
            }
            break;
        default:
            break;
        }
    }
}

void TypSetImp::recvWarn()
{
    QMessageBox::warning(this, tr("警告"), tr("匝间采样失败"), QMessageBox::Ok);
    recvShowEvent();
}

void TypSetImp::recvOver()
{
    btnWorkL->setEnabled(true);
    btnWorkR->setEnabled(true);
    waveCopys.append(wvdata);
    btnWaveS->setEnabled(false);
    btnWaveA->setEnabled(waveCopys.size() > 1 ? true : false);
    btnWaveD->setEnabled(true);
    btnWaveC->setText(tr("完成采集%1").arg(waveCopys.size()));
    btnWaveC->setEnabled(waveCopys.size() > 1 ? true : false);
    wView->setEnabled((waveCopys.size() == 1) ? true : false);
    if (waveCopys.size() > 1 && vacuoBox->isChecked())
        sampleOver();
    qDebug() << "Sample over";
}

void TypSetImp::recvParm(QString dat)
{
    change();
    int numb = dat.toInt() / 100;
    int freq = dat.toInt() % 100;
    int parm = (btnWorkL->isChecked()) ? FREQIMP1 : FREQIMP2;
    mView->item(numb, parm)->setText(QString::number(freq));
}

void TypSetImp::recvWave(QStringList ws)
{
    timer->stop();
    if (ws.size() < 2)
        return;
    int addr = tmpSet.value(4000 + Qt::Key_H).toInt();
    int work = btnWorkL->isChecked() ? 0 : 1;
    int numb = ws.at(0).toInt();
    ws.removeFirst();
    QVector<double> y(IMP_SIZE);
    for (int i=0; i < qMin(IMP_SIZE, ws.size()); i++) {
        y[i] = ws.at(i).toInt() * 100 / 1024;
        tmpSet.insert(addr + IMP_SIZE*(numb*2 + work) + i, ws.at(i));
    }
    waves.at(numb)->setWave(y, 4);
    waves.at(numb)->update();
    wvdata.insert(numb, ws.join(" "));
}

void TypSetImp::recvNewMsg(QTmpMap msg)
{
    int cmd = msg.value(Qt::Key_1).toInt();
    QString dat = msg.value(Qt::Key_5).toString();
    QStringList ws = dat.split(" ");
    if (cmd == 6055)
        recvParm(dat);
    if (cmd == 6042)
        recvWave(ws);
    if (cmd == 6085)
        recvOver();
}

void TypSetImp::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 IMP"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
    swapWave();
}

void TypSetImp::recvAppMsg(QTmpMap msg)
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

void TypSetImp::showEvent(QShowEvent *e)
{
    QTimer::singleShot(2000, this, SLOT(sampleWait()));
    recvShowEvent();
    btnWaveS->setEnabled(false);
    int index = vacuoBox->isChecked() ? 1 : 0;
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6052 %1").arg(index));
    emit sendAppMap(tmpMap);
    tmpMap.clear();

    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6068 %1").arg(0));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    this->setFocus();
    e->accept();
}

void TypSetImp::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}
