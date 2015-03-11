#include "nonebuildnetwork.h"

NoneBuildNetwork::NoneBuildNetwork():
    BuildNetwork()
{

}

NoneBuildNetwork::~NoneBuildNetwork(){

}

std::vector<arbol*> NoneBuildNetwork::run(Terrain*){
    std::vector<arbol*> empty;
    return empty;

}

std::vector<arbol*> NoneBuildNetwork::getData(){
    std::vector<arbol*> empty;
    return empty;
}

void NoneBuildNetwork::render(glm::mat4, float){

}

void NoneBuildNetwork::glewReady(){

}

QString NoneBuildNetwork::getName(){
    return QString("Select Algorithm");
}

QWidget* NoneBuildNetwork::getConf(){
    return 0;
}
