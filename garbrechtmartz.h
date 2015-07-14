#ifndef GARBRECHTMARTZ_H
#define GARBRECHTMARTZ_H

#include <unordered_map>
#include <set>
#include "terrain.h"

class GarbrechtMartz
{
public:
    GarbrechtMartz(Terrain*);
    ~GarbrechtMartz();
    std::unordered_map<int,int> gradientTowardsLowerTerrain(std::set<int>, std::set<int>);
    std::unordered_map<int,int> gradientAwayFromHigherTerrain(std::set<int>, std::set<int>);

private:
    Terrain* ter;
    double elevationIncrement;
};

#endif // GARBRECHTMARTZ_H
