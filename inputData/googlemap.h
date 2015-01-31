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

};

#endif // GOOGLEMAP_H
