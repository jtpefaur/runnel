#ifndef PEUCKERDRAINAGENETWORK_H
#define PEUCKERDRAINAGENETWORK_H
#include "drainagealgorithms.h"
#include "UI/drainageAlgorithm/peuckerconf.h"
#include "painters/shaders/shaderdrainage.h"

#include "terrain.h"
class PeuckerDrainageNetwork : public DrainageAlgorithms
{
        Q_OBJECT
    public:
        PeuckerDrainageNetwork();
        virtual ~PeuckerDrainageNetwork();
        virtual void run(Terrain *terrain);
        virtual void runParallel(Terrain *terrain);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathTree();

    private:
       static void calculateGrid(Terrain *ter);
       static void calculateGridParallelProfiler(Terrain *ter);
       static void calculateGridParallel(Terrain *ter);
       std::vector<glm::vec3> getDrainageColor();
       std::vector<glm::vec3> points_edge;
       PeuckerConf conf;
       Terrain* terrain;
       ShaderDrainage* shader;
};

#endif // PEUCKERDRAINAGENETWORK_H
