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

void BuilderTerrain::runSimplifedTriangulation(Terrain* ter){

    bool thereMightBeAnEdgeLeftToCollapse = true;
    std::vector<runnel::Triangle*> excluded;

    std::cout << "Amount of triangles before simplification: " << ter->struct_triangle.size() <<  std::endl;
    std::cout << "Average triangles area: " << ter->trianglesAverageArea << std::endl;
    std::cout << "Min triangles area: " << ter->trianglesMinArea << std::endl;
    while(thereMightBeAnEdgeLeftToCollapse) {
        thereMightBeAnEdgeLeftToCollapse = false;
        for(runnel::Triangle* triangle : ter->struct_triangle) {
            //This will simplify the triangulation on the flat zones of the terrain
            if(std::find(excluded.begin(), excluded.end(), triangle) == excluded.end() && triangle->getArea() < ter->trianglesMinArea*1.02){
                if(simplifyTriangle(triangle, ter)) thereMightBeAnEdgeLeftToCollapse = true;
                else excluded.push_back(triangle);
            }
        }
    }
    std::cout << "Amount of triangles after simplification: " << ter->struct_triangle.size() <<  std::endl;
}

bool BuilderTerrain::simplifyTriangle(runnel::Triangle* triangle, Terrain* ter){
    assert(("Triangle to simplify was not part of the terrain",std::find(ter->struct_triangle.begin(), ter->struct_triangle.end(), triangle) != ter->struct_triangle.end()));
    runnel::Edge* shortestEdge = triangle->getShortestEdge();
    bool edgeWasCollapsed = edgeCollapse(shortestEdge, ter);
    return edgeWasCollapsed;
}

bool BuilderTerrain::edgeCollapse(runnel::Edge* edge, Terrain* ter){

    if(ter->isBorderEdge(edge)) {
        return false;
    }

    runnel::Point* pointToDelete = edge->point1;
    runnel::Point* pointToKeep = edge->point2;

    if(ter->isBorderPoint(pointToDelete)) {
        runnel::Point* aux = pointToKeep;
        pointToKeep = pointToDelete;
        pointToDelete = aux;
    }

    bool pointWasDeleted = pointDeletion(ter, edge, pointToDelete);
    return pointWasDeleted;
}

int BuilderTerrain::getIntersectingNeighbourPoints(runnel::Point* point1, runnel::Point* point2, Terrain* ter) {

    std::unordered_set<runnel::Point*> neighbourPointsToPoint1;
    std::unordered_set<runnel::Point*> neighbourPointsToPoint2;

    std::unordered_set<runnel::Triangle*> trianglesContainingPoint1 = ter->trianglesContainingPoint[point1];
    assert(("There are no triangles containing point to keep", trianglesContainingPoint1.size() !=0));
    std::unordered_set<runnel::Triangle*> trianglesContainingPoint2 = ter->trianglesContainingPoint[point2];
    assert(("There are no triangles containing point to delete", trianglesContainingPoint2.size() !=0));

    for(runnel::Triangle* triangle : trianglesContainingPoint1) {
        neighbourPointsToPoint1.insert(triangle->points[0]);
        neighbourPointsToPoint1.insert(triangle->points[1]);
        neighbourPointsToPoint1.insert(triangle->points[2]);
    }


    for(runnel::Triangle* triangle : trianglesContainingPoint2) {
        neighbourPointsToPoint2.insert(triangle->points[0]);
        neighbourPointsToPoint2.insert(triangle->points[1]);
        neighbourPointsToPoint2.insert(triangle->points[2]);
    }

    int intersectingPoints = 0;
    for(runnel::Point* point : neighbourPointsToPoint1) {

        if(neighbourPointsToPoint2.find(point) != neighbourPointsToPoint2.end()) {
            intersectingPoints++;
        }
    }
    return intersectingPoints;
}

void BuilderTerrain::removeEdgeFromTerrain(Terrain* ter, runnel::Edge* edge, runnel::Point* pointToKeep, runnel::Point* pointToDelete) {
    assert(("Point to delete is not part of the edge to delete", edge->point1 == pointToDelete || edge->point2 == pointToDelete));
    assert(("Point to delete is not part of the edge to delete", edge->point1 == pointToKeep || edge->point2 == pointToKeep));
    runnel::Triangle* triangleToDelete1 = edge->neighbour_triangle[0];
    runnel::Triangle* triangleToDelete2 = edge->neighbour_triangle[1];

    runnel::Edge* edgeToDelete1 = getEdgeOpposedToPoint(triangleToDelete1, pointToKeep);
    runnel::Edge* edgeToDelete2 = getEdgeOpposedToPoint(triangleToDelete2, pointToKeep);

    runnel::Triangle* triangleToDelete1NeighbourToModify = getTriangleOpposedToPoint(triangleToDelete1, pointToKeep);
    runnel::Triangle* triangleToDelete2NeighbourToModify = getTriangleOpposedToPoint(triangleToDelete2, pointToKeep);

    //Delete edges from terrain
    ter->struct_edge.erase(std::remove(ter->struct_edge.begin(), ter->struct_edge.end(), edgeToDelete1));
    ter->struct_edge.erase(std::remove(ter->struct_edge.begin(), ter->struct_edge.end(), edgeToDelete2));
    ter->struct_edge.erase(std::remove(ter->struct_edge.begin(), ter->struct_edge.end(), edge));
    //Delete triangles from terrain
    ter->struct_triangle.erase(std::remove(ter->struct_triangle.begin(), ter->struct_triangle.end(), triangleToDelete1));
    ter->struct_triangle.erase(std::remove(ter->struct_triangle.begin(), ter->struct_triangle.end(), triangleToDelete2));
    ter->trianglesContainingPoint[pointToKeep].erase(triangleToDelete1);
    ter->trianglesContainingPoint[pointToKeep].erase(triangleToDelete2);

    //Update edgesToKeep neighbour triangles
    runnel::Edge* edgeToKeep1 = getEdgeOpposedToPoint(triangleToDelete1, pointToDelete);
    runnel::Edge* edgeToKeep2 = getEdgeOpposedToPoint(triangleToDelete2, pointToDelete);
    edgeToKeep1->neighbour_triangle.erase(std::remove(edgeToKeep1->neighbour_triangle.begin(), edgeToKeep1->neighbour_triangle.end(), triangleToDelete1));
    edgeToKeep2->neighbour_triangle.erase(std::remove(edgeToKeep2->neighbour_triangle.begin(), edgeToKeep2->neighbour_triangle.end(), triangleToDelete2));
    edgeToKeep1->neighbour_triangle.push_back(triangleToDelete1NeighbourToModify);
    edgeToKeep2->neighbour_triangle.push_back(triangleToDelete2NeighbourToModify);

    //Update points and edges of triangles to modify
    std::vector<runnel::Triangle*> trianglesToModify = getTrianglesToModify(edge, pointToDelete, ter);
    for(runnel::Triangle* triangle : trianglesToModify) {
        std::replace(triangle->points.begin(), triangle->points.end(), pointToDelete, pointToKeep);
        ter->trianglesContainingPoint[pointToKeep].insert(triangle);
        std::replace(triangle->edges.begin(), triangle->edges.end(), edgeToDelete1, edgeToKeep1);
        std::replace(triangle->edges.begin(), triangle->edges.end(), edgeToDelete2, edgeToKeep2);
        for(runnel::Edge* edge : triangle->edges) {
            if(edge->point1 == pointToDelete) edge->point1 = pointToKeep;
            if(edge->point2 == pointToDelete) edge->point2= pointToKeep;
        }
        triangle->refresh();
    }
}


std::vector<runnel::Triangle*> BuilderTerrain::getTrianglesToModify(runnel::Edge* edgeToDelete, runnel::Point* pointToDelete, Terrain* ter) {
    assert(("Point to delete is not part of the edge to delete", edgeToDelete->point1 == pointToDelete || edgeToDelete->point2 == pointToDelete));
    std::vector<runnel::Triangle*> trianglesToModify;
    runnel::Triangle* triangleToDelete1 = edgeToDelete->neighbour_triangle[0];
    runnel::Triangle* triangleToDelete2 = edgeToDelete->neighbour_triangle[1];

    std::unordered_set<runnel::Triangle*> trianglesContainingPointToDelete = ter->trianglesContainingPoint[pointToDelete];
    for(runnel::Triangle* neighbourTriangle : trianglesContainingPointToDelete) {
        if(neighbourTriangle != triangleToDelete1 && neighbourTriangle != triangleToDelete2){
            trianglesToModify.push_back(neighbourTriangle);
        }
    }
    assert(("No triangles to modify", trianglesToModify.size() != 0));
    return trianglesToModify;
}

bool BuilderTerrain::triangleWouldFlipIfModified(runnel::Point* pointToModify, runnel::Triangle* triangle, glm::vec3 pointNewCoord) {
        glm::vec3 oldNormal = triangle->normal;
        glm::vec3 pointOldCoord = pointToModify->coord;

        pointToModify->coord = pointNewCoord;
        triangle->refresh();
        glm::vec3 newNormal = triangle->normal;
        float angle = glm::acos(glm::dot(oldNormal, newNormal)/(glm::length(oldNormal)*glm::length(newNormal)));
        pointToModify->coord = pointOldCoord;
        triangle->refresh();
        if(angle > glm::half_pi<float>()) return true;
        return false;
}

bool BuilderTerrain::pointDeletion(Terrain* ter, runnel::Edge* edge, runnel::Point* pointToDelete){

    assert(("Point to delete is not part of the edge to delete", edge->point1 == pointToDelete || edge->point2 == pointToDelete));
    runnel::Point* pointToKeep = edge->point1 == pointToDelete ? edge->point2 : edge->point1;

    int intersectingPoints = getIntersectingNeighbourPoints(pointToKeep, pointToDelete, ter);
    if(intersectingPoints > 4) {
        return false;
    }

    runnel::Triangle* triangleToDelete1 = edge->neighbour_triangle[0];
    runnel::Triangle* triangleToDelete2 = edge->neighbour_triangle[1];

    runnel::Triangle* triangleToDelete1NeighbourToModify = getTriangleOpposedToPoint(triangleToDelete1, pointToKeep);
    runnel::Triangle* triangleToDelete1NeighbourToKeep = getTriangleOpposedToPoint(triangleToDelete1, pointToDelete);
    runnel::Triangle* triangleToDelete2NeighbourToModify = getTriangleOpposedToPoint(triangleToDelete2, pointToKeep);
    runnel::Triangle* triangleToDelete2NeighbourToKeep = getTriangleOpposedToPoint(triangleToDelete2, pointToDelete);
    if(areNeighbour(triangleToDelete1NeighbourToModify, triangleToDelete1NeighbourToKeep) ||
            areNeighbour(triangleToDelete2NeighbourToModify, triangleToDelete2NeighbourToKeep)) {
        //They would be the same triangle if the edge was deleted
        return false;
    }

    std::vector<runnel::Triangle*> trianglesToModify = getTrianglesToModify(edge, pointToDelete, ter);
    for(runnel::Triangle* triangle : trianglesToModify) {
        if(triangleWouldFlipIfModified(pointToDelete, triangle, pointToDelete->coord)) return false;
    }

    removeEdgeFromTerrain(ter, edge, pointToKeep, pointToDelete);
    return true;

}

runnel::Edge* BuilderTerrain::getEdgeOpposedToPoint(runnel::Triangle* triangle, runnel::Point* point){
    if(std::find(triangle->points.begin(), triangle->points.end(), point) == triangle->points.end()){
        std::cout << "Point is not on the vertex of the triangle" << std::endl;
        for(runnel::Point* p : triangle->points) {
            std::cout << "Triangle point id: " << p->ident << std::endl;
        }
        std::cout << "Point id: " << point->ident << std::endl;
    }
    assert(("Point is not one of the vertex of the triangle", std::find(triangle->points.begin(), triangle->points.end(), point) != triangle->points.end()));
    for(runnel::Edge* edge : triangle->edges) {
        if(edge->point1 != point && edge->point2 != point) {
            return edge;
        }
    }
    assert(("You should never get here",false));
    return nullptr;
}

runnel::Triangle* BuilderTerrain::getTriangleOpposedToPoint(runnel::Triangle* triangle, runnel::Point* point){
    runnel::Edge* opposedEdge = getEdgeOpposedToPoint(triangle, point);
    if(opposedEdge->neighbour_triangle.size() < 2) return nullptr;
    runnel::Triangle* opposedTriangle =
            opposedEdge->neighbour_triangle[0] == triangle ?
                opposedEdge->neighbour_triangle[1] : opposedEdge->neighbour_triangle[0];
    return opposedTriangle;
}

bool BuilderTerrain::areNeighbour(runnel::Triangle* triangle1, runnel::Triangle* triangle2){
    if(triangle1 == nullptr || triangle2 == nullptr) return false;
    for(runnel::Edge* edge : triangle1->edges) {
        if(std::find(triangle2->edges.begin(), triangle2->edges.end(), edge) != triangle2->edges.end()) return true;
    }
    return false;

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

