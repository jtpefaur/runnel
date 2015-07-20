#include "garbrechtmartz.h"

GarbrechtMartz::GarbrechtMartz(Terrain* ter)
{
    this->ter = ter;
    elevationIncrement = 2.0f;
}

GarbrechtMartz::~GarbrechtMartz()
{
}

void GarbrechtMartz::run(std::set<int> flatIds)
{
   auto gradient = combinedGradient(gradientTowardsLowerTerrain(flatIds),
                                    gradientAwayFromHigherTerrain(flatIds));
   applyIncrements(gradient);
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
                    if (neighborIndexIsOutOfRange(id, neighborIndex)) {
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
                    if (neighborIndexIsOutOfRange(id, neighborIndex)) {
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
                if (neighborIndexIsOutOfRange(id, neighborIndex)) {
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

    return combinedGradientAndCanceledIds;
}

void GarbrechtMartz::applyIncrements(std::pair<std::unordered_map<int,int>, std::set<int>> combinedGradientAndCanceledIds)
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

bool GarbrechtMartz::neighborIndexIsOutOfRange(int id, int neighborId)
{
    if (neighborId < 0 || neighborId > (int)ter->struct_point.size()-1 ||
            id == neighborId) {
        return true;
    }
    return false;
}
