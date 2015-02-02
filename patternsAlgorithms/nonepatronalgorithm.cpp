#include "nonepatronalgorithm.h"

NonePatronAlgorithm::NonePatronAlgorithm():
    AlgorithmPatron()
{
}

NonePatronAlgorithm::~NonePatronAlgorithm(){

}

void NonePatronAlgorithm::run(Terrain *, std::vector<arbol*> &){

}

void NonePatronAlgorithm::render(glm::mat4, float){
}

void NonePatronAlgorithm::glewReady(){

}
QString NonePatronAlgorithm::getName(){
    return QString("Select Algorithm");
}
QWidget* NonePatronAlgorithm::getConf(){
    return 0;
}
