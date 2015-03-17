#ifndef GOOGLEMAP_H
#define GOOGLEMAP_H

#include <QWebView>
#include "webpage.h"
#include "datacollector.h"


class GoogleMap : public QWebView {
    Q_OBJECT

    public:
        GoogleMap(QWidget *parent = 0);
        void runMap(DataCollector& information_map);

    private:
        WebPage page;
        QWebFrame *frame_page;
        void convertCoordinatesToLongLat(std::vector<glm::vec3>& values);

    public slots:
        void drawGoogleEarth(std::vector<glm::vec3> values, bool next);

    signals:
        void somethingChanged(bool next);

};

#endif // GOOGLEMAP_H
