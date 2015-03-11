#include "googlemap.h"
#include <QFile>
#include <QByteArray>
#include <QWebFrame>
#include <iostream>



GoogleMap::GoogleMap(QWidget *parent):
    QWebView (parent),
    page(parent)
{
    frame_page = 0;
}

void GoogleMap::runMap(DataCollector& information_map){
    QFile data(":pageWeb/html/mapaGoogleEarth.html");
    if (data.open(QIODevice::ReadOnly)) {
        QByteArray dump = data.readAll();
        frame_page = page.mainFrame();
        page.mainFrame()->setHtml(dump);
        page.mainFrame()->addToJavaScriptWindowObject("information_map", &information_map);
        page.mainFrame()->addToJavaScriptWindowObject("data_map", this);
        this->setPage(&page);
        data.close();
    }
}


void GoogleMap::drawPolylines(){
    //std::vector<glm::vec3> array_data;

  //  frame->addToJavaScriptWindowObject("arrayData", array_data);
}


void GoogleMap::drawGoogleEarth(std::vector<glm::vec3> values, bool next){
    std::cout << "dibujando en google maps" << std::endl;
}
