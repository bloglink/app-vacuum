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
    view->setItemDelegateForColumn(PORTINR2, new BoxQItems);
    view->setItemDelegateForColumn(VOLTINR1, new BoxQItems);

    BoxDouble *real = new BoxDouble;
    real->setMaxinum(500);
    view->setItemDelegateForColumn(LOWERINR, real);
    view->setItemDelegateForColumn(UPPERINR, real);

    BoxDouble *doubleSpinBox = new BoxDouble;
    doubleSpinBox->setMininum(0.5);
    doubleSpinBox->setMaxinum(999.9);
    view->setItemDelegateForColumn(TIMEINR1, doubleSpinBox);

    QStringList nameL;
    nameL  << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8"
           << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";

    buttonL = new QGroupBox(tr("端口"), this);
    QGridLayout *btnlayoutL = new QGridLayout;
    buttonL->setLayout(btnlayoutL);
    for (int i=0; i < nameL.size(); i++) {
        QCheckBox *box = new QCheckBox(nameL.at(i), this);
        checkboxs.append(box);
        connect(box, SIGNAL(clicked(bool)), this, SLOT(autoCheck()));
        if (i == 0) {
            btnlayoutL->addWidget(box, 0, 0);
        } else {
            btnlayoutL->addWidget(box, i/8, i%8);
        }
        if (i % 3 == 0)
            box->setEnabled(false);
    }
    buttonL->resize(560, 150);
    buttonL->hide();
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

    view->setColumnHidden(0, (vacu != 2));
    view->setColumnHidden(1, (vacu != 2));
    view->setColumnHidden(2, (vacu != 2));

    view->setRowHidden(0, (vacu != 2));
    view->setRowHidden(1, (vacu != 2));
    view->setRowHidden(2, (vacu != 2));
    view->setRowHidden(3, (vacu != 2));
    view->setRowHidden(4, (vacu == 2));
    view->setFixedHeight((vacu == 2) ? 240 : 120);

    int test = tmpSet.value(back + backTest).toInt();  // 特殊配置
    buttonL->setFixedHeight((test&0x04) ? 150 : 100);
    checkboxs.at(7)->setText((test&0x04) ? "PE" : "8");
    for (int i=8; i < checkboxs.size(); i++) {
        checkboxs.at(i)->setVisible(test&0x04);  // 输出扩展
    }

    isInit = (this->isHidden()) ? false : true;
}

void TypSetInr::saveSettings()
{
    buttonL->hide();
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
            if (t == PORTINR2) {
                if (str.contains("PE")) {
                    str.replace("PE", "8");
                }
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
        buttonL->hide();
        int c = index.column();
        if (c == PORTINR1) {
            QString dat = index.data().toString();
            int x = ports.indexOf(dat);
            mView->setData(index, ports.at((x+1)%ports.size()), Qt::DisplayRole);
        }
        if (c == PORTINR2) {
            buttonL->raise();
            buttonL->show();
            int x = view->x();
            int y = view->y() + view->height() + 24;
            buttonL->move(x, y);
            QString dat = index.data().toString();
            QString str = mView->index(index.row(), 1).data().toString();
            for (int i=0; i < checkboxs.size(); i++) {
                checkboxs.at(i)->setChecked((dat.contains(checkboxs.at(i)->text())));
                if (checkboxs.at(i)->text() == str && checkboxs.at(i)->isChecked())
                    checkboxs.at(i)->setChecked(false);
            }
        }
        if (c == VOLTINR1) {
            QString dat = index.data().toString();
            mView->setData(index, (dat == "500") ? "1000" : "500", Qt::DisplayRole);
            change();
        }
    }
}

void TypSetInr::autoCheck()
{
    int row = view->currentIndex().row();
    QString str;
    for (int i=0; i < checkboxs.size(); i++) {
        if (checkboxs.at(i)->isChecked())
            str.append(checkboxs.at(i)->text());
    }
    mView->setData(mView->index(row, 2), str, Qt::DisplayRole);
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
    buttonL->hide();
    recvShowEvent();
    e->accept();
}

void TypSetInr::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}

