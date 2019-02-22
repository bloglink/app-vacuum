#ifndef TYPSETLVS_H
#define TYPSETLVS_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "main.h"
#include "appbackup.h"
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"

#define LVS_SIZE 1

#define CHECKLVS 0x00  // 电参测试
#define CMAXLVS1 0x01  // 电流上限
#define CMINLVS1 0x02  // 电流下限
#define PMAXLVS1 0x03  // 功率上限
#define PMINLVS1 0x04  // 功率下限
#define VMAXLVS1 0x05  // 容压上限
#define VMINLVS1 0x06  // 容压下限
#define TURNLVS1 0x07  // 转向
#define TIMELVS1 0x08  // 测试时间

#define CACHELVS 0x10  // 离散数据长度

class TypSetLVS : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetLVS(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoChange(QModelIndex index);
    void autoInput();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QSpinBox *passBox;
    QDoubleSpinBox *compBox;
    QSpinBox *voltBox;

    QStringList turns;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETLVS_H
