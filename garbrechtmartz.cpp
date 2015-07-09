#include "garbrechtmartz.h"

GarbrechtMartz::GarbrechtMartz(Terrain* ter)
{
    this->ter = ter;
    elevationIncrement = 2;
}

GarbrechtMartz::~GarbrechtMartz()
{

}

void GarbrechtMartz::gradientTowardsLowerTerrain(std::set<int> flatIds,
                                                 std::set<int> downslopeGradientIds)
{
    if (flatIds.empty()) {
        return;
    }

    int width = ter->width;

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
                    flatIds.erase(id);
                }
            }
        }
    }

    for (int id : flatIds) {
        /* Increase elevation of remaining flat-belonging points
        without a downslope gradient. */
        ter->struct_point[id]->coord.z += elevationIncrement;
    }

    gradientTowardsLowerTerrain(flatIds, downslopeGradientIds);
}

