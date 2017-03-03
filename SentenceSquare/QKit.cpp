#include "QKit.h"

#include <QFile>
#include <QDir>
#include <phonon/MediaObject>
#include <QUrl>
#include <QTextCodec>

QKit::QKit(void)
{
}

QKit::~QKit(void)
{
}

QString QKit::loadStyleSheet(QString sheetName)
{
    QString styleSheet = "";
    QFile file(":/qss/" + sheetName + ".qss");
    if( file.open(QFile::ReadOnly) ){
        styleSheet = QLatin1String(file.readAll());
        file.close();
    }
    return styleSheet;
}

QString QKit::loadFileContent(QString filename)
{
    QFile file;
    file.setFileName(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextCodec *code=QTextCodec::codecForName("utf8");
        QTextStream out(&file);
        out.setCodec(code);
        QString jQuery = out.readAll();
        file.close();
        return jQuery;
    }
    return "";
}

void QKit::announceSentence(QString rel)
{
    QUrl url("http://dict.youdao.com/dictvoice?audio="+rel);
    Phonon::MediaObject *music =
        Phonon::createPlayer(Phonon::MusicCategory,
        Phonon::MediaSource(url));
    music->play();
}

QString QKit::getAbsolutePath(QString relativePath)
{
    QDir temDir(relativePath);
    return temDir.absolutePath();
}