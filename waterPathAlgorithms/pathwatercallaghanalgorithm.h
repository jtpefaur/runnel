#ifndef PATHWATERCALLAGHANALGORITHM_H
#define PATHWATERCALLAGHANALGORITHM_H
#include "painters/shaders/shadergradient.h"
#include "terrain.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"

class PathWaterCallaghanAlgorithm : public PathWaterAlgorithm
{
    public:
        PathWaterCallaghanAlgorithm();
        virtual ~PathWaterCallaghanAlgorithm();
        virtual void run(glm::vec3, Terrain *);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:
        ShaderGradient* shader;
};

#endif // PATHWATERCALLAGHANALGORITHM_H
