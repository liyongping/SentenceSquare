#ifndef SENTENCEWIDGET_H
#define SENTENCEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QToolButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/SeekSlider>
#include <phonon/VolumeSlider>

#include "Model.h"
#include "checkedheader.h"

enum CloumnDefine{
    CHECK_COLUMN = 0,
    PLAY_COLUMN = 1,
    SENTENCE_COLUMN=2
};


class SentenceWidget : public QWidget
{
    Q_OBJECT
public:
    SentenceWidget(QWidget *parent = 0);
    virtual ~SentenceWidget();
    bool insertWidgetItem(Sentence &st);

private slots:
    void tableCellClicked(int row, int column);
    void tableCellDoubleClicked(int row, int column);
    void lineClicked(int row);
    void tableItemClicked(QTableWidgetItem * item);

    void clickCheckAllBtn(bool checked);

    void stateChanged(Phonon::State newState, Phonon::State oldState);
    void aboutToFinish();

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *toolBarLayout;

    //Phonon::SeekSlider *seekSlider;
    Phonon::MediaObject *mediaObject;
    Phonon::MediaObject *metaInformationResolver;
    Phonon::AudioOutput *audioOutput;
    Phonon::VolumeSlider *volumeSlider;
    QList<Phonon::MediaSource> sources;

    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *nextAction;
    QAction *previousAction;


    QAction *selectAll;
    void initToolBar();

    QTableWidget *table;
    CheckedHeader *checkedHeader;
    QPixmap soundPixmap;
    void initTableWidget();
    void setWidgetRow(int row, Sentence st);
    Qt::CheckState getTableWidgetState(QTableWidget *table,int column);

    QSqlDatabase db;
    QList<Sentence> sentenceList;
    void initSentenceList();

    void initActions();
    
};

#endif