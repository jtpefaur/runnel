#include <iostream>
#include "normalvectorsimilarityalgorithm.h"

using namespace std;

NormalVectorSimilarityAlgorithm::NormalVectorSimilarityAlgorithm():
    FluvialTerraceAlgorithm()
{
    ter = 0;
    baseTriangle = 0;
    shader = 0;
}

NormalVectorSimilarityAlgorithm::~NormalVectorSimilarityAlgorithm()
{
    if (shader) {
        delete shader;
    }
}

void NormalVectorSimilarityAlgorithm::run(glm::vec3 point, Terrain *terr){
    cout << "Running NormalVectorSimilarityAlgorithm with point "
         << point.x
         << " "
         << point.y
         << " "
         << point.z
         << endl;
    this->ter = terr;

    terraceVertices.clear();

    baseTriangle = this->ter->getClosestTriangle(point);
    baseTriangle->calculateNormalVector();

    //std::vector<int> terraceIds;
    float angle;
    //TODO: Convert user-given threshold to radians
    const float threshold = glm::radians(20.0f);

    for(runnel::Triangle* tri : this->ter->struct_triangle){
        tri->calculateNormalVector();
        angle = glm::acos(glm::dot(baseTriangle->normal, tri->normal) /
                          glm::length(baseTriangle->normal)*glm::length(tri->normal));
        if (angle <= threshold) {
            for(runnel::Point* p : tri->points) {
                terraceVertices.push_back(p->coord);
            }
            //terraceIds.push_back(tri->ident);
            //cout << tri->ident << endl;
        }
    }

    shader->fillPositionBuffer(terraceVertices);
}

void NormalVectorSimilarityAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if (shader){
        cout << "Rendering NVS with shader buffer size " << shader->data_buffer_size << endl;
        shader->render(matrix, exag_z, color);
    }
}

void NormalVectorSimilarityAlgorithm::glewReady(){
    shader = new ShaderNormalVectorSimilarity();
}

QString NormalVectorSimilarityAlgorithm::getName(){
    return QString("Normal Vector Similarity");
}

QWidget* NormalVectorSimilarityAlgorithm::getConf(){
    return 0;
}
