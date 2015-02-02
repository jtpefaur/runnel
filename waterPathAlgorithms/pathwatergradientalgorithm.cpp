#include "pathwatergradientalgorithm.h"

PathWaterGradientAlgorithm::PathWaterGradientAlgorithm(){
    shader = 0;
}

PathWaterGradientAlgorithm::~PathWaterGradientAlgorithm(){
    if(shader){
        delete shader;
    }
}

void PathWaterGradientAlgorithm::run(glm::vec3 point, Terrain *terr){

}

void PathWaterGradientAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if(shader){

    }
}

void PathWaterGradientAlgorithm::glewReady(){
    shader = new ShaderGradient();
}

QString PathWaterGradientAlgorithm::getName(){
    return QString("Gradient");
}

QWidget* PathWaterGradientAlgorithm::getConf(){
    return 0;
}
