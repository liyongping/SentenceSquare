#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "SentenceWidget.h"
#include "webwidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QListWidget;
class QListWidgetItem;
class QTextEdit;
class QStackedWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

private slots:
    void print();
    void about();
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void save(QVariantMap vm);
private:
    Ui::MainWindowClass ui;

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    QTextEdit *textEdit;

    QListWidget *listWidget;
    void InitListWidget();
    QStackedWidget *stackedWidget;
    WebWidget *webWidget;
    SentenceWidget *sentenceWidget;
    void InitStackedWidget();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction *printAct;
    QAction *aboutAct;
    QAction *quitAct;
    
};

#endif // MAINWINDOW_H
