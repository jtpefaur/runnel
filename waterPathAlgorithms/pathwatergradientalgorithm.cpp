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
    triangles_id.clear();
    runnel::Triangle*trian = this->getTriangleOfPoint(point);
    this->getNextTriangle(trian);
    shader->fillPositionBuffer(edge_path);
}

void PathWaterGradientAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if(shader){
        shader->render(matrix, exag_z, color);
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

void PathWaterGradientAlgorithm::getNextTriangle(runnel::Triangle* trian){
    glm::vec3 vector_z = glm::vec3(0, 0, -1);
    runnel::Triangle* trian_ant = 0;
    while(1){
        glm::vec3 vector_b = glm::dot(trian->normal, -vector_z)*trian->normal;
        glm::vec3 vector_a =  vector_z + vector_b;

        if(triangles_id[trian->ident]){
            return;
        }
        float maximun_angle = -100;
        runnel::Edge* interception_edge = 0;
        runnel::Edge* interception_edge_old = 0;
        for(runnel::Edge* ed : trian->edges){
            runnel::Triangle* trian_other = this->calculateOtherTriangleInEdge(trian, ed);
            if(trian_other == 0){
                continue;
            }

            float value = glm::dot(glm::vec2(glm::normalize(trian_other->incentro - trian->incentro)), glm::vec2( vector_a));

            if(value > maximun_angle){
                interception_edge_old = interception_edge;
                interception_edge = ed;
                maximun_angle = value;
            }
        }
        if(!interception_edge){
    //        std::cout<<"Interception edge no encontrada"<<std::endl;
            return;
        }
        runnel::Triangle* next_triangle = calculateOtherTriangleInEdge(trian, interception_edge);
        if(!next_triangle)
            return;
        if(trian_ant == next_triangle){

            next_triangle = calculateOtherTriangleInEdge(trian, interception_edge_old);
        }
        edge_path.push_back(trian->incentro);
        edge_path.push_back(next_triangle->incentro);
        triangles_id[trian->ident] = 1;
        trian = next_triangle;
    }

}

runnel::Triangle* PathWaterGradientAlgorithm::calculateOtherTriangleInEdge(runnel::Triangle* trian_initial, runnel::Edge* ed){
    for(runnel::Triangle* trian: ed->neighbour_triangle){
        if(!(trian->ident == trian_initial->ident)){
            return trian;
        }
    }
    return (runnel::Triangle*)0;
}


std::vector<glm::vec3> PathWaterGradientAlgorithm::getPathWater(){
    return edge_path;
}
