#include <iostream>
#include "garbrechtmartz.h"

GarbrechtMartz::GarbrechtMartz(Terrain* ter)
{
    //this->ter = ter;
    elevationIncrement = 2.0f;
    test();
}

GarbrechtMartz::~GarbrechtMartz()
{   
    delete this->ter;
}

void GarbrechtMartz::run(std::set<int> flatIds)
{
   auto gradient = combinedGradient(gradientTowardsLowerTerrain(flatIds),
                                    gradientAwayFromHigherTerrain(flatIds));
   performIncrements(gradient);
}

void GarbrechtMartz::test()
{
    ter = new Terrain();
    ter->width = 7;
    ter->height = 7;
    std::vector<runnel::Point*> v;

    for (int i = 0; i < 7*7; ++i) {
        runnel::Point* p = new runnel::Point(glm::vec3(0,0,9),i);
        v.push_back(p);
    }

    std::set<int> flatIds;

    for (runnel::Point* p : v) {
        int id = p->ident;
        if ((id >= 8 && id <= 12) ||
                (id >= 15 && id <= 19) ||
                (id >= 22 && id <= 26) ||
                (id >= 29 && id <= 33) ||
                (id >= 36 && id <= 40)) {
            p->coord.z = 6;
            flatIds.insert(id);
        } else if (id == 14 || id == 21 || id == 34 || id == 41 || id == 47 || id == 48) {
            p->coord.z = 8;
        } else if (id == 28 || id == 35 || id == 42 || id == 43 || id == 45 || id == 46) {
            p->coord.z = 7;
        } else if (id == 44) {
            p->coord.z = 5;
        }

        ter->addPoint(p);
    }

    run(flatIds);

    for (runnel::Point* p : ter->struct_point) {
        std::cout << p->coord.z << " ";
        if ((p->ident+1)%7 == 0) {
            std::cout << std::endl;
        }
    }

    std::cout << "Fin test C:" << std::endl;
}

std::unordered_map<int,int> GarbrechtMartz::gradientTowardsLowerTerrain(std::set<int> flatIds)
{
    int width = ter->width;
    std::unordered_map<int,int> idIncrementMap;
    std::set<int> flatIdsMarkedForDeletion;
    std::set<int> idsMarkedForDownslopeGradient;
    std::set<int> downslopeGradientIds;

    while (!flatIds.empty()) {
        for (int id : flatIds) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int neighborIndex = id + j*width + i;
                    if (neighborIndex < 0 ||
                            neighborIndex == id ||
                            neighborIndex > ter->struct_point.size()-1) {
                        continue;
                    }
                    if ((downslopeGradientIds.find(neighborIndex) !=
                         downslopeGradientIds.end()) ||
                            (ter->struct_point[id]->coord.z >
                             ter->struct_point[neighborIndex]->coord.z)) {
                        // Point 'id' has a downslope gradient.
                        idsMarkedForDownslopeGradient.insert(id);
                        flatIdsMarkedForDeletion.insert(id);
                    }
                }
            }
        }

        for (int id : flatIdsMarkedForDeletion) {
            flatIds.erase(id);
        }

        flatIdsMarkedForDeletion.clear();

        for (int id : idsMarkedForDownslopeGradient) {
            downslopeGradientIds.insert(id);
        }

        idsMarkedForDownslopeGradient.clear();

        for (int id : flatIds) {
            /* Increase elevation of remaining flat-belonging points
             * without a downslope gradient. */
            idIncrementMap[id]++;
        }
    }
/*
    std::cout << "downslope ids and increments" << std::endl;
    for (auto &entry : idIncrementMap) {
        std::cout << entry.first << ":" << entry.second << " ";
    }
    std::cout << std::endl;
*/
    return idIncrementMap;
}

std::unordered_map<int, int> GarbrechtMartz::gradientAwayFromHigherTerrain(std::set<int> flatIds)
{
    int width = ter->width;
    std::unordered_map<int,int> idIncrementMap;
    std::set<int> flatIdsMarkedForDeletion;
    std::set<int> idsMarkedForUpslopeGradient;
    std::set<int> upslopeGradientIds;

    while (!flatIds.empty()) {
        for (int id : flatIds) {
            bool adjacentToHigherTerrain = false;
            bool adjacentToLowerTerrain = false;

            for (int i = -1; i <= 1 && !adjacentToLowerTerrain; ++i) {
                for (int j = -1; j <= 1 && !adjacentToLowerTerrain; ++j) {
                    int neighborIndex = id + j*width + i;
                    if (neighborIndex < 0 ||
                            neighborIndex == id ||
                            neighborIndex > ter->struct_point.size()-1) {
                        continue;
                    }
                    if (ter->struct_point[id]->coord.z >
                            ter->struct_point[neighborIndex]->coord.z) {
                        // Point 'id' has a downslope gradient. Do not increment.
                        adjacentToLowerTerrain = true;
                        flatIdsMarkedForDeletion.insert(id);
                    } else if ((upslopeGradientIds.find(neighborIndex) !=
                                upslopeGradientIds.end()) ||
                               (ter->struct_point[id]->coord.z <
                                ter->struct_point[neighborIndex]->coord.z)) {
                        // Point 'id' has an upslope gradient.
                        adjacentToHigherTerrain = true;
                    }
                }
            }

            if (adjacentToHigherTerrain && !adjacentToLowerTerrain) {
                idsMarkedForUpslopeGradient.insert(id);
                flatIdsMarkedForDeletion.insert(id);
            }
        }

        for (int id : flatIdsMarkedForDeletion) {
            flatIds.erase(id);
        }

        flatIdsMarkedForDeletion.clear();

        for (int id : idsMarkedForUpslopeGradient) {
            upslopeGradientIds.insert(id);
        }

        idsMarkedForUpslopeGradient.clear();

        for (int id : upslopeGradientIds) {
            idIncrementMap[id]++;
        }
    }
/*
    std::cout << "upslope ids and increments" << std::endl;
    for (auto &entry : idIncrementMap) {
        std::cout << entry.first << ":" << entry.second << " ";
    }
    std::cout << std::endl;
*/
    return idIncrementMap;
}

std::pair<std::unordered_map<int, int>,std::set<int>> GarbrechtMartz::combinedGradient(std::unordered_map<int,int> towardsLowerMap, std::unordered_map<int,int> awayFromHigherMap)
{
    int width = ter->width;
    std::pair<std::unordered_map<int,int>, std::set<int>> combinedGradientAndCanceledIds;
    std::set<int> canceledIncrementIds;

    // Copy towardsLowerMap to new map.
    std::unordered_map<int,int> combinedIdIncrementMap(towardsLowerMap);

    // Add awayFromHigherMap contents to the new map.
    for (auto &entry : awayFromHigherMap) {
        int id = entry.first;
        int incrementCount = entry.second;
        combinedIdIncrementMap[id] += incrementCount;
    }

    // Check if gradients on both maps cancel each other.
    for (auto &entry : combinedIdIncrementMap) {
        int id = entry.first;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int neighborIndex = id + j*width + i;
                if (neighborIndex < 0 ||
                        neighborIndex == id ||
                        neighborIndex > ter->struct_point.size()-1) {
                    continue;
                }
                if (towardsLowerMap[id] == awayFromHigherMap[neighborIndex] &&
                        awayFromHigherMap[id] == towardsLowerMap[neighborIndex] &&
                        canceledIncrementIds.find(neighborIndex) ==
                         canceledIncrementIds.end()) {
                    // The gradients cancel each other. Half-increment required.
                    canceledIncrementIds.insert(id);
                }
            }
        }
    }

    combinedGradientAndCanceledIds.first = combinedIdIncrementMap;
    combinedGradientAndCanceledIds.second = canceledIncrementIds;

    /*for (auto &entry : combinedIdIncrementMap) {
        std::cout << entry.first << ":" << entry.second << std::endl;
    }*/

    /*std::cout << "canceled ids: ";
    for (int id : canceledIncrementIds) {
        std::cout << id << " ";
    }
    std::cout << std::endl;*/

    return combinedGradientAndCanceledIds;
}

void GarbrechtMartz::performIncrements(std::pair<std::unordered_map<int,int>, std::set<int>> combinedGradientAndCanceledIds)
{
    std::unordered_map<int,int> combinedGradient = combinedGradientAndCanceledIds.first;
    std::set<int> canceledIncrementIds = combinedGradientAndCanceledIds.second;

    for (auto &entry : combinedGradient) {
        ter->struct_point[entry.first]->coord.z += entry.second*elevationIncrement;
    }

    for (int id : canceledIncrementIds) {
        ter->struct_point[id]->coord.z += elevationIncrement/2.0f;
    }
}

