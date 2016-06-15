#include "builderterrain.h"
#include <QProcess>
#include <iostream>
#include "primitives/triangle.h"
#include <unordered_map>
#include <unordered_set>
#include "lib/glm/gtc/constants.hpp"

BuilderTerrain::BuilderTerrain()
{
}


void BuilderTerrain::runTriangulation(Terrain* ter){
    std::cout << "Creando estructura con la triangulacion" << std::endl;
    std::unordered_map< int, std::unordered_map<int, runnel::Edge*> > edges_neigh;
    int triangle_count = 0;
    float areaSum = 0;
    float minArea = std::numeric_limits<float>::infinity();
    for(int h = 0; h<ter->height-1;h++){
        for(int w = 0; w<ter->width-1;w++){
            int topleft = h*ter->width+w;
            int p11 = topleft;
            int p12 = topleft+1;
            int p21 = topleft+ter->width;
            int p22 = topleft+ter->width+1;

            runnel::Triangle *trian = new runnel::Triangle(triangle_count);
            this->buildStruct(ter, trian, p21, p12, p11);

            this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p21, p12);
            this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p12, p11);
            this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p11, p21);
            ter->addTriangle(trian);

            float triangleArea = trian->getArea();
            areaSum += triangleArea;
            minArea = std::min(minArea, triangleArea);


            triangle_count++;
            trian = new runnel::Triangle(triangle_count);
            this->buildStruct(ter, trian, p22, p12, p21);


            this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p22, p12);
            this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p12, p21);
            this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p21, p22);
            ter->addTriangle(trian);

            triangleArea = trian->getArea();
            areaSum += triangleArea;
            minArea = std::min(minArea, triangleArea);
            triangle_count++;
        }
    }
    ter->trianglesAverageArea = areaSum/ter->struct_triangle.size();
    ter->trianglesMinArea = minArea;

}


}

void BuilderTerrain::buildStruct(Terrain* ter, runnel::Triangle* trian, int pto1, int pto2, int pto3){
    glm::vec3 normal = this->calculateNormal(ter->struct_point[pto1],ter->struct_point[pto2], ter->struct_point[pto3]);
    if(normal.z < 0){
        normal = -normal;
        int aux = pto3;
        pto3 = pto2;
        pto2 = aux;
    }
    trian->addGroupPoints(ter->struct_point[pto1],ter->struct_point[pto2], ter->struct_point[pto3] );
    trian->normal = normal;
    trian->calculateIncentroPoint();
}

void BuilderTerrain::buildNeighbourhoodByEdges(Terrain* ter, runnel::Triangle* trian, std::unordered_map< int, std::unordered_map<int, runnel::Edge*> > & edges_neigh, int pto0, int pto1){
    int e1_min = std::min(pto0, pto1);
    int e1_max = std::max(pto0, pto1);
    runnel::Edge* ed1 = new runnel::Edge(ter->struct_point[pto0], ter->struct_point[pto1]);
    ed1->addTriangle(trian);
    if (edges_neigh.find(e1_min) == edges_neigh.end()){ //no ta
        std::unordered_map<int,runnel::Edge * > aux_min;
        aux_min[e1_max] = ed1;
        edges_neigh[e1_min] = aux_min;
        ter->addEdge(ed1);
    }else{
        if(edges_neigh[e1_min].find(e1_max) ==  edges_neigh[e1_min].end()){//no ta en el segundo
            edges_neigh[e1_min][e1_max] = ed1;
            ter->addEdge(ed1);
        }else{//si ta
            delete ed1;
            ed1 = edges_neigh[e1_min][e1_max];
            ed1->addTriangle(trian);
            edges_neigh[e1_min][e1_max] = ed1;
        }
    }
    trian->addEdge(ed1);


}

glm::vec3 BuilderTerrain::calculateNormal(runnel::Point* p1, runnel::Point* p2, runnel::Point* p3){
    glm::vec3 vector_a = p2->coord - p1->coord;
    glm::vec3 vector_b = p3->coord - p1->coord;
    glm::vec3 value_normal = glm::normalize(glm::cross(vector_a, vector_b));
    return value_normal;
}

