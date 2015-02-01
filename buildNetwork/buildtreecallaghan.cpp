#include "buildtreecallaghan.h"
#include <iostream>
#include <algorithm>

struct customMore{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->water_value > p2->water_value;
    }
};

buildTreeCallaghan::buildTreeCallaghan(std::vector<runnel::Point*>& points, float water_max)
{
    points_order = points;
    max_water = water_max;
    std::sort( points_order.begin(), points_order.end(), customMore());
    for (runnel::Point* pto: points_order){
        if (pto->water_value > 0.02*max_water){
            point_counter[pto->ident] = 1;
        }else{
            point_counter[pto->ident] = 0;
        }
    }
}

std::vector<arbol*> buildTreeCallaghan::reviewPoints(){
    std::vector<arbol*> arbolitos;
    for(runnel::Point* pto: points_order){
       // std::cout << "first point "<< pto->ident << " mi altura es " << pto->coord.z << std::endl;
        if(point_counter[pto->ident] == 0){ //ya fue utilizado
            continue;
        }else{
            if(1){
                arbol* pa = new arbol(pto);
                point_counter[pto->ident] = 0;
                this->createTree(pa);
                arbolitos.push_back(pa);
            }else{
                std::cout << "alguno de los padres fue utilizado D:" << std::endl;
            }
        }
    }
//    std::cout << "cantidad de arbolitos" << arbolitos.size() << std::endl;
    for(arbol* b: arbolitos){
        b->getNumberStrahlerHorton();
    }

    return arbolitos;

}

void buildTreeCallaghan::createTree(arbol* parent){
    if(parent->pto->water_parent.size() > 0){
      //  std::cout << "tamano del arbol "<< parent->pto->water_parent.size() << std::endl;
        for(runnel::Point* pto: parent->pto->water_parent){
            if( point_counter[pto->ident] == 1){
                arbol* pa = new arbol(pto);
                parent->hijos.push_back(pa);
                point_counter[pto->ident] = 0;
                this->createTree(pa);
             }else{
             //   std::cout << "no cumple con la cantidad de agua" << std::endl;
            }
        }
    }

}

bool buildTreeCallaghan::isAnyFatherUsed(runnel::Point* pto){
    bool used = false;
    for(runnel::Point* paren : pto->water_parent){
        used = used || (point_counter[paren->ident] == 0);
    }
    return used;
}
