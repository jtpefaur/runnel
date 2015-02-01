#include "nonebuildnetwork.h"

NoneBuildNetwork::NoneBuildNetwork():
    BuildNetwork()
{

}

NoneBuildNetwork::~NoneBuildNetwork(){

}

void NoneBuildNetwork::run(Terrain*){

}

void NoneBuildNetwork::render(glm::mat4, float, glm::vec3){

}

void NoneBuildNetwork::glewReady(){

}

QString NoneBuildNetwork::getName(){
    return QString("Select Algorithm");
}

QWidget* NoneBuildNetwork::getConf(){
    return 0;
}
