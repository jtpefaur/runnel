#ifndef NONEPATHWATERALGORITHM_H
#define NONEPATHWATERALGORITHM_H
#include "pathwateralgorithm.h"

class NonePathWaterAlgorithm : public PathWaterAlgorithm
{
    public:
        NonePathWaterAlgorithm();
        virtual ~NonePathWaterAlgorithm();
        virtual void run(glm::vec3, Terrain *);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathWater();

};

#endif // NONEPATHWATERALGORITHM_H
