#include "nonedrainagealgorithm.h"

NoneDrainageAlgorithm::NoneDrainageAlgorithm():
    DrainageAlgorithms()
{
}


NoneDrainageAlgorithm::~NoneDrainageAlgorithm(){

}

void NoneDrainageAlgorithm::run(Terrain *ter){

}

void NoneDrainageAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){

}

void NoneDrainageAlgorithm::glewReady(){

}

QString NoneDrainageAlgorithm::getName(){
    return QString("Select Algorithm");
}

QWidget* NoneDrainageAlgorithm::getConf(){
    return &conf;
}

