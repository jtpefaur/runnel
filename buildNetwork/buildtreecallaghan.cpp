#include "buildtreecallaghan.h"
#include <iostream>
#include <algorithm>

struct customMore{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->water_value > p2->water_value;
    }
};

struct customLess{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->coord.z > p2->coord.z;
    }
};

buildTreeCallaghan::buildTreeCallaghan(){
    shader_build = 0;
    terr = 0;
    max_water = 1/10000.0f;
    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));

}

buildTreeCallaghan::~buildTreeCallaghan(){
    if(shader_build){
        delete shader_build;
    }
}

std::vector<arbol*> buildTreeCallaghan::run(Terrain* ter){
    terr = ter;
//    for(runnel::Point* pto: terr->struct_point){
//        pto->water_value = 0;
//    }
    point_counter.clear();
    arbolitos.clear();
    max_water = conf.getMaxWater()/10000.0f;
    this->getPoints(ter->struct_point);

    this->reviewPoints();
    std::vector<std::string> types;
    types.clear();

    shader_build->fillPositionBuffer(arbolitos, types);
    return arbolitos;
}

std::vector<arbol*> buildTreeCallaghan::getData(){
    return arbolitos;
}

void buildTreeCallaghan::render(glm::mat4 matrix, float exag_z){
    if (shader_build){
        shader_build->render(matrix, exag_z);
    }
}


void buildTreeCallaghan::glewReady(){
    shader_build = new ShaderPatron();
}


QString buildTreeCallaghan::getName(){
    return QString("Callaghan");
}


QWidget* buildTreeCallaghan::getConf(){
    return &conf;
}



void buildTreeCallaghan::getPoints(std::vector<runnel::Point*>& points)
{
    points_order = points;
    std::sort( points_order.begin(), points_order.end(), customLess());
    for(runnel::Point* pto : points_order){
        this->chooseMoreDepthPoint(terr->struct_point, pto);
    }
    terr->getMoreWaterPoint();
    points_order = points;
    std::sort( points_order.begin(), points_order.end(), customMore());
    std::cout << terr->max_value_water << std::endl;
    for (runnel::Point* pto: points_order){
        if (pto->water_value > max_water*terr->max_value_water){
            point_counter[pto->ident] = 1;
        }else{
            //point was used
            point_counter[pto->ident] = 0;
        }
    }

}

void buildTreeCallaghan::chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto){
    int w = terr->width;
    float delta_water = 0;
    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - w);
    position_neightbour.push_back(pto->ident + w);
    if(pto->ident%w > 0){
        position_neightbour.push_back(pto->ident + w - 1);
        position_neightbour.push_back(pto->ident - w - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%w < w-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - w + 1);
        position_neightbour.push_back(pto->ident + w + 1);
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

void buildTreeCallaghan::reviewPoints(){
    for(runnel::Point* pto: points_order){
       // std::cout << "first point "<< pto->ident << " mi altura es " << pto->coord.z << std::endl;
        if(point_counter[pto->ident] == 0){ //ya fue utilizado
            continue;
        }else{
            arbol* pa = new arbol(pto);
            point_counter[pto->ident] = 0;
            this->createTree(pa);
            arbolitos.push_back(pa);
        }
    }

    for(arbol* b: arbolitos){
        b->getNumberStrahlerHorton();
    }
}

void buildTreeCallaghan::createTree(arbol* parent){
    if(parent->pto->water_parent.size() > 0){
        for(runnel::Point* pto: parent->pto->water_parent){
            if( point_counter[pto->ident] == 1){
                arbol* pa = new arbol(pto);
                parent->hijos.push_back(pa);
                point_counter[pto->ident] = 0;
                this->createTree(pa);
             }
        }
    }

}

void buildTreeCallaghan::changeAttr(){
    this->run(terr);
}

