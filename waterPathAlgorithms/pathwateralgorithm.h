#ifndef PATHWATERALGORITHM_H
#define PATHWATERALGORITHM_H
#include <QWidget>
#include "terrain.h"
#include "lib/glm/glm.hpp"


class PathWaterAlgorithm
{
    public:
        PathWaterAlgorithm();
        virtual ~PathWaterAlgorithm();
        virtual void run(glm::vec3 coords, Terrain *ter) = 0;
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;
        virtual std::vector<glm::vec3> getPathWater() = 0;

};

#endif // PATHWATERALGORITHM_H
