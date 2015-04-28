#include "nonefluvialterracealgorithm.h"

NoneFluvialTerraceAlgorithm::NoneFluvialTerraceAlgorithm():
    FluvialTerraceAlgorithm()
{

}

NoneFluvialTerraceAlgorithm::~NoneFluvialTerraceAlgorithm()
{

}

void NoneFluvialTerraceAlgorithm::run(glm::vec3, Terrain *)
{

}

void NoneFluvialTerraceAlgorithm::render(glm::mat4, float, glm::vec3)
{

}

void NoneFluvialTerraceAlgorithm::glewReady()
{

}

QString NoneFluvialTerraceAlgorithm::getName()
{
    return QString("Select Algorithm");
}

QWidget *NoneFluvialTerraceAlgorithm::getConf()
{
    return 0;
}

