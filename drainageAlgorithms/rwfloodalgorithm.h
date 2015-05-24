#ifndef RWFLOODALGORITHM_H
#define RWFLOODALGORITHM_H
#include "drainagealgorithms.h"
class RWFloodAlgorithm : public DrainageAlgorithms
{
    Q_OBJECT
public:
    RWFloodAlgorithm();
    ~RWFloodAlgorithm();
    virtual void run(Terrain *ter);
    virtual void glewReady();
    virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
    virtual QString getName();
    virtual QWidget* getConf();
    virtual std::vector<glm::vec3> getPathTree();

private:
    Terrain* ter;
    enum direction {
        TOP_LEFT=0,
        TOP=1,
        TOP_RIGHT=2,
        LEFT=3,
        RIGHT=4,
        BOTTOM_LEFT=5,
        BOTTOM=6,
        BOTTOM_RIGHT=7
    };
    void flood();
    bool setInitialDirection(runnel::Point*);
    std::vector<runnel::Point*> computeNeighborhood(runnel::Point*);
};

#endif // RWFLOODALGORITHM_H
