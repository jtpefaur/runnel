#include "diedralangledrainage.h"
#include "CL/cl.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

extern "C"
{
    #include "openclutils.h"
}

DiedralAngleDrainage::DiedralAngleDrainage(){
    QObject::connect(&conf, SIGNAL(changeAttr()), this, SLOT(changeAttr()));

}

DiedralAngleDrainage::~DiedralAngleDrainage(){
    if( shader ){
        delete shader;
    }
}

void DiedralAngleDrainage::run(Terrain *ter){
    terr = ter;
    std::vector<glm::vec3> angle_value_edge = ter->calculateNeightbourByEdges();
    std::vector<glm::vec3> height = ter->calculateHeigtArray();
    position_terrain = ter->getVectorPoints();
    shader->fillPositionBuffer(position_terrain, angle_value_edge, height );
}

void DiedralAngleDrainage::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    float min_angle = conf.getMinAngle();
    float max_angle = conf.getMaxAngle();
    float width_line = conf.getLineWidth();
    glm::vec3 color_min= glm::vec3(0,1,0);
    glm::vec3 color_max = glm::vec3(1,0,0);;
    if ( shader ){
        shader->render(matrix, exag_z, min_angle, max_angle, width_line, color_min, color_max);
    }
}

void DiedralAngleDrainage::glewReady(){
    shader = new ShaderAngleEdge();
}

QString DiedralAngleDrainage::getName(){
    return QString("Angle Diedral");
}

QWidget* DiedralAngleDrainage::getConf(){
    return &conf;
}

void DiedralAngleDrainage::changeAttr(){
    this->run(terr);
    emit reload();
}

std::vector<glm::vec3> DiedralAngleDrainage::getPathTree(){
    return position_terrain;
}
