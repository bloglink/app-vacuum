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
    mView->setSort(0, Qt::DescendingOrder);
    mView->select();
    QStringList headers;
    headers << tr("编号") << tr("日期") << tr("启动") << tr("结束") << tr("型号")
            << tr("条码") << tr("用户") << tr("工位") << tr("温度") << tr("判定");
    for (int i=0; i < mView->columnCount(); i++)
        mView->setHeaderData(i, Qt::Horizontal, headers.at(i));
    tView = new QTableView(this);
    tView->setModel(mView);
    tView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    tView->setColumnWidth(5, 200);
    tView->hideColumn(0);
    connect(tView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickView(QModelIndex)));
    layout->addWidget(tView);

    mItem = new QSqlTableModel(this, QSqlDatabase::database("record"));
    mItem->setTable("aip_record");
    mItem->select();

    QStringList titles;
    titles << tr("项目") << tr("参数") << tr("结果") << tr("判定");
    tItem = new QTableWidget(this);
    tItem->setColumnCount(titles.size());
    tItem->setFixedSize(600, 200);
    tItem->setHorizontalHeaderLabels(titles);
    tItem->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tItem);
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

    QDateTime fromdate = QDateTime::currentDateTime();
    fromdate = fromdate.addDays(-1);

    from = new QDateTimeEdit(this);
    from->setFixedHeight(40);
    from->setCalendarPopup(true);
    from->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    from->setDateTime(fromdate);
    blayout->addWidget(new QLabel(tr("起始日期"), this));
    blayout->addWidget(from);

    stop = new QDateTimeEdit(this);
    stop->setFixedHeight(40);
    stop->setCalendarPopup(true);
    stop->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    blayout->addWidget(new QLabel(tr("结束日期"), this));
    blayout->addWidget(stop);

    blayout->addStretch();

    QPushButton *btnSigle = new QPushButton(this);
    btnSigle->setText(tr("删除单项"));
    btnSigle->setFixedSize(80, 44);
    blayout->addWidget(btnSigle);
    connect(btnSigle, SIGNAL(clicked(bool)), this, SLOT(clickSigle()));

    QPushButton *btnDelete = new QPushButton(this);
    btnDelete->setText(tr("删除全部"));
    btnDelete->setFixedSize(80, 44);
    blayout->addWidget(btnDelete);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(clickDelete()));

    QPushButton *btnSelect = new QPushButton(this);
    btnSelect->setText(tr("查询数据"));
    btnSelect->setFixedSize(80, 44);
    blayout->addWidget(btnSelect);
    connect(btnSelect, SIGNAL(clicked(bool)), this, SLOT(clickSelect()));

    QPushButton *btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("导出数据"));
    btnUpdate->setFixedSize(80, 44);
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

void SqlRecord::clickView(QModelIndex index)
{
    int row = index.row();
    quint64 uuid = mView->index(row, 0).data().toLongLong();
    mItem->setFilter(QString("R_UUID=%1").arg(uuid));
    mItem->select();

    row = 0;
    QMap<int, QString> tmp;
    for (int i=0; i < mItem->rowCount(); i++) {
        int index = mItem->index(i, 1).data().toInt();
        QString str = mItem->index(i, 3).data().toString();
        if (index >= 30256 && index < 65535) {
            tmp.insert(index, str);
            if (index % 4 == 0)
                row++;
        }
    }
    tItem->setRowCount(row);
    row = 0;
    foreach(int index, tmp.keys()) {
        if (index % 4 == 0) {
            int add = 0;
            if (index >= 31536 && index < 31536 + 256)
                add = (index % 16 == 0) ? 0 : 1;
            for (int i=0; i < 4-add; i++) {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(tmp.value(index + i));
                item->setTextAlignment(Qt::AlignCenter);
                tItem->setItem(row, i+add, item);
            }
            row++;
        }
    }
}

void SqlRecord::clickSigle()
{
    QString w = tr("数据删除后无法恢复,确定删除吗?");
    int ret = QMessageBox::warning(this, tr("警告"), w, QMessageBox::Cancel | QMessageBox::Ok);
    if (ret == QMessageBox::Ok) {
        if (mItem->rowCount() > 0) {
            QSqlQuery query(QSqlDatabase::database("record"));
            quint64 uuid = mItem->index(0, 0).data().toLongLong();

            QString filter = "delete from aip_sqlite where ";
            filter += QObject::tr("R_UUID == '%1'").arg(uuid);
            if (query.exec(filter))
                clickSelect();
            filter.replace("aip_sqlite", "aip_record");
            query.exec(filter);
        }
    }
}

void SqlRecord::clickDelete()
{
    QString w = tr("数据删除后无法恢复,确定删除吗?");
    int ret = QMessageBox::warning(this, tr("警告"), w, QMessageBox::Cancel | QMessageBox::Ok);
    if (ret == QMessageBox::Ok) {
        QSqlQuery query(QSqlDatabase::database("record"));
        qint64 t1 = from->dateTime().toMSecsSinceEpoch();
        qint64 t2 = stop->dateTime().toMSecsSinceEpoch();
        t1 = (t1 << 20);
        t2 = (t2 << 20);
        QString filter = "delete from aip_sqlite where ";
        filter += QObject::tr("R_UUID >= '%1' and R_UUID <= '%2'").arg(t1).arg(t2);
        if (!type->currentText().isEmpty()) {
            filter += QObject::tr(" and R_TYPE = '%1'").arg(type->currentText());
        }
        if (query.exec(filter))
            clickSelect();
        filter.replace("aip_sqlite", "aip_record");
        query.exec(filter);
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
