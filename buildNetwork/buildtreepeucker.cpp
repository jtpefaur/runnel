#include "buildtreepeucker.h"
#include <iostream>
#include <algorithm>

struct customMorePeucker{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->coord.z < p1->coord.z;
    }
};

BuildTreePeucker::BuildTreePeucker(){
    shader = 0;
}

BuildTreePeucker::~BuildTreePeucker(){
    if (shader){
        delete shader;
    }

}

std::vector<arbol*> BuildTreePeucker::run(Terrain* ter){
    t = ter;
    this->calculateGrid(ter);
    this->createStructure(ter->struct_point, ter);
    this->reviewPoints();
    std::vector<std::string> types;
    types.clear();
    shader->fillPositionBuffer(arbolitos, types);
    return arbolitos;

}

void BuildTreePeucker::render(glm::mat4 matrix, float exag_z){
    if (shader){
        shader->render(matrix, exag_z);
    }
}

void BuildTreePeucker::glewReady(){
    shader = new ShaderPatron();
}

QString BuildTreePeucker::getName(){
    return QString("Peucker");
}

QWidget* BuildTreePeucker::getConf(){
    return &conf;
}

void BuildTreePeucker::createStructure(std::vector<runnel::Point*>& points, Terrain* ter)
{
    points_order = points;
    std::sort( points_order.begin(), points_order.end(), customMorePeucker());
    for (runnel::Point* pto: points_order){
         point_counter_peucker[pto->ident] = 0;

    }

}



void BuildTreePeucker::createTree(arbol* ar){
    runnel::Point* pto = ar->pto;
    std::vector<int> position_neightbour;
    if(pto->ident - t->width >= 0){
        position_neightbour.push_back(pto->ident - t->width);
    }
    if(pto->ident + t->width < t->width*t->height){
        position_neightbour.push_back(pto->ident + t->width );
    }
    if(pto->ident%t->width > 0){
        position_neightbour.push_back(pto->ident + t->width - 1);
        position_neightbour.push_back(pto->ident - t->width - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%t->width < (t->width-1)){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - t->width + 1);
        position_neightbour.push_back(pto->ident + t->width + 1);
    }

    for(int position: position_neightbour){
        if(position >= (int)t->struct_point.size() || position < 0)
            continue;
        runnel::Point* p = t->struct_point[position];
    //    std::cout << p->isFlagsOn(p->PEUCKER) << std::endl;
        if(!p->isFlagsOn(p->PEUCKER)){

            if(point_counter_peucker[p->ident] == 0){

                arbol *child = new arbol(p);
                ar->hijos.push_back(child);
                point_counter_peucker[p->ident] = 1;
                this->createTree(child);
            }
        }

    }
    return;
}

std::vector<arbol*> BuildTreePeucker::reviewPoints(){
    arbolitos.clear();
    for(runnel::Point* pto: points_order){

        if(point_counter_peucker[pto->ident] == 1){ //ya fue utilizado
            continue;
        }else{
            if(!pto->isFlagsOn(pto->PEUCKER)){
                arbol* pa = new arbol(pto);
                point_counter_peucker[pto->ident] = 1;
                this->createTree(pa);
                arbolitos.push_back(pa);
            }

        }
    }

    for(arbol* b: arbolitos){
        b->getNumberStrahlerHorton();
    }


}

void BuildTreePeucker::calculateGrid(Terrain *ter){
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
