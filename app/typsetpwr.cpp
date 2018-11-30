#include "typsetpwr.h"

TypSetPwr::TypSetPwr(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetPwr::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetPwr::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->addStretch();
}

void TypSetPwr::initViewBar()
{
    QStringList headers;
    headers << tr("电参") << tr("电流上限") << tr("电流下限") << tr("功率上限")
            << tr("功率下限") << tr("容压上限") << tr("容压下限") << tr("转向")
            << tr("时间") ;
    mView = new BoxQModel;
    mView->setRowCount(PWR_SIZE);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < PWR_SIZE; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, CHECKPWR)->setCheckable(true);
        mView->item(i, CHECKPWR)->setText(QString("12%1").arg(i+3));
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKPWR, QHeaderView::Fixed);
    view->setColumnWidth(CHECKPWR, 96);
    view->setFixedHeight(240);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange(QModelIndex)));
    layout->addWidget(view);
}

void TypSetPwr::initButtonBar()
{
    QGridLayout *btnLayout = new QGridLayout;

    int row = 0;
    passBox = new QSpinBox(this);
    passBox->setFixedSize(125, 40);
    passBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    passBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(passBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("输出通道")), row, 0);
    btnLayout->addWidget(passBox, row, 1);

    row++;
    vmaxBox = new QSpinBox(this);
    vmaxBox->setMaximum(500);
    vmaxBox->setFixedSize(125, 40);
    vmaxBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    vmaxBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btnLayout->addWidget(new QLabel(tr("电压上限")), row, 0);
    btnLayout->addWidget(vmaxBox, row, 1);

    vminBox = new QSpinBox(this);
    vminBox->setMaximum(500);
    vminBox->setFixedSize(125, 40);
    vminBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    vminBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(vminBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("电压下限")), row, 2);
    btnLayout->addWidget(vminBox, row, 3);

    row++;
    voltBox = new QSpinBox(this);
    voltBox->setMaximum(500);
    voltBox->setFixedSize(125, 40);
    voltBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    voltBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(voltBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("输出电压")), row, 0);
    btnLayout->addWidget(voltBox, row, 1);

    compBox = new QDoubleSpinBox(this);
    compBox->setDecimals(1);
    compBox->setMinimum(-99.9);
    compBox->setFixedSize(125, 40);
    compBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    compBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(compBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("补偿电压")), row, 2);
    btnLayout->addWidget(compBox, row, 3);

    row++;
    smaxBox = new QDoubleSpinBox(this);
    smaxBox->setDecimals(1);
    smaxBox->setFixedSize(125, 40);
    smaxBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    smaxBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(smaxBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("温度上限")), row, 0);
    btnLayout->addWidget(smaxBox, row, 1);

    sminBox = new QDoubleSpinBox(this);
    sminBox->setDecimals(1);
    sminBox->setFixedSize(125, 40);
    sminBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    sminBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(sminBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("温度下限")), row, 2);
    btnLayout->addWidget(sminBox, row, 3);

    QPushButton *btnSave = new QPushButton(tr("保存"), this);
    btnSave->setFixedSize(125, 40);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    btnLayout->addWidget(btnSave, row, 7);

    btnLayout->setColumnStretch(6, 1);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(btnLayout);
    layout->addWidget(box);
    layout->addStretch();
}

void TypSetPwr::initItemDelegate()
{
    isInit = false;
    turns << tr("NULL") << tr("CCW") << tr("CW");

    view->setItemDelegateForColumn(CHECKPWR, new BoxQItems);

    BoxDouble *curr = new BoxDouble;
    view->setItemDelegateForColumn(CMAXPWR1, curr);
    view->setItemDelegateForColumn(CMINPWR1, curr);

    view->setItemDelegateForColumn(PMAXPWR1, curr);
    view->setItemDelegateForColumn(PMINPWR1, curr);

    view->setItemDelegateForColumn(VMAXPWR1, curr);
    view->setItemDelegateForColumn(VMINPWR1, curr);

    view->setItemDelegateForColumn(TURNPWR1, new BoxQItems);

    view->setItemDelegateForColumn(TIMEPWR1, curr);
}

void TypSetPwr::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_7)).toInt();
    passBox->setValue(tmpSet.value(addr + 0).toDouble());
    vmaxBox->setValue(tmpSet.value(addr + 1).toDouble());
    vminBox->setValue(tmpSet.value(addr + 2).toDouble());
    voltBox->setValue(tmpSet.value(addr + 4).toDouble());
    compBox->setValue(tmpSet.value(addr + 5).toDouble());
    smaxBox->setValue(tmpSet.value(addr + 6).toDouble());
    sminBox->setValue(tmpSet.value(addr + 7).toDouble());

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_7)).toInt() + CACHEPWR;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEPWR*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKPWR) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            if (t == TURNPWR1) {
                str = turns.at(str.toInt() % turns.size());
            }
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    for (int i=1; i < PWR_SIZE; i++) {
        view->setRowHidden(i, (mode == 1) ? true : false);
    }
    view->setFixedHeight((mode == 1) ? 120 : 360);
    isInit = (this->isHidden()) ? false : true;
}

void TypSetPwr::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_7)).toInt();

    tmpMsg.insert(addr + 0, QString::number(passBox->value()));
    tmpMsg.insert(addr + 1, QString::number(vmaxBox->value()));
    tmpMsg.insert(addr + 2, QString::number(vminBox->value()));
    tmpMsg.insert(addr + 4, QString::number(voltBox->value()));
    tmpMsg.insert(addr + 5, QString::number(compBox->value()));
    tmpMsg.insert(addr + 6, QString::number(smaxBox->value()));
    tmpMsg.insert(addr + 7, QString::number(sminBox->value()));

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_7)).toInt() + CACHEPWR;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKPWR) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == TURNPWR1) {
                str = QString::number(turns.indexOf(mView->index(i, t).data().toString()));
            }
            tmpMsg.insert(addr + CACHEPWR*t + i, str);
        }
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetPwr::confSettings()
{
    tmpMap.insert("pass", QString::number(passBox->value()));
    tmpMap.insert("vmax", QString::number(vmaxBox->value()));
    tmpMap.insert("vmin", QString::number(vminBox->value()));
    tmpMap.insert("volt", QString::number(voltBox->value()));
    tmpMap.insert("comp", QString::number(compBox->value()));
    tmpMap.insert("tmax", QString::number(smaxBox->value()));
    tmpMap.insert("tmin", QString::number(sminBox->value()));

    QStringList names;
    names << "test" << "curr_max" << "curr_min" << "pwr_max" << "pwr_min"
          << "cap_max" << "cap_min" << "turn" << "time";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKPWR) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1+i);
            }
            if (t == TURNPWR1) {
                str = QString::number(turns.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    config.insert("PWR", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetPwr::autoChange(QModelIndex index)
{
    change();
    if (isInit) {
        int r = index.row();
        int c = index.column();
        int i = 0;
        switch (c) {
        case TURNPWR1:
            i = turns.indexOf(view->currentIndex().data().toString());
            i = qMax(1, (i + 1) % turns.size());
            mView->setData(mView->index(r, c), turns.at(i), Qt::DisplayRole);
            break;
        default:
            break;
        }
    }
}

void TypSetPwr::autoInput()
{
    change();
    if (isInit) {
        int row = view->currentIndex().row();
        int col = view->currentIndex().column();
        if (col == CMAXPWR1 || col == CMINPWR1) {
            double max = mView->item(row, CMAXPWR1)->text().toDouble();
            double min = mView->item(row, CMINPWR1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, CMINPWR1)->setText("0.00");
            }
        }
        if (col == PMAXPWR1 || col == PMINPWR1) {
            double max = mView->item(row, PMAXPWR1)->text().toDouble();
            double min = mView->item(row, PMINPWR1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, PMINPWR1)->setText("0.00");
            }
        }
        if (col == VMAXPWR1 || col == VMINPWR1) {
            double max = mView->item(row, VMAXPWR1)->text().toDouble();
            double min = mView->item(row, VMINPWR1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, VMINPWR1)->setText("0.00");
            }
        }
    }
}

void TypSetPwr::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetPwr::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 PWR"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetPwr::recvAppMsg(QTmpMap msg)
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

void TypSetPwr::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetPwr::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}


