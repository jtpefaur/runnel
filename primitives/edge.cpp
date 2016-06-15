#include "edge.h"
#include "primitives/triangle.h"
#include <iostream>
#include <iomanip>
#define PI 3.14159265

runnel::Edge::Edge(runnel::Point* value1, runnel::Point* value2)
{
    point1 = value1;
    point2 = value2;
    calculateEdgeVector();
}

void runnel::Edge::addTriangle(runnel::Triangle *t){
    neighbour_triangle.push_back(t);
}

void runnel::Edge::calculateEdgeVector(){
    edge_vector = point1->coord - point2->coord;
}

float runnel::Edge::calculateAngleDiedro(){
    float angulo_diedro = 180;
    if(neighbour_triangle.size() == 2){
        runnel::Triangle *t1 = neighbour_triangle[0];
        runnel::Triangle *t2 = neighbour_triangle[1];
        //std::cout << "Producto punto normales : " << glm::dot(t1->normal, t2->normal)<<" "<<std::hex << glm::dot(t1->normal, t2->normal)<<std::dec<<std::endl;
        angulo_diedro = glm::acos(glm::dot(t1->normal, t2->normal))*180.0 / PI;
        glm::vec3 crossProduct = glm::cross(t1->normal, t2->normal);
        bool sameDirection = glm::dot(crossProduct, edge_vector) >= 0;
        if(sameDirection)
            angulo_diedro = -angulo_diedro;
    }
    return angulo_diedro;
    //std::cout << "angulo diedro " << angulo_diedro << std::endl;
}
