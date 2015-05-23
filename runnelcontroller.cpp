#include "runnelcontroller.h"
#include <iostream>


#include "builders/builderterrain.h"
#include "painters/painterterrain.h"
#include "inputData/tiffdata.h"
#include "inputData/runneldata.h"

RunnelController::RunnelController():
    gm(),
    pterrain(),
    information_map()
{
    QObject::connect(&information_map, SIGNAL(changeTerrain()), this, SLOT(getTerrain()));
    QObject::connect(this, SIGNAL(setTerrainDataCollector(Terrain*)), &information_map, SLOT(getTerrainStruct(Terrain*)));
    QObject::connect(&information_map, SIGNAL(finishTerrain()), this, SLOT(getObtainTerrain()));
    QObject::connect(&pterrain, SIGNAL(glewIsReady()), this, SIGNAL(glewIsReady()));
    QObject::connect(&pterrain, SIGNAL(drawGoogleEarth(std::vector<glm::vec3>)), &gm, SLOT(drawGoogleEarth(std::vector<glm::vec3>)));
    QObject::connect(this, SIGNAL(drawGoogleEarth(std::vector<glm::vec3>)), &gm, SLOT(drawGoogleEarth(std::vector<glm::vec3>)));
    gm.runMap(information_map);
    coords = glm::vec3(0,0,0);
    std::cout << "Start Runnel Controller..." << std::endl;

}

GoogleMap& RunnelController::getGoogleMap(){
    return gm;
}

std::unordered_map<std::string, glm::vec3> RunnelController::getColorConf(){
    return pterrain.color_conf;
}

void RunnelController::changeColors(std::string name, glm::vec3 value){
}

PainterTerrain& RunnelController::getPainter(){
    return pterrain;
}

void RunnelController::getTerrain(){
    ter = new Terrain();

    emit setTerrainDataCollector(ter);
}


void RunnelController::getObtainTerrain(){
   this->buildTerrain();
}

void RunnelController::obtainFileTerrain(QString name_file, std::string file_type){
    ter = new Terrain();
    std::cout << file_type << std::endl;
    if(file_type == "TIFF"){
        std::cout << "entro a "  << std::endl;
        TiffData tif;
        tif.openFile(name_file);
        tif.getDataTerrain(ter);
    }
    if(file_type == "RUNNEL"){
        RunnelData rundata;
        rundata.openFile(name_file);
        rundata.getDataTerrain(ter);
        ter->signalPaintGoogle = true;
    }

    std::cout << "Finish Obtain Data..." << std::endl;

    this->buildTerrain();
}

void RunnelController::saveData(QString fileName){
    RunnelData rundata;
    if(!rundata.writeFile(fileName, ter)){
        std::cout << "Could not write to file" << std::endl;
    }
}

void RunnelController::buildTerrain(){
    BuilderTerrain bt;
    bt.runTriangulation(ter);
    std::cout << "Finish Triangulation..." << std::endl;
    ter->normalize();

}

void RunnelController::changeSelectDrainage(DrainageAlgorithms* alg){
    alg->run(ter);
    if ( this->ter->signalPaintGoogle ){
        emit drawGoogleEarth(alg->getPathTree());
    }
}

void RunnelController::changeSelectPatron(AlgorithmPatron* alg){
    if (drainage_network.size() > 0){
        alg->run(ter, drainage_network);
        if ( this->ter->signalPaintGoogle ){
            emit drawGoogleEarth(alg->getPathTree());
        }
    }else{
        std::cout << "Network drainage dont have tree" << std::endl;
    }
}

void RunnelController::changeSelectWater(PathWaterAlgorithm* alg){
    alg->run(coords, ter);
}

void RunnelController::changeSelectNetwork(BuildNetwork* alg){
    drainage_network = alg->run(ter);
    if ( this->ter->signalPaintGoogle ){
        emit drawGoogleEarth(alg->getPathTree());
    }
}

void RunnelController::changeSelectFluvialTerrace(FluvialTerraceAlgorithm* alg){
    alg->run(coords, ter);
}
