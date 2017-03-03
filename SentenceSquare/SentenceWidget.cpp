#include "SentenceWidget.h"
#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>

#include "QKit.h"

SentenceWidget::SentenceWidget(QWidget *parent)
    : QWidget(parent),
      table(NULL)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sqlite/data.db");
    //db.setDatabaseName(":memory:");

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
        this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    Phonon::createPath(mediaObject, audioOutput);
	
    initActions();
    initToolBar();
    initTableWidget();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(toolBarLayout);
    mainLayout->addWidget(table);
    setLayout(mainLayout);
}

SentenceWidget::~SentenceWidget()
{
    if (db.isOpen())
        db.close();
}

void SentenceWidget::initToolBar()
{
    QToolBar *bar = new QToolBar;
    bar->addAction(previousAction);
    bar->addAction(playAction);
    bar->addAction(pauseAction);
    bar->addAction(stopAction);
    bar->addAction(nextAction);

    toolBarLayout = new QHBoxLayout;
    toolBarLayout->addStretch();
    toolBarLayout->addWidget(bar);
    //toolBarLayout->addWidget(playButton);
    //toolBarLayout->addWidget(pauseButton);
    //toolBarLayout->addWidget(stopButton);
    toolBarLayout->addStretch();
}

void SentenceWidget::initActions()
{
    QSize iconSize(32, 32);
    playAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
    playAction->setShortcut(tr("Ctrl+P"));
    playAction->setDisabled(true);
    pauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
    pauseAction->setShortcut(tr("Ctrl+A"));
    pauseAction->setDisabled(true);
    stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
    stopAction->setShortcut(tr("Ctrl+S"));
    stopAction->setDisabled(true);
    nextAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipForward), tr("Next"), this);
    nextAction->setShortcut(tr("Ctrl+N"));
    previousAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipBackward), tr("Previous"), this);
    previousAction->setShortcut(tr("Ctrl+R"));

    connect(playAction, SIGNAL(triggered()), mediaObject, SLOT(play()));
    connect(pauseAction, SIGNAL(triggered()), mediaObject, SLOT(pause()));
    connect(stopAction, SIGNAL(triggered()), mediaObject, SLOT(stop()));
}

void SentenceWidget::initTableWidget()
{
    initSentenceList();

    table = new QTableWidget(sentenceList.size(),3);
    checkedHeader = new CheckedHeader(Qt::Horizontal);
    table->setHorizontalHeader(checkedHeader);
    table->setHorizontalHeaderLabels(QStringList()<<tr("")<<tr("")<<tr("Name"));
    table->horizontalHeader()->resizeSection(CHECK_COLUMN,22);
    table->horizontalHeader()->resizeSection(PLAY_COLUMN,24);
    //table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->horizontalHeader()->setStretchLastSection(true);

    table->verticalHeader()->setVisible(true);
    //table->verticalHeader()->setStyleSheet(QKit::loadStyleSheet("headview"));
    // don't allow edit cell
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    // select complete row
    //table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);

    //table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    soundPixmap = QPixmap::fromImage(QImage(":/img/sound_small.gif"));
    
    connect(table, SIGNAL(cellClicked(int, int)), this, SLOT(tableCellClicked(int, int)));
    connect(table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableCellDoubleClicked(int, int)));
    connect(table, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(tableItemClicked(QTableWidgetItem *)));
    connect(table->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(lineClicked(int)));
    connect(table->horizontalHeader(), SIGNAL(toggled(bool)), this, SLOT(clickCheckAllBtn(bool)));

    QTime myTimer;
    myTimer.start();
    for(int i=0; i!=sentenceList.size();i++){
        setWidgetRow(i,sentenceList.at(i));
        QUrl url("http://dict.youdao.com/dictvoice?audio="+sentenceList.at(i).dataRel);
        Phonon::MediaSource source(url);
        sources.append(source);
    }
    qDebug()<<"setWidgetRow time:"<<QString::number(myTimer.elapsed())<<"ms";
}

void SentenceWidget::initSentenceList()
{
    QTime myTimer;
    myTimer.start();
    if (db.open()) {
        QSqlQuery query;
        query.exec("select * from sentences");
        while(query.next()){
            sentenceList.append(Sentence(query.value(0).toInt(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toString(),
                query.value(6).toString()));
        }
        int si = sentenceList.size();
    }else{
        QMessageBox::warning(0, QObject::tr("Open Database Error"),db.lastError().text());
    }
    qDebug()<<"initSentenceList time:"<<QString::number(myTimer.elapsed())<<"ms";
}

bool SentenceWidget::insertWidgetItem(Sentence &st)
{
    if (db.open()){
        QSqlQuery query = st.getInsertQuery();
        if(query.exec()){
            st.setId(query.lastInsertId().toInt());
            int currentRow = table->rowCount();
            table->insertRow(currentRow);
            setWidgetRow(currentRow, st);
            return true;
        }
    }
    return false;
}

void SentenceWidget::setWidgetRow(int row, Sentence st)
{
    QTableWidgetItem *enDataItem = new QTableWidgetItem(st.enData);
    QString tip = st.enData+"\r\n"+st.zhData;
    enDataItem->setToolTip(tip);
    QTableWidgetItem *playItem = new QTableWidgetItem();
    playItem->setData(Qt::DecorationRole,soundPixmap );

    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setCheckState(Qt::Unchecked);

    table->setItem(row, CHECK_COLUMN, checkItem);
    table->setItem(row, PLAY_COLUMN, playItem);
    table->setItem(row, SENTENCE_COLUMN, enDataItem);
}


Qt::CheckState SentenceWidget::getTableWidgetState(QTableWidget *table,int column)
{
    int rows = table->rowCount();
    bool findChecked = false;
    bool findUnChecked = false;
    for (int row=0; row< rows; row++){
        QTableWidgetItem *item = table->item(row,column);
        if (item){
            if (item->checkState() == Qt::Checked)
                findChecked = true;
            else
                findUnChecked = true;
            if(findChecked && findUnChecked)
                return Qt::PartiallyChecked;
        }
    }
    if (findChecked && !findUnChecked)
        return Qt::Checked;
    else
        return Qt::Unchecked;
}

void SentenceWidget::tableCellClicked(int row, int column)
{
/*
    QTableWidgetItem *item = table->item(row, CHECK_CLOUMN);
    Qt::CheckState state = item->checkState();
    if (state == Qt::Unchecked)
        item->setCheckState(Qt::Checked);
    else if (state == Qt::Checked)
        item->setCheckState(Qt::Unchecked);*/
    mediaObject->setCurrentSource(sources[row]);
}

void SentenceWidget::tableCellDoubleClicked(int row, int column)
{
    bool wasPlaying = mediaObject->state() == Phonon::PlayingState;

    mediaObject->stop();
    mediaObject->clearQueue();

    if (row >= sources.size())
        return;

    mediaObject->setCurrentSource(sources[row]);

    if (!wasPlaying)
        mediaObject->play();
    else
        mediaObject->stop();

    //QTableWidgetItem *item = table->item(row,column);
    //QKit::announceSentence(sentenceList.at(row).dataRel);
}

void SentenceWidget::lineClicked(int row)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item = table->item(row,0);
}

void SentenceWidget::tableItemClicked(QTableWidgetItem * item)
{
    QTableWidgetItem *item2 = item;
}

void SentenceWidget::clickCheckAllBtn(bool checked)
{
    int rows = table->rowCount();
    QTableWidgetItem *item = NULL;
    for (int r=0; r< rows; r++){
        item = table->item(r,CHECK_COLUMN);
        if (item){
            item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
        }
    }
}

void SentenceWidget::stateChanged(Phonon::State newState, Phonon::State oldState)
{
    switch (newState) {
        case Phonon::ErrorState:
            if (mediaObject->errorType() == Phonon::FatalError) {
                QMessageBox::warning(this, tr("Fatal Error"),
                    mediaObject->errorString());
            } else {
                QMessageBox::warning(this, tr("Error"),
                    mediaObject->errorString());
            }
            break;
        case Phonon::PlayingState:
            playAction->setEnabled(false);
            pauseAction->setEnabled(true);
            stopAction->setEnabled(true);
            break;
        case Phonon::StoppedState:
            stopAction->setEnabled(false);
            playAction->setEnabled(true);
            pauseAction->setEnabled(false);
            break;
        case Phonon::PausedState:
            pauseAction->setEnabled(false);
            stopAction->setEnabled(true);
            playAction->setEnabled(true);
        case Phonon::BufferingState:
            break;
        default:
            ;
    }
}

void SentenceWidget::aboutToFinish()
{
/*
    int index = sources.indexOf(mediaObject->currentSource()) + 1;
    if (sources.size() > index) {
        mediaObject->enqueue(sources.at(index));
    }*/
}