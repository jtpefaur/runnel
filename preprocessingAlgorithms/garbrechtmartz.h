#ifndef GARBRECHTMARTZ_H
#define GARBRECHTMARTZ_H

#include <set>
#include <unordered_map>
#include "terrain.h"

class GarbrechtMartz
{
public:
    GarbrechtMartz();
    ~GarbrechtMartz();
    void run(Terrain* ter);
    void changeIncrementation(double value);

private:
    Terrain* ter;
    double elevationIncrement;
    std::set<int> extractFlatIds();
    std::unordered_map<int,int> gradientTowardsLowerTerrain(std::set<int>);
    std::unordered_map<int,int> gradientAwayFromHigherTerrain(std::set<int>);
    std::unordered_map<int,int> combineGradients(std::unordered_map<int, int> gradientTowardsLowerTerrain,
                                                 std::unordered_map<int, int> gradientAwayFromHigherTerrain);
    std::set<int> findHalfIncrementIds(std::unordered_map<int,int> combinedGradient,
                                       std::unordered_map<int,int> towardsLower,
                                       std::unordered_map<int,int> awayFromHigher);
    void applyIncrements(std::unordered_map<int,int> combinedGradient,
                         std::set<int> findHalfIncrementIds);
    bool neighborIndexIsOutOfRange(int id, int neighborId);
};

#endif // GARBRECHTMARTZ_H
