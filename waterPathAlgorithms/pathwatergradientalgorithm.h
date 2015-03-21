#ifndef PATHWATERGRADIENTALGORITHM_H
#define PATHWATERGRADIENTALGORITHM_H
#include "terrain.h"
#include "painters/shaders/shadergradient.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"

class PathWaterGradientAlgorithm : public PathWaterAlgorithm
{
        Q_OBJECT
    public:
        PathWaterGradientAlgorithm();
        virtual ~PathWaterGradientAlgorithm();
        virtual void run(glm::vec3 point, Terrain *terr);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathWater();

   private:
        ShaderGradient* shader;
        Terrain* ter;
        runnel::Triangle* getTriangleOfPoint(glm::vec3 coords);
        void getNextTriangle(runnel::Triangle* trian);
        std::pair<runnel::Triangle*, glm::vec3> getEdgeInterseption(runnel::Triangle*trian, glm::vec3 vector_a, glm::vec3 pto);
        runnel::Triangle* calculateOtherTriangleInEdge(runnel::Triangle* trian_initial, runnel::Edge* ed);
        std::vector<glm::vec3> edge_path;
        std::unordered_map<int, int> triangles_id;
};

#endif // PATHWATERGRADIENTALGORITHM_H
