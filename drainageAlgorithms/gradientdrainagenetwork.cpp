#include "gradientdrainagenetwork.h"
#include "iostream"

void GradientDrainageNetwork::calculateFinalPointTriangle(std::vector<glm::vec3>& points_gradient, std::vector<runnel::Point *> points ,runnel::Triangle* t, glm::vec3 pto){
    std::cout << " valor del gradiente " << t->gradient_vector.z << std::endl;
    if (t->gradient_vector.z > 0){
        return;
    }
    std::cout << "punto de inicio " << pto.x << " " << pto.y << pto.z << std::endl;
    glm::vec3 movement_water = pto + t->gradient_vector*10.0f;
    std::cout << "movimiento de la agua " << movement_water.x << " " << movement_water.y << " " << movement_water.z << std::endl;

    std::vector<runnel::Edge*> edge_opposite = getEdgeOpposite( points, t, pto);

    runnel::Edge *final = edge_opposite[0];
    glm::vec3 pto_final = glm::vec3(0,0,0);
   // std::vector<glm::vec3> pto_interseption;

    for(runnel::Edge* ed : edge_opposite){
        glm::vec3 p5 = calculatePointInterseption(points[ed->id1]->coord, points[ed->id2]->coord, pto, movement_water);
        std::cout << "punto p5 " << p5.x << " " << p5.y << " "<<  p5.z << std::endl;
        if (isOnEdge(points[ed->id1], points[ed->id2], p5) && !isSamePoint(pto, p5)    ){
            final = ed;
            pto_final = p5;

        }

    }
    glm::all(glm::equal(pto_final, glm::vec3(0,0,0)));
    if(isSamePoint(pto_final, glm::vec3(0,0,0)) ){
        //buscar en los triangulos vecinos
        std::vector<runnel::Triangle *> triangle_neighbour;
        getTriangleNeigthbour(t, triangle_neighbour, points, t,  pto, pto, t);

//        for(runnel::Edge* ed_neigh: t->edges){
//            if(isOnEdge(points[ed_neigh->id1], points[ed_neigh->id2], pto)){
//                if(ed_neigh->neighbour_triangle[0]->ident == t->ident){

//                }
//                if(ed_neigh->neighbour_triangle[0]->ident == t->ident){

//                }
//            }
//        }

    }
    runnel::Triangle* trian_opposite_edge = getTriangleForEdge(final, t);
    points_gradient.push_back(pto);
    points_gradient.push_back(pto_final);
    std::cout << "value of gradient " << pto.x << " " << pto.y << " " << pto.z << " " << pto_final.x << " " << pto_final.y << " " << pto_final.z << std::endl;
    calculateFinalPointTriangle(points_gradient, points ,trian_opposite_edge, pto_final);
}


runnel::Triangle* GradientDrainageNetwork::getTriangleForEdge(runnel::Edge* ed, runnel::Triangle* t){
    for(runnel::Triangle* trian: ed->neighbour_triangle){
        if(!trian->ident == t->ident){
            return trian;
        }
    }
}

glm::vec3 GradientDrainageNetwork::calculatePointInterseption(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4){
//    float A = (p3.x - p1.x)/(p2.x - p1.x);
//    float B = (p3.y - p1.y)/(p2.y - p1.y);
//    float C = (p4.y - p3.y)/(p2.y - p1.y);
//    float D = (p4.x - p3.x)/(p2.x - p1.x);
//    float beta = (A - B)/(C - D);
//    return p3 + (p4 - p3)*beta;
    glm::vec3 a = p2-p1;
    glm::vec3 b = p4-p3;
    glm::vec3 c = p3-p1;
    float s = glm::dot(glm::cross(c, b), glm::cross(a, b))/glm::dot(glm::cross(a, b),glm::cross(a, b));
    glm::vec3 X = p1 + s*a;
    return X;
}

std::vector<runnel::Edge*> GradientDrainageNetwork::getEdgeOpposite(std::vector<runnel::Point *> points, runnel::Triangle*trian, glm::vec3 pto){
    std::vector<runnel::Edge*> edge_opposite;
    for(runnel::Edge* ed: trian->edges){
     //  if (!isOnEdge(points[ed->id1], points[ed->id2], pto)){
           edge_opposite.push_back(ed);
      // }
    }
    return edge_opposite;
}

bool GradientDrainageNetwork::isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto){
    bool isOn = false;
    glm::vec3 min_edge = glm::min(p1->coord, p2->coord);
    glm::vec3 max_edge = glm::max(p1->coord, p2->coord);
    std::cout << std::endl;
    std::cout << " pto menor " << p1->coord.x << " " << p1->coord.y << " " << p1->coord.z << std::endl;
    std::cout << "min " << min_edge.x << "," << min_edge.y << "," << min_edge.z << " max " << max_edge.x << "," << max_edge.y << "," << max_edge.z << std::endl;
    std::cout << "pto " << pto.x << "," << pto.y <<"," << pto.z << std::endl;
    if(glm::all(glm::lessThanEqual(min_edge, pto)) && glm::all(glm::greaterThanEqual(max_edge, pto)) ){
        std::cout << " pertenece" << std::endl;
        std::cout << std::endl;
        return true;
    }
//    float m = (p2->coord.y - p1->coord.y)/(p2->coord.x - p1->coord.x);
//    float c = p1->coord.y - m*p1->coord.x;
//    float y_expect = pto.x*m - c;
//    if (std::abs(y_expect - pto.y) < 0.01 ){
//        isOn = true;
//    }
    return isOn;
}


void GradientDrainageNetwork::calculateDrainage(std::vector<glm::vec3>& points_gradient, Terrain *ter, glm::vec3 pto){
    runnel::Triangle* trian = ter->struct_triangle[70];
    runnel::Point* p = trian->points[0];
    glm::vec3 pto_fixed = p->coord;
    std::cout << "comenzando con el gradient " << std::endl;
    calculateFinalPointTriangle(points_gradient, ter->struct_point, trian, pto_fixed);

}


bool GradientDrainageNetwork::isSamePoint(glm::vec3 pto_result, glm::vec3 pto_initial){
    if( pto_initial.x == pto_result.x && pto_initial.y == pto_result.y && pto_initial.z == pto_result.z){
        return true;
    }
    return false;
}


void GradientDrainageNetwork::getTriangleNeigthbour(runnel::Triangle* t_initial, std::vector<runnel::Triangle *>& triangle_neighbour, std::vector<runnel::Point *> points, runnel::Triangle* trian, glm::vec3 pto, glm::vec3 pto_edge, runnel::Triangle* ant_trian){
    runnel::Triangle *ant_triangle;

    for( runnel::Edge* edge_trian : trian->edges){
        if( (isSamePoint(points[edge_trian->id1]->coord, pto) || isSamePoint(points[edge_trian->id2]->coord, pto)) && (! isSamePoint(points[edge_trian->id2]->coord, pto_edge) && !isSamePoint(points[edge_trian->id2]->coord, pto_edge)) ){//es la arista que contiene el punto
            runnel::Triangle*  tri_neig = edge_trian->neighbour_triangle[0];
            if(edge_trian->neighbour_triangle[0]->ident == trian->ident){
                tri_neig = edge_trian->neighbour_triangle[1];
            }
            std::cout <<" inicial trian " << t_initial->ident << " trian_vecino " << tri_neig->ident << std::endl;
            if( tri_neig->ident == t_initial->ident){ //se llego al primer triangulo
                return;
            }
            if(tri_neig->ident == ant_trian->ident ){ //se esta dando la vuelta
                return;
            }else{
                ant_triangle = trian;
            }
            glm::vec3 other_point_edge = points[edge_trian->id1]->coord;
            if(isSamePoint(points[edge_trian->id1]->coord, pto)){
                other_point_edge = points[edge_trian->id2]->coord;
            }
            triangle_neighbour.push_back(tri_neig);
            getTriangleNeigthbour(t_initial, triangle_neighbour, points, tri_neig, pto, other_point_edge, ant_triangle);

        }
    }



}

