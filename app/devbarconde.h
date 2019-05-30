#ifndef DEVBARCONDE_H
#define DEVBARCONDE_H

#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class DevBarconde : public QObject
{
    Q_OBJECT
public:
    explicit DevBarconde(QObject *parent = 0);

signals:
    void sendAppMap(QVariantMap msg);
public slots:
    bool setOpen(QVariantMap map, int buadrate, QSerialPort::Parity parity);
    bool setQuit(QVariantMap map);
    void readyRead();
    void recvAppMap(QVariantMap map);

public:
    QSerialPort *com;
    QVariantMap tmp;
};

#endif // DEVBARCONDE_H
