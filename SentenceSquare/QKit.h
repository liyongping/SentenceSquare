#ifndef QKIT_H
#define QKIT_H

#include <QString>

class QKit
{
public:
    QKit(void);
    virtual ~QKit(void);
public:
    static QString loadStyleSheet(QString sheetName);
    static QString loadFileContent(QString filename);
    static void announceSentence(QString rel);
    static QString getAbsolutePath(QString relativePath);
};

#endif // QKIT_H