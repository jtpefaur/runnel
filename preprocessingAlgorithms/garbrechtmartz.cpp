#include "garbrechtmartz.h"

GarbrechtMartz::GarbrechtMartz(Terrain* ter)
{
    this->ter = ter;
    elevationIncrement = 2.0;
}

GarbrechtMartz::~GarbrechtMartz()
{
}

void GarbrechtMartz::run()
{
    std::set<int> flatIds = extractFlatIds();
    std::unordered_map<int,int> towardsLower = gradientTowardsLowerTerrain(flatIds);
    std::unordered_map<int,int> awayFromHigher = gradientAwayFromHigherTerrain(flatIds);
    std::unordered_map<int,int> combinedGradient = combineGradients(towardsLower,
                                                                    awayFromHigher);
    std::set<int> halfIncrementIds = findHalfIncrementIds(combinedGradient,
                                                             towardsLower,
                                                             awayFromHigher);
    applyIncrements(combinedGradient, halfIncrementIds);
}

std::set<int> GarbrechtMartz::extractFlatIds()
{
    std::set<int> flatIds;
    int width = ter->width;

    for (runnel::Point* point : ter->struct_point) {
        if (point->coord.z == 0) {
            // No-data point. Do not consider as part of a flat area.
            continue;
        }

        int id = point->ident;
        bool hasGradient = false;
        std::set<int> localFlatIds;

        // We assume a 3x3 point region to be the minimal unit of a flat area.
        for (int i = -1; i <= 1 && !hasGradient; ++i) {
            for (int j = -1; j <= 1 && !hasGradient; ++j) {
                localFlatIds.insert(id);
                int neighborIndex = id + j*width + i;
                if (neighborIndexIsOutOfRange(id, neighborIndex)) {
                    continue;
                }
                if (ter->struct_point[id]->coord.z !=
                        ter->struct_point[neighborIndex]->coord.z) {
                    hasGradient = true;
                } else {
                    localFlatIds.insert(neighborIndex);
                }
            }
        }

        if (!hasGradient) {
            for (int flatId : localFlatIds) {
                flatIds.insert(flatId);
            }
        }
    }

    return flatIds;
}

std::unordered_map<int,int> GarbrechtMartz::gradientTowardsLowerTerrain(std::set<int> flatIds)
{
    int width = ter->width;
    int prevFlatIdCount = -1;
    std::unordered_map<int,int> idIncrementMap;
    std::set<int> flatIdsMarkedForDeletion;
    std::set<int> idsMarkedForDownslopeGradient;
    std::set<int> downslopeGradientIds;

    while (!flatIds.empty()) {
        if ((int)flatIds.size() == prevFlatIdCount) {
            // Remaining flat areas are isolated and cannot be resolved by this algorithm.
            break;
        }

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

        prevFlatIdCount = flatIds.size();

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
    int prevFlatIdCount = -1;
    std::unordered_map<int,int> idIncrementMap;
    std::set<int> flatIdsMarkedForDeletion;
    std::set<int> idsMarkedForUpslopeGradient;
    std::set<int> upslopeGradientIds;

    while (!flatIds.empty()) {
        if ((int)flatIds.size() == prevFlatIdCount) {
            // Remaining flat areas are isolated and cannot be resolved by this algorithm.
            break;
        }

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

        prevFlatIdCount = flatIds.size();

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

std::unordered_map<int, int> GarbrechtMartz::combineGradients(std::unordered_map<int, int> gradientTowardsLowerTerrain,
                                                              std::unordered_map<int, int> gradientAwayFromHigherTerrain)
{
    // Copy first map to new map, using the copy constructor.
    std::unordered_map<int,int> combinedGradient(gradientTowardsLowerTerrain);

    // Add second map's contents to the new map.
    for (auto &entry : gradientAwayFromHigherTerrain) {
        int id = entry.first;
        int incrementCount = entry.second;
        combinedGradient[id] += incrementCount;
    }

    return combinedGradient;
}

std::set<int> GarbrechtMartz::findHalfIncrementIds(std::unordered_map<int, int> combinedGradient,
                                                   std::unordered_map<int, int> towardsLower,
                                                   std::unordered_map<int, int> awayFromHigher)
{
    int width = ter->width;
    std::set<int> halfIncrementIds;

    // Check if gradients on both maps cancel each other.
    for (auto &entry : combinedGradient) {
        int id = entry.first;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int neighborIndex = id + j*width + i;
                if (neighborIndexIsOutOfRange(id, neighborIndex)) {
                    continue;
                }
                if (towardsLower[id] == awayFromHigher[neighborIndex] &&
                        awayFromHigher[id] == towardsLower[neighborIndex] &&
                        halfIncrementIds.find(neighborIndex) ==
                        halfIncrementIds.end()) {
                    // The gradients cancel each other. Extra half-increment required.
                    halfIncrementIds.insert(id);
                }
            }
        }
    }

    return halfIncrementIds;
}

void GarbrechtMartz::applyIncrements(std::unordered_map<int,int> combinedGradient,
                                     std::set<int> halfIncrementIds)
{
    for (auto &entry : combinedGradient) {
        ter->struct_point[entry.first]->coord.z += entry.second*elevationIncrement;
    }

    for (int id : halfIncrementIds) {
        ter->struct_point[id]->coord.z += elevationIncrement/2.0f;
    }
}

bool GarbrechtMartz::neighborIndexIsOutOfRange(int id, int neighborId)
{
    if (neighborId < 0 || neighborId > (int)ter->struct_point.size()-1 ||
            id == neighborId) {
        return true;
    }
    if ((id%ter->width == 0 && neighborId%ter->width == ter->width - 1) ||
            (id%ter->width == ter->width - 1 && neighborId%ter->width == 0)) {
        return true; // neighborId is not a true neighbor in the DEM grid!
    }
    return false;
}
