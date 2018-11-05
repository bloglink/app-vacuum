/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       霍尔配置界面
*******************************************************************************/
#include "typsethal.h"

TypSetHal::TypSetHal(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetHal::initUI()
{
    initLayout();
    initViewBar();
    initWaveBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetHal::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetHal::initViewBar()
{
    QStringList names;
    names << tr("上限") << tr("下限");
    QStringList items;
    items << tr("高电平(V)") << tr("低电平(V)") << tr("占空比(%)") << tr("频率(Hz)");
    iMode = new BoxQModel(this);
    iMode->setRowCount(items.size());
    iMode->setColumnCount(names.size());
    iMode->setHorizontalHeaderLabels(names);
    iMode->setVerticalHeaderLabels(items);
    connect(iMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    iView = new QTableView(this);
    iView->setModel(iMode);
    iView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    iView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    iView->horizontalHeader()->setFixedHeight(30);
    iView->verticalHeader()->setFixedWidth(80);

    QHBoxLayout *blay = new QHBoxLayout;
    blay->addWidget(new QLabel(tr("磁极数:")));
    line = new QSpinBox(this);
    line->setFixedHeight(35);
    blay->addWidget(line);
    connect(line, SIGNAL(valueChanged(int)), this, SLOT(change()));
    blay->addWidget(new QLabel(tr("测试方式:")));
    comb = new QComboBox(this);
    comb->setFixedHeight(35);
    comb->setView(new QListView);
    comb->addItem(tr("空载"));
    comb->addItem(tr("负载"));
    comb->addItem(tr("BEMF"));
    blay->addWidget(comb);
    connect(comb, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    blay->setStretch(1, 1);
    blay->setStretch(3, 1);

    QVBoxLayout *play = new QVBoxLayout;
    play->addWidget(iView);
    play->addLayout(blay);
    QGroupBox *boxTest = new QGroupBox(this);
    boxTest->setTitle(tr("测试项目"));
    boxTest->setFixedHeight(240);
    boxTest->setLayout(play);

    QStringList pname;
    pname << tr("是否显示");
    QStringList parms;
    parms << tr("HU") << tr("HV") << tr("HW") << tr("HA") << tr("HB");
    wMode = new BoxQModel(this);
    wMode->setRowCount(parms.size());
    wMode->setColumnCount(pname.size());
    wMode->setHorizontalHeaderLabels(pname);
    wMode->setVerticalHeaderLabels(parms);

    wView = new QTableView(this);
    wView->setModel(wMode);
    wView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wView->horizontalHeader()->setFixedHeight(30);
    wView->verticalHeader()->setFixedWidth(36);
    connect(wView, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    QHBoxLayout *vlay = new QHBoxLayout;
    vlay->addWidget(wView);
    QGroupBox *boxParm = new QGroupBox(this);
    boxParm->setTitle(tr("显示波形"));
    boxParm->setFixedHeight(240);
    boxParm->setLayout(vlay);

    QHBoxLayout *flay = new QHBoxLayout;
    flay->addWidget(boxTest);
    flay->addWidget(boxParm);
    layout->addStretch();
    layout->addLayout(flay);
}

void TypSetHal::initWaveBar()
{
    QGroupBox *box = new QGroupBox(tr("波形预览"), this);
    QVBoxLayout *wlayout = new QVBoxLayout;
    hWave = new BoxQImage(this);
    wlayout->addWidget(hWave);
    box->setFixedHeight(240);
    box->setLayout(wlayout);
    layout->addWidget(box);
}

void TypSetHal::initButtonBar()
{
    QPushButton *btnSave = new QPushButton(tr("保存"), this);
    btnSave->setMinimumSize(97, 44);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QHBoxLayout *blayout = new QHBoxLayout;
    blayout->addStretch();
    blayout->addWidget(btnSave);

    layout->addLayout(blayout);
    layout->addStretch();
}

void TypSetHal::initItemDelegate()
{
    isInit = false;
    waves << "不显示" << "显示";
    BoxDouble *volt = new BoxDouble;
    iView->setItemDelegateForRow(0, volt);
    iView->setItemDelegateForRow(1, volt);
    BoxDouble *rate = new BoxDouble;
    iView->setItemDelegateForRow(2, rate);
    iView->setItemDelegateForRow(3, rate);

    wView->setItemDelegateForColumn(0, new BoxQItems);
    wView->setItemDelegateForColumn(1, new BoxQItems);
}

void TypSetHal::initSettings()
{
    int row = 0;
    int addr = tmpSet.value(4000 + Qt::Key_B).toInt();  // 负载配置地址

    for (int i=0; i < 10; i++) {
        QString str = tmpSet.value(addr + CACHEHAL*row + i).toString();
        if (i == 8)
            line->setValue(str.toInt());
        if (i == 9)
            comb->setCurrentIndex(str.toInt());
        if (i < 8)
            iMode->setData(iMode->index(i/2, i%2), str, Qt::DisplayRole);
    }
    row++;
    for (int i=0; i < 5; i++) {
        QString str = tmpSet.value(addr + CACHEHAL*row + i).toString();
        str = waves.at((str.toInt()) % waves.size());
        wMode->setData(wMode->index(i, 0), str, Qt::DisplayRole);
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetHal::saveSettings()
{
    confSettings();
    int addr = tmpSet.value(4000 + Qt::Key_B).toInt();  // 负载配置地址
    for (int i=0; i < iMode->rowCount()*2 + 2; i++) {
        if (i < 8) {
            QString str = QString::number(iMode->index(i/2, i%2).data().toDouble());
            tmpMsg.insert(addr + i, str);
        }
        if (i == 8) {
            tmpMsg.insert(addr + i, QString::number(line->value()));
        }
        if (i == 9) {
            tmpMsg.insert(addr + i, QString::number(comb->currentIndex()));
        }
    }
    for (int i=0; i < wMode->rowCount(); i++) {
        QString tmpStr = wMode->index(i, 0).data().toString();
        tmpMsg.insert(addr + CACHEHAL*0x01 + i, QString::number(waves.indexOf(tmpStr)));
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetHal::confSettings()
{
    QStringList title;
    title << "volt_low_max" << "volt_low_min" << "volt_up_max" << "volt_up_min" << "freq_max"
          << "freq_min" << "duty_max" << "duty_min" << "count" << "mode";
    for (int i=0; i < iMode->rowCount()*2+2; i++) {
        if (i < 8) {
            tmpMap.insert(title.at(i), iMode->index(i/2, i%2).data().toString());
        }
        if (i == 8) {
            tmpMap.insert(title.at(i), line->text());
        }
        if (i == 9) {
            tmpMap.insert(title.at(i), QString::number(comb->currentIndex()));
        }
    }
    title.clear();
    for (int i=0; i < wMode->rowCount(); i++) {
        QString tmpStr = wMode->index(i, 0).data().toString();
        title.append(QString::number(waves.indexOf(tmpStr)));
    }
    tmpMap.insert("wave", title.join(","));

    config.insert("HALL", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetHal::autoChange()
{
    change();
    if (isInit) {
        int r = wView->currentIndex().row();
        QString tmpStr = wMode->index(r, 0).data().toString();
        tmpStr = waves.at((waves.indexOf(tmpStr) + 1) % waves.size());
        wMode->setData(wMode->index(r, 0), tmpStr, Qt::DisplayRole);
        drawWave();
    }
}

void TypSetHal::autoInput()
{
    change();
    if (isInit) {
        int r = iView->currentIndex().row();
        double max = iMode->item(r, 0)->text().toDouble();
        double min = iMode->item(r, 1)->text().toDouble();
        if (min > max) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            iMode->item(r, 1)->setText("0.00");
        }
    }
}

void TypSetHal::drawWave()
{
    QList<int> cc;
    cc << int(Qt::cyan) << int(Qt::yellow) << int(Qt::green) << int(Qt::cyan) << int(Qt::yellow);
    for (int i=0; i < 5; i++) {
        QVariantMap tmp;
        tmp.insert("width", 2);
        tmp.insert("index", i);
        tmp.insert("frame", 1);
        tmp.insert("shade", 1);
        tmp.insert("color", cc.at(i));
        QStringList mPoint;
        for (int t=0; t < 400; t++) {
            if (((t + i*30) / 45) % 2 == 0)
                mPoint.append(QString::number(17 + (4-i)*20));
            else
                mPoint.append(QString::number(3 + (4-i)*20));
        }
        if (wMode->index(i, 0).data().toString() == tr("显示"))
            tmp.insert("point", mPoint);
        else
            tmp.insert("point", 0);

        hWave->setLines(tmp);
    }
    hWave->update();
}

void TypSetHal::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetHal::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 HALL"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetHal::recvAppMsg(QTmpMap msg)
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

void TypSetHal::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    drawWave();
    e->accept();
}

void TypSetHal::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}

