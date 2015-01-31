#ifndef RUNNELCONTROLLER_H
#define RUNNELCONTROLLER_H

#include <vector>
#include "inputData/googlemap.h"
#include "painters/painterterrain.h"
#include "terrain.h"
#include <drainageAlgorithms/drainagealgorithms.h>

class RunnelController: public QObject
{
    Q_OBJECT

    public:

        GoogleMap gm;
        PainterTerrain pterrain;
        DataCollector information_map;
        Terrain* ter;

        void buildTerrain();
        GoogleMap& getGoogleMap();
        PainterTerrain& getPainter();
        std::unordered_map<std::string, glm::vec3> getColorConf();
        void changeColors(std::string name, glm::vec3 value);
        RunnelController();
        void obtainFileTerrain(QString name_file, std::string file_type);




    public slots:
        void getTerrain();
        void getObtainTerrain();
        void changeSelectDrainage(DrainageAlgorithms* alg);
        void changeSelectPatron(QString name);
        void changeSelectWater(QString name);


    signals:
        void setTerrainDataCollector(Terrain* ter);
        void glewIsReady();

};

#endif // RUNNELCONTROLLER_H

