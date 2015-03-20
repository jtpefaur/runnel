#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QObject>
#include "terrain.h"

class DataCollector : public QObject
{
    Q_OBJECT

    public:

        DataCollector();
        virtual ~DataCollector();

    private:
        int number_points;
        Terrain* ter_data;
        int counter;
        int alt;
        int anch;
        int ancho_pix;
        int largo_pix;

    public slots:
        Q_INVOKABLE
        void getCoord(QByteArray list_coord );
        void setCantPoints(int num, int anch, int alt, int ancho_pixel, int height_pixel, float lat0, float lng0);
        void finishInformation();
        void getTerrainStruct(Terrain* ter);

    signals:
        void changeTerrain();
        void finishTerrain();

};

#endif // DATACOLLECTOR_H

