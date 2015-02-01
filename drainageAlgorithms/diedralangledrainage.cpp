#include "diedralangledrainage.h"

DiedralAngleDrainage::DiedralAngleDrainage(){

}

DiedralAngleDrainage::~DiedralAngleDrainage(){
    if( shader ){
        delete shader;
    }
}

void DiedralAngleDrainage::run(Terrain *ter){
    std::vector<glm::vec3> angle_value_edge = ter->calculateNeightbourByEdges();
    std::vector<glm::vec3> height = ter->calculateHeigtArray();
    std::vector<glm::vec3> position_terrain = ter->getVectorPoints();
    shader->fillPositionBuffer(position_terrain, angle_value_edge, height );
}

void DiedralAngleDrainage::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    float min_angle = 0;
    float max_angle = 0;
    float width_line = 0;
    glm::vec3 color_min= glm::vec3(0,0,0);
    glm::vec3 color_max = glm::vec3(0,0,0);
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
