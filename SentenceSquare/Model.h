#ifndef MODEL_H
#define MODEL_H

#include <QSqlQuery>

class Sentence{
public:
    int id;
    QString word;
    QString enData;
    QString zhData;
    QString dataRel;
    QString exampleHref;
    QString exampleText;

    Sentence(int id, QString enData, QString zhData, QString dataRel, 
        QString word, QString exampleHref = "", QString exampleText = "")
    {
        this->id = id;
        this->word = word;
        this->enData = enData;
        this->zhData = zhData;
        this->dataRel = dataRel;
        this->exampleHref = exampleHref;
        this->exampleText = exampleText;
    }
    Sentence(QString enData, QString zhData, QString dataRel, 
        QString word, QString exampleHref = "", QString exampleText = "")
    {
        this->word = word;
        this->enData = enData;
        this->zhData = zhData;
        this->dataRel = dataRel;
        this->exampleHref = exampleHref;
        this->exampleText = exampleText;
    }

    QSqlQuery getInsertQuery()
    {
        QSqlQuery query;
        query.prepare("INSERT INTO sentences(enData,zhData,dataRel,word,exampleHref,exampleText) "
            "VALUES(:enData,:zhData,:dataRel,:word,:exampleHref,:exampleText)");
        query.bindValue(":enData", enData);
        query.bindValue(":zhData", zhData);
        query.bindValue(":dataRel", dataRel);
        query.bindValue(":word", word);
        query.bindValue(":exampleHref", exampleHref);
        query.bindValue(":exampleText", exampleText);
        return query;
    }

    bool isExist()
    {
        QSqlQuery query;
        query.prepare("SELECT * from sentences where enData=:endata");
        query.bindValue(":endata", enData);
        if (query.exec())
        {
            if (query.next())
                return true;
        }
        return false;
    }

    void setId(int id)
    {
        this->id = id;
    }
};





#endif