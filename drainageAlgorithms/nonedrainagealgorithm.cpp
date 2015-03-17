#include "nonedrainagealgorithm.h"

NoneDrainageAlgorithm::NoneDrainageAlgorithm():
    DrainageAlgorithms()
{
}


NoneDrainageAlgorithm::~NoneDrainageAlgorithm(){

}

void NoneDrainageAlgorithm::run(Terrain *){

}

void NoneDrainageAlgorithm::render(glm::mat4, float, glm::vec3){

}

void NoneDrainageAlgorithm::glewReady(){

}

QString NoneDrainageAlgorithm::getName(){
    return QString("Select Algorithm");
}

QWidget* NoneDrainageAlgorithm::getConf(){
    return 0;
}

std::vector<glm::vec3> NoneDrainageAlgorithm::getPathTree(){
    std::vector<glm::vec3> path;
    return path;
}
