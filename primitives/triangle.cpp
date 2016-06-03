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

glm::vec3 runnel::Triangle::calculateHeight(){
    glm::vec3 vec_a = points[1]->coord - points[0]->coord;
    glm::vec3 vec_b = points[2]->coord - points[1]->coord;
    glm::vec3 vec_c = points[0]->coord - points[2]->coord;

    glm::vec3 sides;
    sides.x = std::sqrt(vec_a.x*vec_a.x + vec_a.y*vec_a.y + vec_a.z*vec_a.z);
    sides.y = std::sqrt(vec_b.x*vec_b.x + vec_b.y*vec_b.y + vec_b.z*vec_b.z);
    sides.z = std::sqrt(vec_c.x*vec_c.x + vec_c.y*vec_c.y + vec_c.z*vec_c.z);
    float s = sides.x + sides.y + sides.z;
    glm::vec3 height;
    height.x = 2*std::sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.x;
    height.y = 2*std::sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.y;
    height.z = 2*std::sqrt(s*(s - sides.x)*(s - sides.y)*(s - sides.z))/sides.z;
    return height;

}
void runnel::Triangle::calculateLineHorizontal(){
    glm::vec3 vector_z = glm::vec3(0,0,-1);
    glm::vec3 vector_b = glm::dot(this->normal, vector_z)*this->normal;
    glm::vec3 vector_a = glm::normalize(vector_z - vector_b);
    float value = 0;
    for(runnel::Edge*ed : this->edges){
        value += std::sqrt(glm::dot(ed->edge_vector,ed->edge_vector));
    }
    vector_a = -vector_a*value/3.0f;
    gradient.push_back(this->incentro);
    gradient.push_back(this->incentro + vector_a);
    gradient_vector = vector_a;
}


std::vector<runnel::Triangle*> runnel::Triangle::getNeighbours(){
    std::vector<Triangle*> neighbour;
    for (runnel::Edge* ed: this->edges){
        runnel::Triangle* trian_first = ed->neighbour_triangle[0];
        if ( trian_first->ident == this->ident ){
            neighbour.push_back(trian_first);
        }else{
            neighbour.push_back(ed->neighbour_triangle[1]);
        }
    }
    return neighbour;
}
