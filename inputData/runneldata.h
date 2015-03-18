#ifndef RUNNELDATA_H
#define RUNNELDATA_H

#include "terrain.h"
#include "QString"

class RunnelData
{
    public:
        RunnelData();
        bool openFile(QString name);
        void getDataTerrain(Terrain* ter);
        bool writeFile(QString fileName, Terrain* ter);
        ~RunnelData();
};

#endif // RUNNELDATA_H
