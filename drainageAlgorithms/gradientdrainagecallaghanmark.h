#ifndef GRADIENTDRAINAGECALLAGHANMARK_H
#define GRADIENTDRAINAGECALLAGHANMARK_H

#include <vector>
#include "painters/shaders/shadercallaghan.h"
#include "drainageAlgorithms/drainagealgorithms.h"
#include "primitives/point.h"
#include "terrain.h"
#include "UI/drainageAlgorithm/callaghanconf.h"

class GradientDrainageCallaghanMark : public DrainageAlgorithms
{

    public:
        GradientDrainageCallaghanMark();
        virtual ~GradientDrainageCallaghanMark();
        virtual void run(Terrain *ter);
        virtual void glewReady();
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual QString getName();
        virtual QWidget* getConf();

    private:

        void sortElement(std::vector<runnel::Point *> points);
        void chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto);
        ShaderCallaghan* shader_callaghan;
        int w;
        int h;
        float moreWater;
        float linewater;
        std::vector < runnel::Point* > points_terrain;
        CallaghanConf conf;


};

#endif // GRADIENTDRAINAGECALLAGHANMARK_H
