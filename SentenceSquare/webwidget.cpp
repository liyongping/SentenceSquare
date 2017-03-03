#include "webwidget.h"

#include <QtNetwork>
#include <QtWebKit>
#include <QWebPage>
#include "QKit.h"

WebWidget::WebWidget(QWidget *parent)
    : QWidget(parent)
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    templatePath = QKit::getAbsolutePath("./template/queryresult.html");
    baseUrl = QUrl::fromLocalFile(templatePath);

    view = new QWebView();
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    connect(view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
        this, SLOT(addJsWindowObject()));

    // if we want to improve the style, we cann refer to "searchbox"
    locationEdit = new QLineEdit();
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().horizontalPolicy());
    locationEdit->setFocus();
    QFont font;
    font = locationEdit->font();
    font.setPointSize(22);
    locationEdit->setFont(font);
    
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    searchButton = new QToolButton();
    searchButton->setText(tr("Search"));
    searchButton->setToolTip(tr("Search"));
    searchButton->setFixedHeight(46);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(changeLocation()));

    QToolBar *toolBar = new QToolBar;

    toolBar->addWidget(locationEdit);
    toolBar->addWidget(searchButton);
    toolBar->setFixedHeight(50);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(toolBar);
    layout->addWidget(view);
    setLayout(layout);
}

WebWidget::~WebWidget()
{

}

void WebWidget::showEvent(QShowEvent * event)
{
    locationEdit->setFocus();
}

void WebWidget::changeLocation()
{
    QString keyword = locationEdit->text();
    QString tmp = QKit::loadFileContent(templatePath);
    view->setHtml(tmp, baseUrl);
    //QUrl url = QUrl(locationEdit->text());
    //view->load(url);
    //view->setFocus();
}

void WebWidget::adjustLocation()
{
    //locationEdit->setText(view->url().toString());
}


void WebWidget::setProgress(int p)
{
    progress = p;
}

void WebWidget::finishLoading(bool ok)
{
    progress = 100;
}

void WebWidget::addJsWindowObject()
{
    view->page()->mainFrame()->addToJavaScriptWindowObject("WebWidget", this);
}

void WebWidget::save(QVariantMap vm)
{
    emit SaveSend(vm);
}