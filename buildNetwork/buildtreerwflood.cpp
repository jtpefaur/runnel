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

    for (auto it1 = networkTrees.begin(); it1 != networkTrees.end(); std::advance(it1, 1)) {
        for (auto it2 = (*it1)->hijos.begin(); it2 != (*it1)->hijos.end(); std::advance(it2, 1)) {
            delete (*it2);
        }
        delete (*it1);
    }
}

std::vector<arbol *> BuildTreeRWFlood::run(Terrain *ter)
{
    this->ter = ter;
    networkTrees.clear();

    buildNetworkTrees();

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

void BuildTreeRWFlood::buildNetworkTrees()
{
    std::map<int,bool> visitedPoints;

    sortPoints(ter->struct_point);

    for (runnel::Point* point : sortedPoints) {
        if (visitedPoints[point->ident]) {
            continue;
        }

        arbol* tree = new arbol(point);
        visitedPoints[point->ident] = true;
        buildTree(tree, visitedPoints);
        networkTrees.push_back(tree);
    }

    for (arbol* tree : networkTrees) {
        tree->computeNetworkStrahlerOrdering();
    }
}

void BuildTreeRWFlood::buildTree(arbol *parent, std::map<int,bool>& visitedPoints)
{
    if (parent->pto->water_parent.size() > 0) {
        for (runnel::Point* point : parent->pto->water_parent) {
            if (!visitedPoints[point->ident]) {
                visitedPoints[point->ident] = true;
                arbol* tree = new arbol(point);
                parent->hijos.push_back(tree);
                buildTree(tree, visitedPoints);
             }
        }
    }
}

void BuildTreeRWFlood::sortPoints(std::vector<runnel::Point *>& terrainPoints)
{
    sortedPoints = terrainPoints;

    std::sort(sortedPoints.begin(), sortedPoints.end(),
              [] (runnel::Point* p1, runnel::Point* p2) {
                    return p1->water_value > p2->water_value;
    });
}
