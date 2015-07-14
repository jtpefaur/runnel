#include "garbrechtmartz.h"

GarbrechtMartz::GarbrechtMartz(Terrain* ter)
{
    this->ter = ter;
    elevationIncrement = 2;
}

GarbrechtMartz::~GarbrechtMartz()
{

}

std::unordered_map<int,int> GarbrechtMartz::gradientTowardsLowerTerrain(std::set<int> flatIds,
                                                 std::set<int> downslopeGradientIds)
{
    int width = ter->width;
    std::unordered_map<int,int> idIncrementMap;
    std::set<int> flatIdsMarkedForDeletion;

    while (!flatIds.empty()) {
        for (int id : flatIds) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int neighborIndex = id + j*width + i;
                    if ((downslopeGradientIds.find(neighborIndex) !=
                         downslopeGradientIds.end()) ||
                            (ter->struct_point[id]->coord.z >
                             ter->struct_point[neighborIndex]->coord.z)) {
                        // Point 'id' has a downslope gradient.
                        downslopeGradientIds.insert(id);
                        flatIdsMarkedForDeletion.insert(id);
                    }
                }
            }
        }

        for (int id : flatIdsMarkedForDeletion) {
            flatIds.erase(id);
        }

        flatIdsMarkedForDeletion.clear();

        for (int id : flatIds) {
            /* Increase elevation of remaining flat-belonging points
        without a downslope gradient. */
            idIncrementMap[id]++;
        }
    }

    return idIncrementMap;
}

std::unordered_map<int, int> GarbrechtMartz::gradientAwayFromHigherTerrain(std::set<int> flatIds,
                                                   std::set<int> upslopeGradientIds)
{
    int width = ter->width;
    std::unordered_map<int,int> idIncrementMap;
    std::set<int> flatIdsMarkedForDeletion;

    while (!flatIds.empty()) {
        for (int id : flatIds) {
            bool adjacentToHigherTerrain = false;
            bool adjacentToLowerTerrain = false;

            for (int i = -1; i <= 1 && !adjacentToLowerTerrain; ++i) {
                for (int j = -1; j <= 1 && !adjacentToLowerTerrain; ++j) {
                    int neighborIndex = id + j*width + i;
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
                upslopeGradientIds.insert(id);
                flatIdsMarkedForDeletion.insert(id);
            }
        }

        for (int id : flatIdsMarkedForDeletion) {
            flatIds.erase(id);
        }

        flatIdsMarkedForDeletion.clear();

        for (int id : upslopeGradientIds) {
            idIncrementMap[id]++;
        }
    }

    return idIncrementMap;
}

