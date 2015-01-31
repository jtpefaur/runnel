#include "gradientdrainagehalfpointnetwork.h"
#include "iostream"



void GradientDrainageHalfPointNetwork::calculateDrainage(Terrain* ter, glm::vec3 pto, std::vector<glm::vec3>& points_gradient){
    runnel::Triangle* trian = ter->struct_triangle[41];
    calculateNextTriangle(trian, ter->struct_point, points_gradient);
}


void GradientDrainageHalfPointNetwork::calculateNextTriangle(runnel::Triangle* trian, std::vector<runnel::Point*> points, std::vector<glm::vec3>& points_gradient){
    std::unordered_map<int, int> triangle_path;
    runnel::Triangle* trian_ant = 0;
    while(1){
        glm::vec3 pto_half = trian->incentro;

        glm::vec3 vector_movement_gradient = trian->gradient_vector + pto_half;
        if(triangle_path[trian->ident]){
            return;
        }
        runnel::Edge* interception_edge = 0;
        runnel::Edge* interception_edge_old = 0;
        float maximun_angle = -100;
        for(runnel::Edge* ed : trian->edges){
            runnel::Triangle* trian_other = calculateOtherTriangleInEdge(trian, ed);
            if(trian_other == 0){
                continue;
            }

            float value = glm::dot(glm::vec2(glm::normalize(trian_other->incentro - trian->incentro)), glm::vec2( trian->gradient_vector));
                if(value > maximun_angle){
                    interception_edge_old = interception_edge;
                    interception_edge = ed;
                    maximun_angle = value;
                }
        }
        if(!interception_edge){
            return;
        }
        runnel::Triangle* next_triangle = calculateOtherTriangleInEdge(trian, interception_edge);
        if(!next_triangle)
            return;
        if(trian_ant == next_triangle){

            next_triangle = calculateOtherTriangleInEdge(trian, interception_edge_old);
        }
        points_gradient.push_back(pto_half);
        points_gradient.push_back(next_triangle->incentro);
        triangle_path[trian->ident] = 1;
        trian = next_triangle;
    }

}


bool GradientDrainageHalfPointNetwork::isSamePoint(glm::vec3 pto1, glm::vec3 pto2){
    return glm::all(glm::equal(pto1, pto2));
}

glm::vec3 GradientDrainageHalfPointNetwork::calculatePointInterception(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4){
    glm::vec3 a = p2-p1;
    glm::vec3 b = p4-p3;
    glm::vec3 c = p3-p1;
    float s = glm::dot(glm::cross(c, b), glm::cross(a, b))/glm::dot(glm::cross(a, b),glm::cross(a, b));
    glm::vec3 X = p1 + s*a;
    return X;
}

bool GradientDrainageHalfPointNetwork::isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto){
    bool isOn = false;
//    glm::vec3 min_edge = glm::min(p1->coord, p2->coord);
//    if(glm::all(glm::lessThanEqual(min_edge, pto)) && glm::all(glm::greaterThanEqual(max_edge, pto)) ){
//        return true;
//    }
    return isOn;
}

runnel::Triangle* GradientDrainageHalfPointNetwork::calculateOtherTriangleInEdge(runnel::Triangle* trian_initial, runnel::Edge* ed){
    for(runnel::Triangle* trian: ed->neighbour_triangle){
        if(!(trian->ident == trian_initial->ident)){
            return trian;
        }
    }
    return (runnel::Triangle*)0;
}
