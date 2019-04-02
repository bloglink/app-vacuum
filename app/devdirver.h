#ifndef DEVDIRVER_H
#define DEVDIRVER_H

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QSerialPort>

class DevDirver : public QObject
{
    Q_OBJECT
public:
    explicit DevDirver(QObject *parent = 0);

signals:

public slots:
    void open(QString name);
    void quit();
    void turn();
private:
    QSerialPort *com;
    QVariantMap tmp;
};

#endif // DEVDIRVER_H
