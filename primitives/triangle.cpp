#include "triangle.h"
#include <iostream>
runnel::Triangle::Triangle(int id)
{
    incentro = glm::vec3(0,0,0);
    ident = id;
}

void runnel::Triangle::addPoint(runnel::Point* p){
    points.push_back(p);
}

void runnel::Triangle::addGroupPoints(runnel::Point* p1, runnel::Point* p2, runnel::Point* p3){
    this->addPoint(p1);
    this->addPoint(p2);
    this->addPoint(p3);
}

void runnel::Triangle::addPositionPoint(int x){
    position.push_back(x);
}

void runnel::Triangle::addEdge(runnel::Edge* e){
    edges.push_back(e);
}

void runnel::Triangle::addGroupEdges(runnel::Edge* e1, runnel::Edge* e2, runnel::Edge* e3){
    this->addEdge(e1);
    this->addEdge(e2);
    this->addEdge(e3);
}

void runnel::Triangle::calculateNormalVector(){
    normal = glm::normalize(glm::cross(edges[2]->edge_vector, edges[0]->edge_vector));
}

void runnel::Triangle::calculateIncentroPoint(){

    incentro = (points[0]->coord + points[1]->coord + points[2]->coord)/3.0f;

}

void runnel::Triangle::calculateHeight(){
    glm::vec3 vec_a = points[1]->coord - points[0]->coord;
    glm::vec3 vec_b = points[2]->coord - points[1]->coord;
    glm::vec3 vec_c = points[0]->coord - points[2]->coord;

    sides.x = std::sqrt(vec_a.x*vec_a.x + vec_a.y*vec_a.y + vec_a.z*vec_a.z);
    sides.y = std::sqrt(vec_b.x*vec_b.x + vec_b.y*vec_b.y + vec_b.z*vec_b.z);
    sides.z = std::sqrt(vec_c.x*vec_c.x + vec_c.y*vec_c.y + vec_c.z*vec_c.z);
    float s = sides.x + sides.y + sides.z;
    height.x = 2*std::sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.x;
    height.y = 2*std::sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.y;
    height.z = 2*std::sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.z;

}
glm::vec3 runnel::Triangle::calculateLineHorizontal(){
    glm::vec3 point_4 = glm::vec3(0,0,0);
    std::vector<glm::vec3> point_orden = this->calculate_orden();
    float alpha = (point_orden[0].z - point_orden[1].z)/(point_orden[2].z - point_orden[1].z);
    point_4.x = point_orden[1].x + (point_orden[2].x - point_orden[1].x)*alpha;
    point_4.y = point_orden[1].y + (point_orden[2].y - point_orden[1].y)*alpha;
    point_4.z = point_orden[0].z;
    glm::vec3 vector_horizontal = point_4 - point_orden[0];
    float pdte = (point_4.y - point_orden[0].y)/(point_4.x - point_orden[0].x);
    float cte = point_4.y - point_4.x*pdte;

    float lambda = glm::dot(glm::normalize(point_4 - point_orden[0]), point_orden[1] - point_orden[0]);
    glm::vec3 point_5 = point_orden[0] + lambda*(glm::normalize(point_4 - point_orden[0]));
    gradient.push_back(point_orden[1]);
    gradient.push_back(point_5);
    gradient_vector = point_5 - point_orden[1];
    return vector_horizontal;
}

//void runnel::Triangle::void calculateGradient();
std::vector<glm::vec3> runnel::Triangle::calculate_orden(){
    std::vector<glm::vec3> result_point;
    result_point.push_back(points[2]->coord);
    result_point.push_back(points[0]->coord);
    result_point.push_back(points[1]->coord);

//    for(unsigned int j = 0; j < 2 ;  ++j){
//        for(unsigned int i = 0; i < 2 ; ++i){
//            if(!(result_point[i].x > result_point[i+1].x)){
//                glm::vec3 aux = result_point[i];
//                result_point[i] = result_point[i+1];
//                result_point[i+1] = aux;
//            }
//        }
//    }

    for(unsigned int j = 1; j < 3 ;  ++j){
        if(points[j]->coord.z > result_point[1].z){
           result_point[1] = points[j]->coord;
           result_point[0] = points[j-1]->coord;
           result_point[2] = points[(j+1)%3]->coord;
        }
    }

    return result_point;
}


