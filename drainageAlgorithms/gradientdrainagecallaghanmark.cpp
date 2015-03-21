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
    max_water = 0;
    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));
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
    this->ter = ter;
    count_water.clear();
    position_water_points.clear();
    w = this->ter->width;
    h = this->ter->height;
    max_value_water = 0;

    for(runnel::Point* pto: this->ter->struct_point){
        pto->water_value = 1;
        pto->water_parent.clear();
    }
    GradientDrainageCallaghanMark::sortElement(ter->struct_point);

    for(runnel::Point* pto : points_terrain){
        this->chooseMoreDepthPoint(ter->struct_point, pto);
    }
    this->getMoreWaterPoint();
    max_water = max_value_water;
    shader_callaghan->fillPositionBuffer(position_water_points, count_water);

}

void GradientDrainageCallaghanMark::chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto){
    moreWater = conf.getWater()/100.0f;
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
    linewater = conf.getLineWater()/1000.0f;
    if (shader_callaghan){
        shader_callaghan->render(matrix, exag_z, linewater , max_water, color);
    }
}

QWidget* GradientDrainageCallaghanMark::getConf(){

    return &conf;
}

void GradientDrainageCallaghanMark::changeAttr(){
    this->run(ter);
    emit reload();
}

std::vector<glm::vec3> GradientDrainageCallaghanMark::getPathTree(){
    std::vector<glm::vec3> points_path;
    std::vector<glm::vec3> position_water = position_water_points;
    int i = 0;
    while(i< position_water.size()){
        if (count_water[i]/max_water > linewater){
            points_path.push_back(position_water[i]);
        }
        ++i;
    }
    return points_path;
}


void GradientDrainageCallaghanMark::getMoreWaterPoint(){

    for (runnel::Edge* edge: this->ter->struct_edge){
        float water_1 = this->ter->struct_point[edge->id1]->water_value;
        float water_2 = this->ter->struct_point[edge->id2]->water_value;
        float value_water =  std::max(water_1, water_2);
        max_value_water = std::max(max_value_water,value_water);
        count_water.push_back(value_water);
        count_water.push_back(value_water);
        position_water_points.push_back(this->ter->struct_point[edge->id1]->coord);
        position_water_points.push_back(this->ter->struct_point[edge->id2]->coord);
    }
}
