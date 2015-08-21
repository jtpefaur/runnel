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
    max_water = 1/100000.0f;
    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));

}

buildTreeCallaghan::~buildTreeCallaghan(){
    if(shader_build){
        delete shader_build;
    }
}

std::vector<arbol*> buildTreeCallaghan::run(Terrain* ter){
    terr = ter;
    max_value_water = 0;
    for(runnel::Point* pto: terr->struct_point){
        pto->water_value = 1;
        pto->water_parent.clear();
    }
    point_counter.clear();
    arbolitos.clear();
    max_water = conf.getMaxWater()/100.0f;

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
    this->getMoreWaterPoint();
    points_order = points;
    std::sort( points_order.begin(), points_order.end(), customMore());
    std::cout << max_value_water << std::endl;
    for (runnel::Point* pto: points_order){
        if (pto->water_value > max_water*max_value_water){
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
        b->computeNetworkStrahlerOrdering();
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
    emit reload();
}

std::vector<glm::vec3> buildTreeCallaghan::getPathTree(){
    std::vector<runnel::Point*> pointPath;
    pointPath.clear();
    for( arbol* ar: arbolitos){
        std::vector<runnel::Point*> points;
        ar->getArbolEdges(points);
        pointPath.insert(pointPath.end(), points.begin(), points.end());
        points.clear();
    }
    std::vector<glm::vec3> coordinatePath;
    for (runnel::Point* point : pointPath) {
        coordinatePath.push_back(point->coord);
    }
    return coordinatePath;
}

void buildTreeCallaghan::getMoreWaterPoint(){

    for (runnel::Edge* edge: this->terr->struct_edge){
        float water_1 = this->terr->struct_point[edge->id1]->water_value;
        float water_2 = this->terr->struct_point[edge->id2]->water_value;
        float value_water =  std::max(water_1, water_2);
        max_value_water = std::max(max_value_water,value_water);
    }
}
