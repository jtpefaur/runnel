#include "gradientdrainagecallaghanmark.h"
#include <iostream>
#include <algorithm>
#include "primitives/point.h"
struct customLess{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->coord.z > p2->coord.z;
    }
};


GradientDrainageCallaghanMark::GradientDrainageCallaghanMark():
    DrainageAlgorithms()
{
    w = 0;
    h = 0;
    shader_callaghan = 0;
   }

GradientDrainageCallaghanMark::~GradientDrainageCallaghanMark(){
    if(shader_callaghan){
        delete shader_callaghan;
    }
}

QString GradientDrainageCallaghanMark::getName(){
    return QString("Callaghan");
}

void GradientDrainageCallaghanMark::sortElement(std::vector<runnel::Point *> points){
    points_terrain =  points;
    std::sort( points_terrain.begin(), points_terrain.end(), customLess());
}


void GradientDrainageCallaghanMark::run(Terrain* ter){
    w = ter->width;
    h = ter->height;
    GradientDrainageCallaghanMark::sortElement(ter->struct_point);

    for(runnel::Point* pto : points_terrain){
        this->chooseMoreDepthPoint(ter->struct_point, pto);
    }
    ter->getMoreWaterPoint();
    shader_callaghan->fillPositionBuffer(ter->position_water_points, ter->count_water);

}

void GradientDrainageCallaghanMark::chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto){
    moreWater = conf.getWater();
    float delta_water = moreWater;
    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - this->w);
    position_neightbour.push_back(pto->ident + this->w);
    if(pto->ident%w > 0){
        position_neightbour.push_back(pto->ident + this->w - 1);
        position_neightbour.push_back(pto->ident - this->w - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%w < w-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - this->w + 1);
        position_neightbour.push_back(pto->ident + this->w + 1);
    }
    float max_z = -1;
    int id_max = -1;
    for (int id : position_neightbour){
        if(id >= 0 && id < points.size()){
            if( points[id]->coord.z <= (pto->coord.z +delta_water)){
                float dist_z = pto->coord.z - points[id]->coord.z;
                if( max_z < dist_z){
                    max_z = dist_z;
                    id_max = id;
                }
            }
        }
    }
    if(!(max_z == -1)){
        points[id_max]->water_parent.push_back(points[pto->ident]);
        points[id_max]->water_value = points[id_max]->water_value + points[pto->ident]->water_value ;
    }
}


void GradientDrainageCallaghanMark::glewReady(){
    shader_callaghan = new ShaderCallaghan();
}

void GradientDrainageCallaghanMark::render(glm::mat4 matrix, float exag_z, glm::vec3 color){

    if (shader_callaghan){
        shader_callaghan->render(matrix, exag_z, conf.getLineWater(), moreWater, color);
    }
}

QWidget* GradientDrainageCallaghanMark::getConf(){

    return &conf;
}
