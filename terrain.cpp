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
    max_value_water = 0;
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
    float ratio = 1.0f*ancho_pix/largo_pix;
    std::cout << "Ratio: "<<ratio<<std::endl;
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
void Terrain::addNeighbourPoint(std::unordered_map<int, std::vector<runnel::Triangle*> > n){
    neigh = n;
    Terrain::calculateNeightbour();
}
void Terrain::calculateNeightbour(){
    std::cout << "calculando vecinos" << std::endl;

    menor_menor = std::numeric_limits<float>::max();
    mayor_mayor = -1*std::numeric_limits<float>::max();

    for( runnel::Triangle* tri: struct_triangle){
        std::vector<float> lala;
        float a = Terrain::minumum(neigh[tri->points[0]->ident], tri);
        float b = Terrain::minumum(neigh[tri->points[1]->ident], tri);
        float c = Terrain::minumum(neigh[tri->points[2]->ident], tri);
        lala.push_back(a);
        lala.push_back(b);
        lala.push_back(c);

        menor_menor = std::min(a, menor_menor);
        menor_menor = std::min(b, menor_menor);
        menor_menor = std::min(c, menor_menor);
        mayor_mayor = std::max(a, mayor_mayor);
        mayor_mayor = std::max(b, mayor_mayor);
        mayor_mayor = std::max(c, mayor_mayor);
        tri->minimun_value_angle_point = lala;
    }
    std::cout << "mayor "<< mayor_mayor << "menor " << menor_menor << std::endl;

}

std::vector<glm::vec3> Terrain::calculateNeightbourByEdges(){
    std::vector<glm::vec3> angles;
    for(runnel::Triangle* t : struct_triangle){
        glm::vec3 value;
        for (runnel::Edge* e : t->edges){
            if(e->calculate_angulo_diendro == 0){
                e->calculateAngleDiedro();
            }
            if(t->points[0]->ident == e->id1 && t->points[1]->ident == e->id2 ){
                value.x = e->angulo_diedro;
            }
            if(t->points[1]->ident == e->id1 && t->points[0]->ident == e->id2 ){
                value.x = e->angulo_diedro;
            }
            if(t->points[2]->ident == e->id1 && t->points[1]->ident == e->id2 ){
                value.y = e->angulo_diedro;
            }
            if(t->points[1]->ident == e->id1 && t->points[2]->ident == e->id2 ){
                value.y = e->angulo_diedro;
            }
            if(t->points[2]->ident == e->id1 && t->points[0]->ident == e->id2 ){
                value.z = e->angulo_diedro;
            }
            if(t->points[0]->ident == e->id1 && t->points[2]->ident == e->id2 ){
                value.z = e->angulo_diedro;
            }

        }
        angles.push_back(value);
        angles.push_back(value);
        angles.push_back(value);
    }

    return angles;

}

std::vector<glm::vec3> Terrain::calculateHeigtArray(){
    std::vector<glm::vec3> heig;
    for(runnel::Triangle* t : struct_triangle){
        t->calculateHeight();
        heig.push_back(glm::vec3(0, t->height.y, 0));
        heig.push_back(glm::vec3(0, 0, t->height.z));
        heig.push_back(glm::vec3(t->height.x, 0, 0));
        //std::cout << "height "<< t->height.x << " " << t->height.y << " " << t->height.z << std::endl;
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


std::vector<glm::vec3> Terrain::getDrainageColor(){
    std::vector<glm::vec3> color;
    for( runnel::Edge* ed : struct_edge){

        runnel::Point* p1 = this->struct_point[ed->id1];
        runnel::Point* p2 = this->struct_point[ed->id2];
        float color_p1 = (p1->isFlagsOn(p1->PEUCKER))? 1.0f : 0.0f;
        float color_p2 = (p2->isFlagsOn(p2->PEUCKER))? 1.0f : 0.0f;
        if((color_p1==0.0f && color_p2==0.0f) ){
            color.push_back(glm::vec3(1,0,0));
            color.push_back(glm::vec3(1,0,0));
            points_edge.push_back(p1->coord);
            points_edge.push_back(p2->coord);
        }
    }
    return color;
}

std::vector<glm::vec3> Terrain::getPointsEdgeDrainage(){
    return points_edge;
}


std::vector<glm::vec3> Terrain::getGradientDirectionVector(){
    std::vector<glm::vec3> vector_gradient;

    for (runnel::Triangle* trian : this->struct_triangle){
        trian->calculateLineHorizontal();
        vector_gradient.push_back(trian->gradient[0]);
        vector_gradient_color.push_back(glm::vec3(1.0f,1.0f, 1.0f));
        vector_gradient.push_back(trian->gradient[1]);
        vector_gradient_color.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
    }
    return vector_gradient;
}

void Terrain::getMoreWaterPoint(){

    for (runnel::Edge* edge: this->struct_edge){
        float water_1 = this->struct_point[edge->id1]->water_value;
        float water_2 = this->struct_point[edge->id2]->water_value;
        float value_water =  std::max(water_1, water_2);
        max_value_water = std::max(max_value_water,value_water);
        count_water.push_back(value_water);
        count_water.push_back(value_water);
        position_water_points.push_back(this->struct_point[edge->id1]->coord);
        position_water_points.push_back(this->struct_point[edge->id2]->coord);
    }
}
