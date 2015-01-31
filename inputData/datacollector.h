#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QObject>
#include "terrain.h"

class DataCollector : public QObject
{
    Q_OBJECT

    public:
        int number_points;
        Terrain* ter_data;
        int counter;
        int alt;
        int anch;
        int ancho_pix;
        int largo_pix;
        DataCollector();

        virtual ~DataCollector();
        glm::vec3 calculateUTM(float lati, float longit, float height);
    public slots:
        Q_INVOKABLE
        void getCoord(QByteArray list_coord );
        void setCantPoints(int num, int anch, int alt, int ancho_pixel, int height_pixel);
        void finishInformation();
        void getTerrainStruct(Terrain* ter);
        float conversionToUTM(float number);

    signals:
        void changeTerrain();
        void finishTerrain();

};

#endif // DATACOLLECTOR_H

