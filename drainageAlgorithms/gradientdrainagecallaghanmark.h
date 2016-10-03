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
        Q_OBJECT
    public:
        GradientDrainageCallaghanMark();
        virtual ~GradientDrainageCallaghanMark();
        virtual void run(Terrain *terrain);
        virtual void runParallel(Terrain* terrain);
        virtual void glewReady();
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathTree();


    private:
        CallaghanConf conf;
        void sortElement(std::vector<runnel::Point *> points);
        void chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto);
        void getMoreWaterPoint();
        ShaderCallaghan* shader_callaghan;
        int w;
        int h;
        Terrain* terrain;
        std::vector<float> count_water;
        float max_value_water;
        std::vector<glm::vec3> position_water_points;
        float moreWater;
        float max_water;
        float linewater;
        std::vector < runnel::Point* > points_terrain;


    public slots:
        void changeAttr();


};

#endif // GRADIENTDRAINAGECALLAGHANMARK_H
