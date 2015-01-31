#ifndef GRADIENTDRAINAGECALLAGHANMARK_H
#define GRADIENTDRAINAGECALLAGHANMARK_H

#include <vector>
#include "primitives/point.h"
#include "terrain.h"

class GradientDrainageCallaghanMark
{

    public:
        GradientDrainageCallaghanMark();
        int w;
        int h;
        std::vector < runnel::Point* > points_terrain;
        void sortElement(std::vector<runnel::Point *> points);
        void chooseMoreDepthPoint(std::vector<runnel::Point *>& points, runnel::Point *pto, float delta_water);

        void runAlgorithm(Terrain *ter, float moreWater);

};

#endif // GRADIENTDRAINAGECALLAGHANMARK_H
