#ifndef PATHWATERGRADIENTALGORITHM_H
#define PATHWATERGRADIENTALGORITHM_H
#include "painters/shaders/shadergradient.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"

class PathWaterGradientAlgorithm : public PathWaterAlgorithm
{
    public:
        PathWaterGradientAlgorithm();
        virtual ~PathWaterGradientAlgorithm();
        virtual void run(glm::vec3 point, Terrain *terr);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

   private:
        ShaderGradient* shader;
};

#endif // PATHWATERGRADIENTALGORITHM_H
