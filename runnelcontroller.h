#ifndef RUNNELCONTROLLER_H
#define RUNNELCONTROLLER_H

#include <vector>
#include "inputData/googlemap.h"
#include "painters/painterterrain.h"
#include "terrain.h"
#include "drainageAlgorithms/drainagealgorithms.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"
#include "patternsAlgorithms/algorithmpatron.h"
#include "buildNetwork/buildnetwork.h"

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
        glm::vec3 coords;


    private:
        std::vector<arbol*> drainage_network;

    public slots:
        void getTerrain();
        void getObtainTerrain();
        void changeSelectDrainage(DrainageAlgorithms* alg);
        void changeSelectPatron(AlgorithmPatron* alg);
        void changeSelectWater(PathWaterAlgorithm* alg);
        void changeSelectNetwork(BuildNetwork* alg);


    signals:
        void setTerrainDataCollector(Terrain* ter);
        void drawGoogleEarth(std::vector<glm::vec3>, bool);
        void glewIsReady();


};

#endif // RUNNELCONTROLLER_H

