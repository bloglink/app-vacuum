/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       低启配置界面
*******************************************************************************/
#include "typsetlvs.h"

TypSetLVS::TypSetLVS(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetLVS::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetLVS::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->addStretch();
}

void TypSetLVS::initViewBar()
{
    QStringList headers;
    headers << tr("电流上限") << tr("电流下限") << tr("功率上限")
            << tr("功率下限") << tr("测试时间") << tr("输入电压")
            << tr("输出电压") << tr("输出频率") << tr("周期");
    mView = new BoxQModel;
    mView->setRowCount(LVS_SIZE);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < LVS_SIZE; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->setFixedHeight(240);
    layout->addWidget(view);
}

void TypSetLVS::initButtonBar()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    btnLayout->addStretch();

    QPushButton *btnSave = new QPushButton(tr("保存"), this);
    btnSave->setFixedSize(80, 40);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    btnLayout->addWidget(btnSave);

    QGroupBox *box = new QGroupBox(this);
    box->setFixedHeight(90);
    box->setLayout(btnLayout);
    layout->addWidget(box);
    layout->addStretch();
}

void TypSetLVS::initItemDelegate()
{
    isInit = false;
    turns << tr("NULL") << tr("CCW") << tr("CW");

    BoxDouble *curr = new BoxDouble;
    curr->setDecimals(3);
    curr->setMaxinum(5);
    view->setItemDelegateForColumn(0x00, curr);
    view->setItemDelegateForColumn(0x01, curr);
    BoxDouble *pwr = new BoxDouble;
    pwr->setDecimals(1);
    pwr->setMaxinum(1000);
    view->setItemDelegateForColumn(0x02, pwr);
    view->setItemDelegateForColumn(0x03, pwr);
    view->setItemDelegateForColumn(0x04, pwr);
    BoxDouble *cyc = new BoxDouble;
    cyc->setDecimals(0);
    cyc->setMaxinum(99);
    view->setItemDelegateForColumn(0x09, cyc);
}

void TypSetLVS::initSettings()
{
    int item = Qt::Key_A;
    item = (this->objectName() == "setlck") ? Qt::Key_9 : item;
    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + item)).toInt() + CACHELVS;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHELVS*t + i).toDouble();
            QString str = QString::number(real);
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    view->setFixedHeight(120);
    view->setColumnHidden(0x08, (this->objectName() != "setlck"));
    isInit = (this->isHidden()) ? false : true;
}

void TypSetLVS::saveSettings()
{
    confSettings();
    int item = Qt::Key_A;
    item = (this->objectName() == "setlck") ? Qt::Key_9 : item;
    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + item)).toInt() + CACHELVS;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            tmpMsg.insert(addr + CACHELVS*t + i, str);
        }
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetLVS::confSettings()
{
    QStringList names;
    names << "curr_max" << "curr_min" << "pwr_max" << "pwr_min" << "time"
          << "volt_input" << "volt" << "freq" << "point";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        QString str = QString::number(mView->index(0, t).data().toDouble());
        if (this->objectName() == "setlck" && t == 0x04)
            str = QString::number(str.toDouble()*1000);
        tmp.append(str);
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    tmpMap.insert("pass", 1);
    QString str = QString("LVS");
    str = (this->objectName() == "setlck") ? QString("LCK") : str;

    config.insert(str, tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetLVS::autoInput()
{
    change();
    if (isInit) {
        int row = view->currentIndex().row();
        int col = view->currentIndex().column();
        if (col == CMAXLVS1 || col == CMINLVS1) {
            double max = mView->item(row, CMAXLVS1)->text().toDouble();
            double min = mView->item(row, CMINLVS1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, CMINLVS1)->setText("0.00");
            }
        }
        if (col == PMAXLVS1 || col == PMINLVS1) {
            double max = mView->item(row, PMAXLVS1)->text().toDouble();
            double min = mView->item(row, PMINLVS1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, PMINLVS1)->setText("0.00");
            }
        }
    }
}

void TypSetLVS::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetLVS::recvShowEvent()
{
    QString str = QString("LVS");
    str = (this->objectName() == "setlck") ? QString("LCK") : str;

    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 %1").arg(str));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetLVS::recvAppMsg(QTmpMap msg)
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

void TypSetLVS::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetLVS::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}
