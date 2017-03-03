#include <QtGui>

#include "mainwindow.h"
#include "QKit.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Sentences Square"));
    setMinimumSize(900,650);
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current form letter"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    InitStackedWidget();
    //textEdit = new QTextEdit;
    setCentralWidget(stackedWidget);

    QDockWidget *dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setMaximumWidth(220);
    dock->setMinimumWidth(220);
    listWidget = new QListWidget(dock);
    InitListWidget();
    dock->setWidget(listWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::print()
{

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Dock Widgets"),
        tr("The <b>Dock Widgets</b> example demonstrates how to "
        "use Qt's dock widgets. You can enter your own text, "
        "click a customer to add a customer name and "
        "address, and click standard paragraphs to add them."));
}

void MainWindow::InitListWidget()
{
    listWidget->setStyleSheet(QKit::loadStyleSheet("customerList"));
    listWidget->setCurrentRow(0);

    QListWidgetItem *webButton = new QListWidgetItem(listWidget);
    webButton->setText(tr("www.youdao.com"));
    //configButton->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::AlignAbsolute);
    webButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *configButton = new QListWidgetItem(listWidget);
    configButton->setText(tr("Sentence"));
    //configButton->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::AlignAbsolute);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(listWidget);
    updateButton->setText(tr("VOA"));
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *queryButton = new QListWidgetItem(listWidget);
    queryButton->setText(tr("Asterisk"));
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(listWidget,
        SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
        this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void MainWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    int x = listWidget->row(current);
    stackedWidget->setCurrentIndex(x);
}

void MainWindow::InitStackedWidget()
{
    stackedWidget = new QStackedWidget;

    webWidget = new WebWidget();
    stackedWidget->addWidget(webWidget);
    connect(webWidget,SIGNAL(SaveSend(QVariantMap)),
        this,SLOT(save(QVariantMap)));

    sentenceWidget = new SentenceWidget();
    stackedWidget->addWidget(sentenceWidget);

/*
    QTextEdit *textEdit2 = new QTextEdit;
    textEdit2->setText("222222222222222");
    QTextEdit *textEdit3 = new QTextEdit;
    textEdit3->setText("3333333333333333");
    stackedWidget->addWidget(textEdit2);
    stackedWidget->addWidget(textEdit3);*/
}

void MainWindow::save(QVariantMap vm)
{
    QString data_rel = vm["data_rel"].toString();
    QString en_data = vm["en_data"].toString();
    QString zh_data = vm["zh_data"].toString();
    QString example_text = vm["example_text"].toString();
    QString example_href = vm["example_href"].toString();
    Sentence st(en_data,zh_data,data_rel,"",example_href,example_text);
    if (st.isExist())
    {
        QMessageBox::warning(this,tr("Warning"),tr("This sentence already exists."));
    }
    else if (sentenceWidget->insertWidgetItem(st))
    {
        QMessageBox::information(this,tr("Tip"),tr("Save sentencs successfully."));
    }else
    {
        QMessageBox::warning(this,tr("Warning"),tr("Fail to save sentence."));
    }
}