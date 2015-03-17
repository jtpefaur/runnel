#include "pathwatergradientalgorithm.h"

PathWaterGradientAlgorithm::PathWaterGradientAlgorithm(){
    shader = 0;
}

PathWaterGradientAlgorithm::~PathWaterGradientAlgorithm(){
    if(shader){
        delete shader;
    }
}

void PathWaterGradientAlgorithm::run(glm::vec3 point, Terrain *terr){
    this->ter = terr;
    edge_path.clear();

}

void PathWaterGradientAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if(shader){

    }
}

void PathWaterGradientAlgorithm::glewReady(){
    shader = new ShaderGradient();
}

QString PathWaterGradientAlgorithm::getName(){
    return QString("Gradient");
}

QWidget* PathWaterGradientAlgorithm::getConf(){
    return 0;
}

runnel::Triangle* PathWaterGradientAlgorithm::getTriangleOfPoint(glm::vec3 coords){
    float minimum_distance = glm::distance(this->ter->struct_triangle[0]->incentro, coords);
    runnel::Triangle* triangle_minimum_distance = this->ter->struct_triangle[0];
    for(runnel::Triangle* trian : this->ter->struct_triangle){
        float distance = glm::distance(trian->incentro, coords);
        if( distance < minimum_distance){
            triangle_minimum_distance = trian;
            minimum_distance = distance;
        }
    }
    return triangle_minimum_distance;
}

runnel::Triangle* PathWaterGradientAlgorithm::getNextTriangle(runnel::Triangle* trian, glm::vec3 pto){
    glm::vec3 vector_z = glm::vec3(0, 0, -1);
    glm::vec3 vector_b = glm::dot(trian->normal, -vector_z)*trian->normal;
    glm::vec3 vector_a = - vector_z - vector_b;
    std::pair<runnel::Triangle*, glm::vec3> value = this->getEdgeInterseption(trian, vector_a, pto);

    return this->getNextTriangle(value.first, value.second);

}

std::pair<runnel::Triangle*, glm::vec3> PathWaterGradientAlgorithm::getEdgeInterseption(runnel::Triangle*trian, glm::vec3 vector_a, glm::vec3 pto){
    for(runnel::Edge* ed: trian->edges){
//         if (! isOnEdge(points[ed->id1], points[ed->id2], pto_interception)){

//         }
    }
}

bool PathWaterGradientAlgorithm::isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto){
    bool isOn = false;
    glm::vec3 min_edge = glm::min(p1->coord, p2->coord);
    glm::vec3 max_edge = glm::max(p1->coord, p2->coord);

    if(glm::all(glm::lessThanEqual(min_edge, pto)) && glm::all(glm::greaterThanEqual(max_edge, pto)) ){
        return true;
    }
    return isOn;
}

std::vector<glm::vec3> PathWaterGradientAlgorithm::getPathWater(){
    return edge_path;
}
