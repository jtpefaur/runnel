#include "buildtreerwflood.h"

BuildTreeRWFlood::BuildTreeRWFlood()
{
    ter = 0;
    shader = 0;
}

BuildTreeRWFlood::~BuildTreeRWFlood()
{
    if (shader) {
        delete shader;
    }
}

std::vector<arbol *> BuildTreeRWFlood::run(Terrain *ter)
{
    this->ter = ter;
    networkTrees.clear();

    // TODO: Compute networkTrees

    int orderThreshold = 1; // TODO: Receive threshold as user input.
    std::vector<std::string> types;
    shader->fillPositionBuffer(networkTrees, types, orderThreshold);
    return networkTrees;
}

void BuildTreeRWFlood::render(glm::mat4 matrix, float exag_z)
{
    if (shader) {
        shader->render(matrix, exag_z);
    }
}

std::vector<arbol *> BuildTreeRWFlood::getData()
{
    return networkTrees;
}

void BuildTreeRWFlood::glewReady()
{
    shader = new ShaderPatron();
}

QString BuildTreeRWFlood::getName()
{
    return QString("RWFlood");
}

QWidget *BuildTreeRWFlood::getConf()
{
    return 0;
}

std::vector<glm::vec3> BuildTreeRWFlood::getPathTree()
{
    std::vector<runnel::Point*> pointPath;
    pointPath.clear();
    for (arbol* tree : networkTrees) {
        std::vector<runnel::Point*> points;
        tree->getArbolEdges(points);
        pointPath.insert(pointPath.end(), points.begin(), points.end());
        points.clear();
    }
    std::vector<glm::vec3> coordinatePath;
    for (runnel::Point* point : pointPath) {
        coordinatePath.push_back(point->coord);
    }
    return coordinatePath;
}

