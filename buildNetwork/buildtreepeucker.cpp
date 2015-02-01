#include "buildtreepeucker.h"
#include <iostream>
#include <algorithm>

struct customMorePeucker{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->coord.z < p1->coord.z;
    }
};



BuildTreePeucker::BuildTreePeucker(std::vector<runnel::Point*>& points, Terrain* ter)
{
    points_order = points;
    t = ter;
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
    std::vector<arbol*> arbolitos;
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

    return arbolitos;
}
