#ifndef PATHWATERCALLAGHANALGORITHM_H
#define PATHWATERCALLAGHANALGORITHM_H
#include "painters/shaders/shadergradient.h"
#include "terrain.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"
#include "UI/waterAlgorithm/callaghanwaterconf.h"

class PathWaterCallaghanAlgorithm : public PathWaterAlgorithm
{
        Q_OBJECT

    public:
        PathWaterCallaghanAlgorithm();
        virtual ~PathWaterCallaghanAlgorithm();
        virtual void run(glm::vec3 coord, Terrain *ter);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathWater();

    private:
        std::vector<int> getPointsNeigthbour(runnel::Point* pto);
        runnel::Point* getPointClosed(glm::vec3 coords);
        void getPath(runnel::Point* pto, std::unordered_map<int, int>& id_used);
        void changeAttr();
        ShaderGradient* shader;
        CallaghanWaterConf conf;
        Terrain *ter;
        std::vector<glm::vec3> edge_water;
        glm::vec3 coords_asked;
        float delta;
};

#endif // PATHWATERCALLAGHANALGORITHM_H
