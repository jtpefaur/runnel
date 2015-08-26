#ifndef BUILDTREERWFLOOD_H
#define BUILDTREERWFLOOD_H

#include "buildnetwork.h"
#include "buildNetwork/arbol.h"
#include "terrain.h"
#include "painters/shaders/shaderpatron.h"
#include "primitives/point.h"

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
private:
    Terrain* ter;
    ShaderPatron* shader;
    std::vector<arbol*> networkTrees;
};

#endif // BUILDTREERWFLOOD_H
