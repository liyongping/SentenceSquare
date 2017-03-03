#include "mainwindow.h"
#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }

    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(mainwindow);
    
    qInstallMsgHandler(customMessageHandler);

    QApplication a(argc, argv);
    
    MainWindow w;
    w.show();
    return a.exec();
}
