#ifndef LIBEXPORT_H
#define LIBEXPORT_H

#include <QDir>
#include <QFile>
#include <QWidget>

class libExport : public QWidget
{
    Q_OBJECT
public:
    explicit libExport(QWidget *parent = 0);

signals:

public slots:
    void initExport();
    void fileExport(QString name);
};

#endif // LIBEXPORT_H
