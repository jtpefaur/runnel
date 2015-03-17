#include "peuckerdrainagenetwork.h"
#include "terrain.h"
#include "primitives/point.h"
#include <cmath>

PeuckerDrainageNetwork::PeuckerDrainageNetwork():
    DrainageAlgorithms()
{
    shader = 0;
    terr = 0;

}

PeuckerDrainageNetwork::~PeuckerDrainageNetwork(){
    if(shader){
        delete shader;
    }
}

void PeuckerDrainageNetwork::run(Terrain *ter){
    terr = ter;
    this->calculateGrid(ter);
    std::vector<glm::vec3> color = ter->getDrainageColor();
    std::vector<glm::vec3> position = ter->getPointsEdgeDrainage();
    shader->fillPositionBuffer(position, color);
}

void PeuckerDrainageNetwork::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if(shader){
        shader->render(matrix, exag_z, color);
    }
}

void PeuckerDrainageNetwork::glewReady(){
    shader = new ShaderDrainage();
}

QString PeuckerDrainageNetwork::getName(){
    return QString("Peucker");
}

QWidget* PeuckerDrainageNetwork::getConf(){
    return &conf;
}

void PeuckerDrainageNetwork::calculateGrid(Terrain *ter){
    int width = ter->width;
    int height = ter->height;

    for(unsigned int i = 0; i < ter->struct_point.size() ; ++i){
        int fila = i/width;

        if ( (i+1) % width == 0){
            continue;
        }
        if( fila == (height-1)){
            break;
        }
        runnel::Point *points[4];
        points[0] = ter->struct_point[i];
        points[1] = ter->struct_point[i + 1];
        points[2] = ter->struct_point[i + width];
        points[3] = ter->struct_point[i + width + 1];

        if(!points[0] || !points[1] || !points[2] || !points[3]){
            continue;
        }

        runnel::Point *max_point = points[0];
        for(runnel::Point* pto: points){
            if(pto->coord.z > max_point->coord.z){
                max_point = pto;
            }
        }

        for(runnel::Point* pto: points){
            if(pto->coord.z == max_point->coord.z){
                max_point->setFlagsOn(runnel::Point::PEUCKER);
            }
        }


    }
}

std::vector<glm::vec3> PeuckerDrainageNetwork::getPathTree(){
    return terr->getPointsEdgeDrainage();
}
