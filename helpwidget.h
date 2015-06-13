#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QWebView>

class HelpWidget : public QWidget
{
    Q_OBJECT
private:
    QWebView * webView;
public:
    explicit HelpWidget(QWidget *parent = 0);
    void loadPage();

signals:

public slots:

};

#endif // HELPWIDGET_H
