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
    enum Direction {
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
        LEFT,
        RIGHT,
        BOTTOM_LEFT,
        BOTTOM,
        BOTTOM_RIGHT
    };
    void flood();
    bool initializeDirection(runnel::Point*);
    std::vector<runnel::Point*> computeNeighborhood(runnel::Point*);
    void setDirectionTowardsAdjacentPoint(runnel::Point*,runnel::Point*);
};

#endif // RWFLOODALGORITHM_H
