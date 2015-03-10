#ifndef PATHWATERGRADIENTALGORITHM_H
#define PATHWATERGRADIENTALGORITHM_H
#include "terrain.h"
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
        Terrain* ter;
        runnel::Triangle* getTriangleOfPoint(glm::vec3 coords);
        runnel::Triangle* getNextTriangle(runnel::Triangle* trian, glm::vec3 pto);
        std::pair<runnel::Triangle*, glm::vec3> getEdgeInterseption(runnel::Triangle*trian, glm::vec3 vector_a, glm::vec3 pto);
        bool isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto);

};

#endif // PATHWATERGRADIENTALGORITHM_H
