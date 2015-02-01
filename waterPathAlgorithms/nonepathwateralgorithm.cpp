#include "nonepathwateralgorithm.h"

NonePathWaterAlgorithm::NonePathWaterAlgorithm():
    PathWaterAlgorithm()
{
}

NonePathWaterAlgorithm::~NonePathWaterAlgorithm(){

}

void NonePathWaterAlgorithm::run(glm::vec3, Terrain *){

}

void NonePathWaterAlgorithm::render(glm::mat4, float, glm::vec3){

}

void NonePathWaterAlgorithm::glewReady(){

}

QString NonePathWaterAlgorithm::getName(){
    return QString("Select Algorithm");
}

QWidget* NonePathWaterAlgorithm::getConf(){
    return 0;
}
