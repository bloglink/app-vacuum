/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#include "sqlrecord.h"

SqlRecord::SqlRecord(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SqlRecord::initUI()
{
    initLayout();
    initViewBar();
    initTextBar();
}

void SqlRecord::initLayout()
{
    layout = new QVBoxLayout;

    QGroupBox *box = new QGroupBox(this);
    box->setTitle(tr("近期数据"));
    box->setLayout(layout);

    QVBoxLayout *mlayout = new QVBoxLayout(this);
    mlayout->addWidget(box);
}

void SqlRecord::initViewBar()
{
    mView = new QSqlTableModel(this, QSqlDatabase::database("record"));
    mView->setTable("aip_sqlite");
    mView->select();
    QStringList headers;
    headers << tr("编号") << tr("日期") << tr("启动") << tr("结束") << tr("型号")
            << tr("条码") << tr("用户") << tr("工位") << tr("温度") << tr("判定");
    for (int i=0; i < mView->columnCount(); i++)
        mView->setHeaderData(i, Qt::Horizontal, headers.at(i));
    tView = new QTableView(this);
    tView->setModel(mView);
    tView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tView->hideColumn(0);
    layout->addWidget(tView);
}

void SqlRecord::initTextBar()
{
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);

    type = new QComboBox(this);
    type->setEditable(true);
    type->setFixedSize(240, 40);
    type->setView(new QListView);
    blayout->addWidget(new QLabel(tr("测试型号"), this));
    blayout->addWidget(type);

    from = new QDateTimeEdit(this);
    from->setFixedHeight(40);
    from->setCalendarPopup(true);
    from->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    blayout->addWidget(new QLabel(tr("起始日期"), this));
    blayout->addWidget(from);

    stop = new QDateTimeEdit(this);
    stop->setFixedHeight(40);
    stop->setCalendarPopup(true);
    stop->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    blayout->addWidget(new QLabel(tr("结束日期"), this));
    blayout->addWidget(stop);

    blayout->addStretch();

    QPushButton *btnSelect = new QPushButton(this);
    btnSelect->setText(tr("查询数据"));
    btnSelect->setFixedSize(97, 44);
    blayout->addWidget(btnSelect);
    connect(btnSelect, SIGNAL(clicked(bool)), this, SLOT(clickSelect()));

    QPushButton *btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("导出数据"));
    btnUpdate->setFixedSize(97, 44);
    blayout->addWidget(btnUpdate);
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(clickExport()));
}

void SqlRecord::initSettings()
{
    QSqlQuery query(QSqlDatabase::database("config"));
    query.exec("select name from sqlite_master where type='table' order by name");
    tmpTyp.clear();
    type->clear();
    type->addItem("");
    while (query.next()) {
        QString t = query.value(0).toString();
        QString numb = t.mid(1, 4);
        tmpTyp.insert(numb.toInt(), t.mid(6, 50));
        type->addItem(t.mid(6, 50));
    }
}

void SqlRecord::clickSelect()
{
    qint64 t1 = from->dateTime().toMSecsSinceEpoch();
    qint64 t2 = stop->dateTime().toMSecsSinceEpoch();
    t1 = (t1 << 20);
    t2 = (t2 << 20);
    QString filter = QObject::tr("R_UUID >= '%1' and R_UUID <= '%2'").arg(t1).arg(t2);
    if (!type->currentText().isEmpty()) {
        filter += QObject::tr(" and R_TYPE = '%1'").arg(type->currentText());
    }
    mView->setFilter(filter);
    mView->select();
}

void SqlRecord::clickExport()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Book);
    tmpMsg.insert(Qt::Key_1, getSaveFileName());
    tmpMsg.insert(Qt::Key_4, "record");
    tmpMsg.insert(Qt::Key_9, (from->dateTime().toMSecsSinceEpoch() << 20));
    tmpMsg.insert(Qt::Key_A, (stop->dateTime().toMSecsSinceEpoch() << 20));
    tmpMsg.insert(Qt::Key_B, type->currentText());
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

QString SqlRecord::getSaveFileName()
{
    QString name = QDateTime::currentDateTime().toString("yyMMddhhmmss");
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Open Config"),
                                                    name,
                                                    tr("Config Files (*.csv)"));
    fileName.append(fileName.endsWith(".csv") ? "" : ".csv");
    return fileName;
}

void SqlRecord::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void SqlRecord::showEvent(QShowEvent *e)
{
    stop->setDateTime(QDateTime::currentDateTime());
    initSettings();
    clickSelect();
    e->accept();
}
