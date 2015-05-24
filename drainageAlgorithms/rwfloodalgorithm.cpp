#include "rwfloodalgorithm.h"

RWFloodAlgorithm::RWFloodAlgorithm():
    DrainageAlgorithms()
{

}

RWFloodAlgorithm::~RWFloodAlgorithm()
{

}

void RWFloodAlgorithm::run(Terrain *ter)
{

}

void RWFloodAlgorithm::glewReady()
{

}

void RWFloodAlgorithm::render(glm::mat4 matrix, float exag_z, glm::vec3 color)
{

}

QString RWFloodAlgorithm::getName()
{
    return QString("RWFlood");
}

QWidget* RWFloodAlgorithm::getConf()
{
    return 0;
}

std::vector<glm::vec3> RWFloodAlgorithm::getPathTree()
{
    std::vector<glm::vec3> path;
    return path;
}

