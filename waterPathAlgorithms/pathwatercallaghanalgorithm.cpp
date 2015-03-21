#include "pathwatercallaghanalgorithm.h"
#include <iostream>

PathWaterCallaghanAlgorithm::PathWaterCallaghanAlgorithm():
    PathWaterAlgorithm()
{
}

PathWaterCallaghanAlgorithm::~PathWaterCallaghanAlgorithm(){

}

void PathWaterCallaghanAlgorithm::run(glm::vec3 coord, Terrain* ter){
    this->ter = ter;
    this->delta = conf.getDelta()/1000.0f;
    coords_asked = coord;
    runnel::Point* point_in_terrain = this->getPointClosed(coord);
    std::unordered_map<int, int> id_used;
    edge_water.clear();
    this->getPath(point_in_terrain, id_used);
    shader->fillPositionBuffer(edge_water);
}

void PathWaterCallaghanAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if(shader){
        shader->render(matrix, exag_z, color);
    }
}

void PathWaterCallaghanAlgorithm::glewReady(){
    shader = new ShaderGradient();
}

QString PathWaterCallaghanAlgorithm::getName(){
    return QString("Callaghan");
}

QWidget* PathWaterCallaghanAlgorithm::getConf(){
    return &conf;

}

void PathWaterCallaghanAlgorithm::getPath(runnel::Point* pto, std::unordered_map<int, int>& id_used){

    std::vector<int> position_neightbour = this->getPointsNeigthbour(pto);
    float max_z = -1;
    int id_max = -1;
    for (int id : position_neightbour){
        if(id >= 0 && id < ter->struct_point.size()){
            if( ter->struct_point[id]->coord.z <= (pto->coord.z + this->delta )){
                if(id_used[id] == 0){
                    float dist_z = (pto->coord.z + this->delta ) - ter->struct_point[id]->coord.z;
                    if( max_z < dist_z){
                        max_z = dist_z;
                        id_max = id;
                    }
                }
            }
        }
    }
    if(id_max != -1 && id_used[id_max] == 0){
        id_used[id_max] = 1;
        edge_water.push_back(pto->coord);
        edge_water.push_back(ter->struct_point[id_max]->coord);
        this->getPath(ter->struct_point[id_max], id_used);
    }else{
        std::cout << "valor del pto " << id_used[id_max] << " id del punto " << id_max << std::endl;
    }
}

std::vector<int> PathWaterCallaghanAlgorithm::getPointsNeigthbour(runnel::Point* pto){
    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - ter->width);
    position_neightbour.push_back(pto->ident + ter->width);
    if(pto->ident%ter->width > 0){
        position_neightbour.push_back(pto->ident + ter->width - 1);
        position_neightbour.push_back(pto->ident - ter->width - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%ter->width < ter->width-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - ter->width + 1);
        position_neightbour.push_back(pto->ident + ter->width + 1);
    }

    return position_neightbour;

}

runnel::Point* PathWaterCallaghanAlgorithm::getPointClosed(glm::vec3 coords){
    float minimum_distance = glm::distance(glm::vec2(ter->struct_point[0]->coord),glm::vec2(coords));
    runnel::Point* pto_minimum = ter->struct_point[0];
    for(runnel::Point*pto : ter->struct_point){
        float dist = glm::distance(glm::vec2(pto->coord), glm::vec2(coords));
        if(dist < minimum_distance){
            pto_minimum = pto;
            minimum_distance = dist;

        }
    }
    return pto_minimum;
}


void PathWaterCallaghanAlgorithm::changeAttr(){
    this->run(coords_asked, ter);
    emit reload();
}

std::vector<glm::vec3> PathWaterCallaghanAlgorithm::getPathWater(){
    return this->edge_water;
}
