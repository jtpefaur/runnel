#include "googlemap.h"
#include <QFile>
#include <QByteArray>
#include <QWebFrame>
#include <iostream>



GoogleMap::GoogleMap(QWidget *parent):
    QWebView (parent),
    page(parent)
{

}

void GoogleMap::runMap(DataCollector& information_map){
    QFile data(":pageWeb/html/mapaGoogleEarth.html");
    if (data.open(QIODevice::ReadOnly)) {
        QByteArray dump = data.readAll();
        page.mainFrame()->setHtml(dump);
        page.mainFrame()->addToJavaScriptWindowObject("information_map", &information_map);
        this->setPage(&page);
        data.close();
    }
}
