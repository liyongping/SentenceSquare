#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QLineEdit>
#include <QWebView>

class WebWidget : public QWidget
{
    Q_OBJECT

public:
    WebWidget(QWidget *parent=0);
    ~WebWidget();

protected slots:
    void adjustLocation();
    void changeLocation();
    void setProgress(int p);
    void finishLoading(bool ok);
    void addJsWindowObject();
    void save(QVariantMap vm);
protected:
    void showEvent(QShowEvent * event);
signals:
    void SaveSend(QVariantMap vm);

private:
    QWebView *view;
    QLineEdit *locationEdit;
    QToolButton *searchButton;
    QUrl baseUrl;
    QString templatePath;
    int progress;
};

#endif // WEBWIDGET_H
