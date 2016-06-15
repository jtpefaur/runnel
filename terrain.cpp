#include "terrain.h"
#include <limits>
#include "iostream"

#define PI 3.14159265

Terrain::Terrain():
    matrix_esc()
{
    float minimo = -std::numeric_limits<float>::max();
    float maximo = std::numeric_limits<float>::max();
    max_bounding = minimo*glm::vec3(1.0f,1.0f,1.0f);
    min_bounding = maximo*glm::vec3(1.0f,1.0f,1.0f);
    sigma = glm::vec3(0.0f,0.0f,0.0f);
    media = glm::vec3(0.0f,0.0f,0.0f);
    signalPaintGoogle = false;
    lat0 = 0;
    lng0 = 0;
    ratio = 1;

}

void Terrain::addPoint(runnel::Point* p){
    struct_point.push_back(p);
}
void Terrain::addTriangle(runnel::Triangle* t){
    struct_triangle.push_back(t);
}

std::vector<runnel::Point*> Terrain::getPoints(){
    return struct_point;
}

void Terrain::normalize()
{
    sigma = max_bounding - min_bounding;
    media = 0.5f*sigma + min_bounding;
}

glm::vec3 Terrain::getMedia()
{
    return media;
}

glm::vec3 Terrain::getSigma()
{
    return sigma;
}

std::vector<glm::vec3> Terrain::getVectorPoints(){
    std::vector<glm::vec3> points_triangle;
    for (runnel::Triangle* tri : struct_triangle){
        points_triangle.push_back(tri->points[0]->coord);
        points_triangle.push_back(tri->points[1]->coord);
        points_triangle.push_back(tri->points[2]->coord);
    }
    return points_triangle;
}

std::vector<glm::vec3> Terrain::getVectorNormals(){
    std::vector<glm::vec3> normals_triangle;
    for (runnel::Triangle* tri : struct_triangle){
        normals_triangle.push_back( tri->incentro);
        normals_triangle.push_back(tri->normal/100.0f + tri->incentro);
    }
    return normals_triangle;
}

void Terrain::setMapPixel(int ancho_pix, int largo_pix){
    // calcular la matrix magica
    ratio = 1.0f*ancho_pix/largo_pix;
    std::cout << "Ratio: "<<ratio<<std::endl;
    matrix_esc[1][1] = ratio;
}

void Terrain::setRatio(){
    matrix_esc[1][1] = ratio;
}

float Terrain::minumum(std::vector<runnel::Triangle *> list_triangle, runnel::Triangle* trian){
    float menor = std::numeric_limits<float>::max();
    for(runnel::Triangle* tin: list_triangle){
        float angle = glm::acos(glm::dot(tin->normal, trian->normal))*180.0 / PI;
        if (angle == angle){
            if(tin != trian){
                menor = std::min(menor, angle);
            }
        }
    }
    if (menor != menor){
        menor = 1;
    }
    return menor;
}

std::vector<glm::vec3> Terrain::calculateNeighbourByEdges(){
    std::vector<glm::vec3> angles;
    for(runnel::Triangle* t : struct_triangle){
        glm::vec3 value;
        for (runnel::Edge* e : t->edges){

            float angulo_diedro = e->calculateAngleDiedro();
            if(t->points[0] == e->point1 && t->points[1] == e->point2 ){
                value.x = angulo_diedro;
            }
            if(t->points[1] == e->point1 && t->points[0] == e->point2 ){
                value.x = angulo_diedro;
            }
            if(t->points[2] == e->point1 && t->points[1] == e->point2 ){
                value.y = angulo_diedro;
            }
            if(t->points[1] == e->point1 && t->points[2] == e->point2 ){
                value.y = angulo_diedro;
            }
            if(t->points[2] == e->point1 && t->points[0] == e->point2 ){
                value.z = angulo_diedro;
            }
            if(t->points[0] == e->point1 && t->points[2] == e->point2 ){
                value.z = angulo_diedro;
            }

        }
        angles.push_back(value);
        angles.push_back(value);
        angles.push_back(value);
    }

    return angles;

}

std::vector<glm::vec3> Terrain::calculateHeightArray(){
    std::vector<glm::vec3> heig;
    for(runnel::Triangle* t : struct_triangle){
        glm::vec3 triangleHeight = t->calculateHeight();
        heig.push_back(glm::vec3(0, triangleHeight.y, 0));
        heig.push_back(glm::vec3(0, 0, triangleHeight.z));
        heig.push_back(glm::vec3(triangleHeight.x, 0, 0));
    }
    return heig;
}
void Terrain::setBoundingBox(glm::vec3 coords)
{
    max_bounding = glm::max(max_bounding,coords);
    min_bounding = glm::min(min_bounding,coords);
}



void Terrain::addEdge(runnel::Edge* ed){
    struct_edge.push_back(ed);
}

std::vector< glm::vec3 > Terrain::getCoordinateAxis(){
    std::vector< glm::vec3 > position_coordinates;
    std::cout << min_bounding.x << " " << min_bounding.y << " "<< min_bounding.z << " "<< std::endl;
    std::cout << max_bounding.x << " " << max_bounding.y << " "<< max_bounding.z << " "<< std::endl;
    glm::vec3 diff = (max_bounding - min_bounding)*0.1f;
    float size = std::max(diff.x, diff.y);

    glm::vec3 init_base = min_bounding;
    init_base.y = max_bounding.y;
    position_coordinates.push_back(init_base);
    position_coordinates.push_back(init_base + glm::vec3(1,0,0)*size);
    position_coordinates.push_back(init_base);
    position_coordinates.push_back(init_base - glm::vec3(0,1,0)*size);
    position_coordinates.push_back(init_base);
    position_coordinates.push_back(init_base + glm::vec3(0,0,1)*size);
    return position_coordinates;
}

runnel::Triangle* Terrain::getClosestTriangle(glm::vec3 point){
    float min_distance = glm::distance(struct_triangle[0]->incentro, point);
    runnel::Triangle* closest_triangle = struct_triangle[0];
    for(runnel::Triangle* t : struct_triangle){
        float distance = glm::distance(t->incentro, point);
        if( distance < min_distance ){
            closest_triangle = t;
            min_distance = distance;
        }
    }
    return closest_triangle;
}

bool Terrain::isBorderPoint(runnel::Point* point){
    int pointId = point->ident;
    if(pointId < width || pointId%width == 0 || (pointId%width) == (width-1) || pointId >= width*(height-1)) {
        return true;
    }
    return false;
}

bool Terrain::isBorderEdge(runnel::Edge* edge){
    if(isBorderPoint(edge->point1) && isBorderPoint(edge->point2)) {
        return true;
    }
    return false;
}
