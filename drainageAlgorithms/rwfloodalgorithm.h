#ifndef RWFLOODALGORITHM_H
#define RWFLOODALGORITHM_H
#include "drainagealgorithms.h"

#include <painters/shaders/shaderrwflood.h>

#include <UI/drainageAlgorithm/rwfloodconf.h>

class RWFloodAlgorithm : public DrainageAlgorithms
{
    Q_OBJECT
public:
    RWFloodAlgorithm();
    ~RWFloodAlgorithm();
    virtual void run(Terrain *ter);
    virtual void runParallel(Terrain *ter);
    virtual void glewReady();
    virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
    virtual QString getName();
    virtual QWidget* getConf();
    virtual std::vector<glm::vec3> getPathTree();

private:
    Terrain* ter;
    ShaderRWFlood* shader;
    RWFloodConf conf;
    int maxWaterCount;
    int waterThreshold;
    std::vector<glm::vec3> drainagePoints;
    std::vector<glm::vec3> drainageColors;
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

    void flood(std::vector<runnel::Point*>&);
    void calculateWaterAccumulation(std::vector<runnel::Point*>&);
    void getDrainagePoints();

    bool initializeDirection(std::vector<runnel::Point*>&,runnel::Point*);
    std::vector<runnel::Point*> computeNeighborhood(runnel::Point*);
    void setDirectionTowardsAdjacentPoint(std::vector<runnel::Point*>&,
                                          runnel::Point*,
                                          runnel::Point*);
    bool isDirectedOutsideTerrainBoundary(runnel::Point*);
    bool isDirectedOutsideTerrainBoundary(int, char);
    int getNextPointId(runnel::Point*);
    int getNextPointId(int, char);

public slots:
    void changeAttr();
};

#endif // RWFLOODALGORITHM_H
