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
    gm.runMap(information_map);
    std::cout << "Start Runnel Controller..." << std::endl;
}

GoogleMap& RunnelController::getGoogleMap(){
    return gm;
}

std::unordered_map<std::string, glm::vec3> RunnelController::getColorConf(){
    return pterrain.color_conf;
}

void RunnelController::changeColors(std::string name, glm::vec3 value){
    pterrain.changeAttrConf(name, value);
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
    }

    std::cout << "Finish Obtain Data..." << std::endl;
    this->buildTerrain();
}

void RunnelController::buildTerrain(){
    BuilderTerrain bt;
    bt.runTriangulation(ter);
    std::cout << "Finish Triangulation..." << std::endl;
    ter->normalize();
    pterrain.initGL();
    pterrain.setTerrain(ter);

}

void RunnelController::changeSelectDrainage(QString name){
    std::string name_selected = name.toStdString();
//    switch (name_selected) {
//        case 'Peucker':
//          // Code
//          break;
//        case 'Callaghan':
//          // Code
//          break;
//        default:
//          break;
//   }

}


void RunnelController::changeSelectPatron(QString name){
    std::string name_selected = name.toStdString();
//    switch (name_selected) {
//        case "ZhangGuilbert":
//          // Code
//          break;
//        default:
//          break;
//   }
}



void RunnelController::changeSelectWater(QString name){
    std::string name_selected = name.toStdString();
//    switch (name_selected) {
//        case "Peucker":
//          // Code
//          break;
//        case "Callaghan":
//          // Code
//          break;
//        case "Gradient":
//          // Code
//          break;
//        default:
//          break;
//   }
}
