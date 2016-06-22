#ifndef NONEDRAINAGEALGORITHM_H
#define NONEDRAINAGEALGORITHM_H
#include "drainageAlgorithms/drainagealgorithms.h"
#include "terrain.h"

class NoneDrainageAlgorithm : public DrainageAlgorithms
{
    public:
        NoneDrainageAlgorithm();
        virtual ~NoneDrainageAlgorithm();
        virtual void run(Terrain *);
        virtual void runParallel(Terrain *);
        virtual void render(glm::mat4 matrix, float, glm::vec3);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathTree();

    private:
};

#endif // NONEDRAINAGEALGORITHM_H
