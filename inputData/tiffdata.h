#ifndef TIFFDATA_H
#define TIFFDATA_H

#include <QImage>
#include "terrain.h"
#include "QString"

class TiffData
{
    public:

        TiffData();
        bool openFile(QString name);
        void getDataTerrain(Terrain* ter);
        QImage image;
};

#endif // TIFFDATA_H
