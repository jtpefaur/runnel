#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>

class WebPage : public QWebPage
{
    Q_OBJECT
protected:
    virtual QString userAgentForUrl(const QUrl &url) const
    {
        return QString::fromUtf8("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:2.0b8) Gecko/20100101 Firefox/4.0b8");
    }

public:
    WebPage(QObject *parent = 0): QWebPage(parent) {}
    virtual ~WebPage(){}
};

#endif // WEBPAGE_H


