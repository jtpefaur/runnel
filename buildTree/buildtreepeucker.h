#ifndef BUILDTREEPEUCKER_H
#define BUILDTREEPEUCKER_H

#include "primitives/point.h"
#include "terrain.h"
#include "patternsAlgorithms/arbol.h"

class BuildTreePeucker
{
    public:
        BuildTreePeucker(std::vector<runnel::Point*>& points, Terrain* ter);
        std::vector<runnel::Point*> points_order;
        std::unordered_map<int, int> point_counter_peucker;
        Terrain* t;
        void createTree(arbol* ar);
        std::vector<arbol*> reviewPoints();
};

#endif // BUILDTREEPEUCKER_H
