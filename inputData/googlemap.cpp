#include "googlemap.h"
#include <QFile>
#include <QByteArray>
#include <QWebFrame>
#include <iostream>
#include "utmconverter.h"



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

void printArrayPosition(std::vector<glm::vec3> values){
    for(int i= 0; i < values.size(); ++i){
        std::cout << " pto " << values[i].x << " " << values[i].y << " " << values[i].z << " end" << std::endl;
    }
}
void GoogleMap::drawGoogleEarth(std::vector<glm::vec3> values, bool next){
    std::cout << "Drawing in Google Earth" << std::endl;
    this->convertCoordinatesToLongLat(values);
    const char* ptr = (const char*)values.data();
    QByteArray lines_array(ptr, sizeof(glm::vec3) * values.size());
    QObject ob;
    ob.setProperty("lines_array", lines_array);
    page.mainFrame()->addToJavaScriptWindowObject("positions", (QObject*)&ob);
    emit somethingChanged(next);
}

void GoogleMap::convertCoordinatesToLongLat(std::vector<glm::vec3>& values){
    for (int i=0; i< values.size(); ++i){
        values[i] = UTMConverter::convertToLatLong(values[i]);
        std::cout << " coord " << values[i].x << " " << values[i].y << " " << values[i].z << std::endl;
    }
}



