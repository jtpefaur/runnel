#include <iostream>
#include "normalvectorsimilarityalgorithm.h"

using namespace std;

NormalVectorSimilarityAlgorithm::NormalVectorSimilarityAlgorithm():
    FluvialTerraceAlgorithm()
{
    ter = 0;
    baseTriangle = 0;
    clickedPoint = glm::vec3(0.0f,0.0f,0.0f);
    shader = 0;

    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));
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
    this->clickedPoint = point;
    this->ter = terr;

    terraceVertices.clear();

    baseTriangle = this->ter->getClosestTriangle(clickedPoint);
    baseTriangle->calculateNormalVector();

    float angle;

    for(runnel::Triangle* tri : this->ter->struct_triangle){
        tri->calculateNormalVector();
        angle = glm::acos(glm::dot(baseTriangle->normal, tri->normal) /
                          glm::length(baseTriangle->normal)*glm::length(tri->normal));
        if (angle <= angleThreshold) {
            for(runnel::Point* p : tri->points) {
                terraceVertices.push_back(p->coord);
            }
        }
    }

    shader->fillPositionBuffer(terraceVertices);
}

void NormalVectorSimilarityAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    angleThreshold = glm::radians(conf.getAngleThreshold());
    cout << "AngleThreshold updated to value (degrees, radians): "
         << "("
         << conf.getAngleThreshold()
         << ", "
         << angleThreshold
         << ")"
         << endl;
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
    return &conf;
}

void NormalVectorSimilarityAlgorithm::changeAttr(){
    this->run(clickedPoint,ter);
    emit reload();
}
