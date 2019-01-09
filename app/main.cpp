/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪
*******************************************************************************/
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QFile>

#include "main.h"
#include "appwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qRegisterMetaType<QTmpMap> ("QTmpMap");

    QFile file;
    QString qss;
    file.setFileName("./qss/darkBlack.qss");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    QTranslator qtTran;
    qtTran.load("./qrc/qt_zh_CN.qm");
    a.installTranslator(&qtTran);

    QTranslator qtBase;
    qtBase.load("./qrc/qtbase_zh_CN.qm");
    a.installTranslator(&qtBase);

    AppWindow w;
#ifdef __linux__
    w.resize(1440, 900);
    w.show();
#else
    w.showFullScreen();
#endif

    return a.exec();
}
