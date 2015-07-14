#ifndef GARBRECHTMARTZ_H
#define GARBRECHTMARTZ_H

#include <set>
#include "terrain.h"

class GarbrechtMartz
{
public:
    GarbrechtMartz(Terrain*);
    ~GarbrechtMartz();
    void gradientTowardsLowerTerrain(std::set<int>, std::set<int>);
    void gradientAwayFromHigherTerrain(std::set<int>, std::set<int>);

private:
    Terrain* ter;
    double elevationIncrement;
};

#endif // GARBRECHTMARTZ_H
