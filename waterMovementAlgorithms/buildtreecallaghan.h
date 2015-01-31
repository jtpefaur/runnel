#ifndef BUILDTREECALLAGHAN_H
#define BUILDTREECALLAGHAN_H
#include <unordered_map>
#include "primitives/point.h"
#include "patternsAlgorithms/arbol.h"

class buildTreeCallaghan
{
    public:
        std::vector<runnel::Point*> points_order;
        float max_water;
        buildTreeCallaghan(std::vector<runnel::Point*>& points, float water_max);
        std::unordered_map<int, int> point_counter;
        bool isAnyFatherUsed(runnel::Point* pto);
        void createTree(arbol* parent);
        std::vector<arbol*> reviewPoints();
};

#endif // BUILDTREECALLAGHAN_H
