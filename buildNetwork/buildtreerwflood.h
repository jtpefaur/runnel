#ifndef BUILDTREERWFLOOD_H
#define BUILDTREERWFLOOD_H

#include "buildnetwork.h"
#include "buildNetwork/arbol.h"
#include "terrain.h"
#include "painters/shaders/shaderpatron.h"
#include "primitives/point.h"
#include <UI/buildNetwork/rwfloodnetworkconf.h>

class BuildTreeRWFlood : public BuildNetwork
{
    Q_OBJECT
public:
    BuildTreeRWFlood();
    virtual ~BuildTreeRWFlood();
    virtual std::vector<arbol*> run(Terrain* ter);
    virtual void render(glm::mat4 matrix, float exag_z);
    virtual std::vector<arbol*> getData();
    virtual void glewReady();
    virtual QString getName();
    virtual QWidget* getConf();
    virtual std::vector<glm::vec3> getPathTree();
public slots:
    void changeWaterLevelThreshold();
private:
    Terrain* ter;
    ShaderPatron* shader;
    RWFloodNetworkConf conf;
    int waterLevelThreshold;
    std::vector<arbol*> networkTrees;
    std::vector<runnel::Point*> sortedPoints;
    void buildNetworkTrees();
    void buildTree(arbol* parent, std::map<int, bool> &visitedPoints);
    void flagPointsUnderThreshold(std::map<int, bool> &visitedPoints);
    void sortPoints(std::vector<runnel::Point*>& terrainPoints);
};

#endif // BUILDTREERWFLOOD_H
