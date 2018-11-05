/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度配置界面
*******************************************************************************/
#include "typsetacw.h"

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
            << tr("ARC");
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
    mView->setData(mView->index(4, CHECKACW), Qt::Checked, Qt::CheckStateRole);
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->setFixedHeight(120);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));
    for (int i=0; i < 3; i++)
        view->hideColumn(i);
    for (int i=0; i < 4; i++)
        view->hideRow(i);
    layout->addStretch();
    layout->addWidget(view);
}

void TypSetAcw::initButtonBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);
    layout->addStretch();

    vacuoBox = new QCheckBox(tr("真空测试"), this);
    connect(vacuoBox, SIGNAL(clicked(bool)), this, SLOT(change()));
    blayout->addWidget(vacuoBox);
    blayout->addStretch();
    vacuoBox->setToolTip(tr("选中后请将交耐测试移动到匝间之前"));

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
}

void TypSetAcw::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_4)).toInt();  // 交耐配置地址
    vacuoBox->setChecked(tmpSet.value(addr).toInt() == 0 ? false : true);
    addr += CACHEACW;
    for (int t=1; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString real = QString::number(tmpSet.value(addr + CACHEACW*t + i).toDouble());
            real = (t == FREQACW1) ? (freqs.at(real.toInt() % freqs.size())) : real;
            mView->item(i, t)->setText(real);
        }
    }

    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vmax = tmpSet.value(back + backVolt).toInt();  // 最高电压
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    BoxDouble *volt = new BoxDouble;
    volt->setMaxinum(vmax);
    volt->setMininum(300);
    volt->setDecimals(0);
    view->setItemDelegateForColumn(VOLTACW1, volt);
    if (mode != 1) { // 非真空模式
        vacuoBox->setChecked(false);
        vacuoBox->hide();
    } else {
        vacuoBox->show();
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetAcw::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_4)).toInt();  // 交耐配置地址
    tmpMsg.insert(addr + 0, QString::number(vacuoBox->isChecked() ? 1 : 0));
    addr += CACHEACW;
    for (int t=1; t < mView->columnCount(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString real = mView->item(i, t)->text();
            real = (t == FREQACW1) ? QString::number(freqs.indexOf(real)) : real;
            tmpMsg.insert(addr + CACHEACW*t + i, real);
        }
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetAcw::confSettings()
{
    QStringList names;
    names << "test" << "port1" << "port2" << "volt" << "max" << "min"
          << "time" << "freq" << "arc";
    QStringList tmp;
    tmpMap.insert("vacuo", tmp.join(","));
    tmp.clear();
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKACW) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == FREQACW1) {
                str = QString::number(freqs.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    config.insert("ACW", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetAcw::autoChange()
{
    change();
    if (isInit) {
        int r = view->currentIndex().row();
        int c = view->currentIndex().column();
        if (c == FREQACW1) {
            QString next = mView->item(r, c)->text();
            mView->item(r, c)->setText(next == "50" ? "60" : "50");
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
