#ifndef TYPSETPWR_H
#define TYPSETPWR_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QShowEvent>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>

#include "main.h"
#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"
#include "boxqimage.h"

#define CACHEPWR 0x10

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
    void initTimeBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoChange();
    void autoInput();
    void drawWave();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *iView;
    BoxQModel *iMode;
    QTableView *vView;
    BoxQModel *vMode;
    QTableView *pView;
    BoxQModel *pMode;
    QTableView *tView;
    BoxQModel *tMode;
    QStringList drivers;
    QStringList sources;
    QStringList speeds;
    QStringList turns;
    QVariantMap config;
    QVariantMap tmpMap;
    BoxQImage *wave;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETPWR_H
