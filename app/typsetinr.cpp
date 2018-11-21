/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置界面
*******************************************************************************/
#include "typsetinr.h"

TypSetInr::TypSetInr(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetInr::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetInr::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetInr::initViewBar()
{
    QStringList headers;
    headers << tr("绝缘") << tr("高端") << tr("低端") << tr("电压(V)")
            << tr("电阻下限(MΩ)") << tr("电阻上限(MΩ)") << tr("时间(s)");
    mView = new BoxQModel();
    mView->setRowCount(5);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
    }
    mView->setData(mView->index(4, CHECKINR), Qt::Checked, Qt::CheckStateRole);
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(change()));

    view = new QTableView(this);
    view->setModel(mView);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKINR, QHeaderView::Fixed);
    view->setColumnWidth(CHECKINR, 54);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoInput(QModelIndex)));
    layout->addStretch();
    layout->addWidget(view);
}

void TypSetInr::initButtonBar()
{
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 44);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QHBoxLayout *blayout = new QHBoxLayout;
    blayout->addStretch();
    blayout->addWidget(btnSave);
    layout->addLayout(blayout);
    layout->addStretch();
}

void TypSetInr::initItemDelegate()
{
    isInit = false;
    volts << "500" << "1000";
    ports << "PE" << "1" << "4" << "7";
    view->setItemDelegateForColumn(CHECKINR, new BoxQItems);
    view->setItemDelegateForColumn(PORTINR1, new BoxQItems);
    BoxDouble *port = new BoxDouble;
    port->setDecimals(0);
    port->setMininum(1);
    port->setMaxinum(8);
    view->setItemDelegateForColumn(PORTINR2, port);
    view->setItemDelegateForColumn(VOLTINR1, new BoxQItems);

    BoxDouble *real = new BoxDouble;
    real->setMaxinum(500);
    view->setItemDelegateForColumn(LOWERINR, real);
    view->setItemDelegateForColumn(UPPERINR, real);

    BoxDouble *doubleSpinBox = new BoxDouble;
    doubleSpinBox->setMininum(0.5);
    doubleSpinBox->setMaxinum(999.9);
    view->setItemDelegateForColumn(TIMEINR1, doubleSpinBox);
}

void TypSetInr::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_3)).toInt();  // 绝缘配置地址
    for (int t=0; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEINR*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKINR) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            if (t == PORTINR1 || t == PORTINR2) {
                str = tmpSet.value(addr + CACHEINR*t + i).toString();
            }
            str = (t == VOLTINR1) ? (volts.at(str.toInt() % volts.size())) : str;
            mView->item(i, t)->setText(str);
        }
    }
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vacu = tmpSet.value(back + backVacu).toInt();

    if (vacu == 0 || vacu == 1) {  // 非真空/真空
        for (int i=0; i < 3; i++)
            view->hideColumn(i);
        for (int i=0; i < 4; i++)
            view->hideRow(i);
        view->setFixedHeight(120);
    }
    if (vacu == 2) {  // 相间
        view->hideRow(4);
        view->setFixedHeight(240);
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetInr::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_3)).toInt();
    for (int t=0; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKINR) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == PORTINR1 || t == PORTINR2) {
                str = mView->index(i, t).data().toString();
            }
            str = (t == VOLTINR1) ? QString::number(volts.indexOf(str)) : str;
            tmpMsg.insert(addr + CACHEINR*t + i, str);
        }
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetInr::confSettings()
{
    QStringList names;
    names << "test" << "port1" << "port2" << "volt" << "min" << "max" << "time";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKINR) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == PORTINR1 || t == PORTINR2) {
                str = mView->index(i, t).data().toString();
            }
            if (t == VOLTINR1) {
                str = QString::number(volts.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    config.insert("IR", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetInr::autoInput(QModelIndex index)
{
    if (isInit) {
        int c = index.column();
        if (c == PORTINR1) {
            QString dat = index.data().toString();
            int x = ports.indexOf(dat);
            mView->setData(index, ports.at((x+1)%ports.size()), Qt::DisplayRole);
        }
        if (c == VOLTINR1) {
            QString dat = index.data().toString();
            mView->setData(index, (dat == "500") ? "1000" : "500", Qt::DisplayRole);
            change();
        }
    }
}

void TypSetInr::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetInr::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 IR"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetInr::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
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

void TypSetInr::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetInr::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}

