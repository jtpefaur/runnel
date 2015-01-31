#ifndef NONEDRAINAGEALGORITHM_H
#define NONEDRAINAGEALGORITHM_H
#include "drainageAlgorithms/drainagealgorithms.h"
#include "terrain.h"
#include "UI/drainageAlgorithm/noneconf.h"

class NoneDrainageAlgorithm : public DrainageAlgorithms
{
    public:
        NoneDrainageAlgorithm();
        virtual ~NoneDrainageAlgorithm();
        virtual void run(Terrain *ter);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:
        NoneConf conf;
};

#endif // NONEDRAINAGEALGORITHM_H
