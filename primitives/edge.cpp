#include "edge.h"
#include "primitives/triangle.h"
#include <iostream>
#include <iomanip>
#define PI 3.14159265

runnel::Edge::Edge(runnel::Point* value1, runnel::Point* value2)
{
    edge_vector = value2->coord - value1->coord;
    calculate_angulo_diendro = 0;
    id1 = value1->ident;
    id2 = value2->ident;
}

void runnel::Edge::addTriangle(runnel::Triangle *t){
    neighbour_triangle.push_back(t);
}

void runnel::Edge::calculateAngleDiedro(){
    angulo_diedro = 180;
    if(neighbour_triangle.size() == 2){
        runnel::Triangle *t1 = neighbour_triangle[0];
        runnel::Triangle *t2 = neighbour_triangle[1];
        //std::cout << "Producto punto normales : " << glm::dot(t1->normal, t2->normal)<<" "<<std::hex << glm::dot(t1->normal, t2->normal)<<std::dec<<std::endl;
        angulo_diedro = glm::acos(glm::dot(t1->normal, t2->normal))*180.0 / PI;
        glm::vec3 edge = getEdgeVector(t1);
        glm::vec3 crossProduct = glm::cross(t1->normal, t2->normal);
        bool sameDirection = glm::dot(crossProduct, edge) >= 0;
        if(sameDirection)
            angulo_diedro = -angulo_diedro;
    }
    calculate_angulo_diendro = 1;

    //std::cout << "angulo diedro " << angulo_diedro << std::endl;
}

glm::vec3 runnel::Edge::getEdgeVector(runnel::Triangle* t1){
    for(int i = 0;i<3;i++){
        if((t1->points[i]->ident == id1 && t1->points[(i+1)%3]->ident == id2)
           || (t1->points[i]->ident == id2 && t1->points[(i+1)%3]->ident == id1)){
            return t1->points[(i+1)%3]->coord - t1->points[i]->coord;
        }
    }
    return glm::vec3();
}
