/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度配置界面
*******************************************************************************/
#include "typsetacw.h"

#define ACW_ROW 6

TypSetAcw::TypSetAcw(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetAcw::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}
void TypSetAcw::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetAcw::initViewBar()
{
    QStringList headers;
    headers << tr("交耐") << tr("高端") << tr("低端") << tr("电压(V)")
            << tr("电流上限(mA)") << tr("电流下限(mA)") << tr("时间(s)") << tr("频率(Hz)")
            << tr("ARC") << tr("真空");
    mView = new BoxQModel();
    mView->setRowCount(ACW_ROW);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < ACW_ROW; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
    }
    mView->setData(mView->index(4, CHECKACW), Qt::Checked, Qt::CheckStateRole);
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKACW, QHeaderView::Fixed);
    view->setColumnWidth(CHECKACW, 54);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange(QModelIndex)));
    layout->addStretch();
    layout->addWidget(view);
}

void TypSetAcw::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);
    layout->addStretch();
    blayout->addStretch();

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 44);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    blayout->addWidget(btnSave);
}

void TypSetAcw::initItemDelegate()
{
    isInit = false;
    freqs << "50" << "60";
    ports << "PE" << "1" << "4" << "7";
    view->setItemDelegateForColumn(CHECKACW, new BoxQItems);
    view->setItemDelegateForColumn(PORTACW1, new BoxQItems);
    BoxDouble *port = new BoxDouble;
    port->setDecimals(0);
    port->setMininum(1);
    port->setMaxinum(8);
    view->setItemDelegateForColumn(PORTACW2, port);

    BoxDouble *curr = new BoxDouble;
    curr->setMaxinum(20);
    curr->setDecimals(2);
    view->setItemDelegateForColumn(UPPERACW, curr);
    view->setItemDelegateForColumn(LOWERACW, curr);

    BoxDouble *time = new BoxDouble;
    time->setMaxinum(999.9);
    time->setMininum(0.5);
    time->setDecimals(1);
    view->setItemDelegateForColumn(TIMEACW1, time);

    view->setItemDelegateForColumn(FREQACW1, new BoxQItems);

    BoxDouble *darc = new BoxDouble;
    darc->setMaxinum(9);
    darc->setDecimals(0);
    view->setItemDelegateForColumn(ADDRACWA, darc);

    view->setItemDelegateForColumn(0x09, new BoxQItems);
}

void TypSetAcw::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_4)).toInt();  // 交耐配置地址
    addr = (this->objectName() == "setdcw") ? tmpSet.value(4000 + Qt::Key_5).toInt() : addr;
    addr += CACHEACW;
    for (int t=0; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEACW*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKACW) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            if (t == PORTACW1 || t == PORTACW2) {
                str = tmpSet.value(addr + CACHEACW*t + i).toString();
            }
            if (t == 0x09) {
                str = (str.toInt() == 0) ? "非真空" : "真空";
            }
            str = (t == FREQACW1) ? (freqs.at(str.toInt() % freqs.size())) : str;
            mView->item(i, t)->setText(str);
        }
    }

    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vmax = tmpSet.value(back + backVolt).toInt();  // 最高电压
    int vacu = tmpSet.value(back + backVacu).toInt();

    BoxDouble *volt = new BoxDouble;
    volt->setMaxinum(vmax);
    volt->setMininum(300);
    volt->setDecimals(0);
    view->setColumnHidden(0, (vacu != 2));
    view->setColumnHidden(1, (vacu != 2));
    view->setColumnHidden(2, (vacu != 2));
    view->setColumnHidden(9, (vacu != 1 && vacu != 3));

    view->setRowHidden(0, (vacu != 2));
    view->setRowHidden(1, (vacu != 2));
    view->setRowHidden(2, (vacu != 2));
    view->setRowHidden(3, (vacu != 2));
    view->setRowHidden(4, (vacu == 2));
    view->setRowHidden(5, (vacu != 3));

    view->setFixedHeight((vacu == 2) ? 240 : 120);

    if (vacu == 2) {  // 相间+100mA
        BoxDouble *curr = new BoxDouble;
        curr->setMaxinum(100);
        curr->setDecimals(2);
        volt->setMininum(200);
        view->setItemDelegateForColumn(UPPERACW, curr);
        view->setItemDelegateForColumn(LOWERACW, curr);
    }

    view->setItemDelegateForColumn(VOLTACW1, volt);
    view->setColumnHidden(7, (this->objectName() == "setdcw") ? true : false);
    view->setColumnHidden(8, (this->objectName() == "setdcw") ? true : false);
    BoxDouble *curr = new BoxDouble;
    curr->setMaxinum((this->objectName() == "setdcw") ? 10 : 20);
    curr->setDecimals(2);
    view->setItemDelegateForColumn(UPPERACW, curr);
    view->setItemDelegateForColumn(LOWERACW, curr);

    int test = tmpSet.value(back + backTest).toInt();  // 美芝感应启动
    if (test & 0x04) {
        BoxDouble *port = new BoxDouble;
        port->setDecimals(0);
        port->setMininum(1);
        port->setMaxinum(16);
        view->setItemDelegateForColumn(PORTACW2, port);
    }

    isInit = (this->isHidden()) ? false : true;
}

void TypSetAcw::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_4)).toInt();  // 交耐配置地址
    addr = (this->objectName() == "setdcw") ? tmpSet.value(4000 + Qt::Key_5).toInt() : addr;
    addr += CACHEACW;
    for (int t=0; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = mView->item(i, t)->text();
            if (t == CHECKACW) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == PORTACW1 || t == PORTACW2) {
                str = mView->index(i, t).data().toString();
            }
            if (t == 0x09) {
                str = (str == "非真空") ? "0" : "1";
            }
            str = (t == FREQACW1) ? QString::number(freqs.indexOf(str)) : str;
            tmpMsg.insert(addr + CACHEACW*t + i, str);
        }
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetAcw::confSettings()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vacu = tmpSet.value(back + backVacu).toInt();
    int row = (vacu == 2 || vacu == 3) ? 6 : 5;
    QStringList names;
    names << "test" << "port1" << "port2" << "volt" << "max" << "min"
          << "time" << "freq" << "arc" << "isvacuo";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < row; i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKACW) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == PORTACW1 || t == PORTACW2) {
                str = mView->index(i, t).data().toString();
            }
            if (t == FREQACW1) {
                str = QString::number(freqs.indexOf(mView->index(i, t).data().toString()));
            }
            if (t == 0x09) {
                str = mView->item(i, t)->text();
                str = (str == "非真空") ? "0" : "1";
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    tmpMap.insert("comp", "0,0,0,0,0,0");
    tmpMap.insert("comp_enable", "0,0,0,0,0,0");
    config.insert(this->objectName() == "setdcw" ? "DCW" : "ACW", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetAcw::autoChange(QModelIndex index)
{
    change();
    if (isInit) {
        int c = index.column();
        if (c == PORTACW1) {
            QString dat = index.data().toString();
            int x = ports.indexOf(dat);
            mView->setData(index, ports.at((x+1)%ports.size()), Qt::DisplayRole);
        }
        if (c == FREQACW1) {
            QString next = index.data().toString();
            mView->setData(index, (next == "50" ? "60" : "50"), Qt::DisplayRole);
        }
        if (c == 0x09) {
            QString next = index.data().toString();
            mView->setData(index, (next == "真空" ? "非真空" : "真空"), Qt::DisplayRole);
        }
    }
}

void TypSetAcw::autoInput()
{
    change();
    if (isInit) {
        int r = view->currentIndex().row();
        int c = view->currentIndex().column();
        QString next;
        switch (c) {
        case UPPERACW:
        case LOWERACW:
            next = mView->item(r, c)->text();
            mView->item(r, c)->setText(QString::number(next.toDouble(), 'g', 4));
            if (mView->item(r, LOWERACW)->text().isEmpty() ||
                    mView->item(r, UPPERACW)->text().isEmpty())
                return;
            if (mView->item(r, LOWERACW)->text().toDouble() >
                    mView->item(r, UPPERACW)->text().toDouble()) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(r, LOWERACW)->setText("0.00");
            }
            break;
        default:
            break;
        }
    }
}

void TypSetAcw::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetAcw::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 ACW"));
    if (this->objectName() == "setdcw")
        tmpMap.insert("text", QString("6004 DCW"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetAcw::recvAppMsg(QTmpMap msg)
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

void TypSetAcw::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetAcw::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}
