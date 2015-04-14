#include <iostream>
#include "normalvectorsimilarityalgorithm.h"

using namespace std;

NormalVectorSimilarityAlgorithm::NormalVectorSimilarityAlgorithm()
{

}

NormalVectorSimilarityAlgorithm::~NormalVectorSimilarityAlgorithm()
{

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

    baseTriangle = this->ter->getClosestTriangle(point);
    baseTriangle->calculateNormalVector();

    std::vector<int> terraceIds;
    float angle;
    //TODO: Convert user-given threshold to radians
    const float threshold = glm::radians(20.0f);

    for(runnel::Triangle* tri : this->ter->struct_triangle){
        tri->calculateNormalVector();
        angle = glm::acos(glm::dot(baseTriangle->normal, tri->normal)
                          / glm::length(baseTriangle->normal)*glm::length(tri->normal));
        if (angle <= threshold) {
            terraceIds.push_back(tri->ident);
            //cout << tri->ident << endl;
        }
    }

    //TODO: The IDs need to be stored somehow, for later rendering
}

void NormalVectorSimilarityAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    //TODO: Call the shader's rendering function
}

void NormalVectorSimilarityAlgorithm::glewReady(){
    //TODO: Initialize the shader
}

QString NormalVectorSimilarityAlgorithm::getName(){
    return QString("Normal Vector Similarity");
}

QWidget* NormalVectorSimilarityAlgorithm::getConf(){
    return 0;
}
