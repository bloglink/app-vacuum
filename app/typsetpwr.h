#ifndef TYPSETPWR_H
#define TYPSETPWR_H

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
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"

#define PWR_SIZE 5

#define CHECKPWR 0x00  // 电参测试
#define CMAXPWR1 0x01  // 电流上限
#define CMINPWR1 0x02  // 电流下限
#define PMAXPWR1 0x03  // 功率上限
#define PMINPWR1 0x04  // 功率下限
#define VMAXPWR1 0x05  // 容压上限
#define VMINPWR1 0x06  // 容压下限
#define TURNPWR1 0x07  // 转向
#define TIMEPWR1 0x08  // 测试时间

#define CACHEPWR 0x10  // 离散数据长度

class TypSetPwr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetPwr(QWidget *parent = 0);
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
    void autoChange();
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
    QSpinBox *vmaxBox;
    QSpinBox *vminBox;
    QComboBox *cnvtBox;
    QDoubleSpinBox *compBox;
    QSpinBox *voltBox;
    QDoubleSpinBox *sminBox;
    QDoubleSpinBox *smaxBox;

    QStringList turns;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETPWR_H
