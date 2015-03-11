#ifndef BUILDTREEPEUCKER_H
#define BUILDTREEPEUCKER_H

#include "primitives/point.h"
#include "terrain.h"
#include "patternsAlgorithms/arbol.h"
#include "buildNetwork/buildnetwork.h"
#include "painters/shaders/shaderpatron.h"
#include "UI/buildNetwork/peuckernetworkconf.h"


class BuildTreePeucker : public BuildNetwork
{
        Q_OBJECT
    public:
        BuildTreePeucker();
        virtual ~BuildTreePeucker();
        virtual std::vector<arbol*> run(Terrain* ter);
        virtual void render(glm::mat4 matrix, float exag_z);
        virtual std::vector<arbol*> getData();
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:
        void createStructure(std::vector<runnel::Point*>& points, Terrain* ter);
        std::vector<runnel::Point*> points_order;
        std::unordered_map<int, int> point_counter_peucker;
        Terrain* t;
        void createTree(arbol* ar);
        std::vector<arbol*> reviewPoints();
        void calculateGrid(Terrain *ter);
        std::vector<arbol*> arbolitos;
        ShaderPatron* shader;
        PeuckerNetworkConf conf;
};

#endif // BUILDTREEPEUCKER_H
