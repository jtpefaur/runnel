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
    cout << "yolo" << endl;
}

void NormalVectorSimilarityAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    cout << "NVS render" << endl;
}

void NormalVectorSimilarityAlgorithm::glewReady(){
    cout << "NVS glewReady" << endl;
}

QString NormalVectorSimilarityAlgorithm::getName(){
    return QString("Normal Vector Similarity");
}

QWidget* NormalVectorSimilarityAlgorithm::getConf(){
    return 0;
}
