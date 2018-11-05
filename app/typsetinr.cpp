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
    view->setFixedHeight(120);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoInput()));
    for (int i=0; i < 3; i++)
        view->hideColumn(i);
    for (int i=0; i < 4; i++)
        view->hideRow(i);
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
    for (int t=1; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString real = QString::number(tmpSet.value(addr + CACHEINR*t + i).toDouble());
            real = (t == VOLTINR1) ? (volts.at(real.toInt() % volts.size())) : real;
            mView->item(i, t)->setText(real);
        }
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetInr::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_3)).toInt();
    for (int t=1; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString real = mView->item(i, t)->text();
            real = (t == VOLTINR1) ? QString::number(volts.indexOf(real)) : real;
            tmpMsg.insert(addr + CACHEINR*t + i, real);
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

void TypSetInr::autoInput()
{
    if (isInit) {
        int c = view->currentIndex().column();
        if (c == VOLTINR1) {
            QString dat = view->currentIndex().data().toString();
            mView->setData(view->currentIndex(), (dat == "500") ? "1000" : "500", Qt::DisplayRole);
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

