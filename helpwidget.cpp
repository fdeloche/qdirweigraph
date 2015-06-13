#include "helpwidget.h"

HelpWidget::HelpWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("Help");
    webView = new QWebView(this);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void HelpWidget::loadPage(){
    webView->load(QUrl("qrc:///help/help.htm"));
}
