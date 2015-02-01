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
        virtual void run(Terrain *ter);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
    private:
       static void calculateGrid(Terrain *ter);
       PeuckerConf conf;
       Terrain* terr;
       ShaderDrainage* shader;
};

#endif // PEUCKERDRAINAGENETWORK_H
