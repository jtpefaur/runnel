#ifndef RUNNELDATA_H
#define RUNNELDATA_H

#include "terrain.h"
#include "QString"
#include "QFile"

class RunnelData
{
    public:
        RunnelData();
        bool openFile(QString name);
        void getDataTerrain(Terrain* ter);
        bool writeFile(QString fileName, Terrain* ter);
        ~RunnelData();

    private:
        QFile file;
};

#endif // RUNNELDATA_H
