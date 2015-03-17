#ifndef BUILDTREECALLAGHAN_H
#define BUILDTREECALLAGHAN_H
#include "buildnetwork.h"
#include <unordered_map>
#include "primitives/point.h"
#include "UI/buildNetwork/callaghannetworkconf.h"
#include "patternsAlgorithms/arbol.h"
#include "painters/shaders/shaderpatron.h"


class buildTreeCallaghan : public BuildNetwork
{
       Q_OBJECT
    public:
        buildTreeCallaghan();
        virtual ~buildTreeCallaghan();
        virtual std::vector<arbol*> run(Terrain* ter);
        virtual void render(glm::mat4 matrix, float exag_z);
        virtual std::vector<arbol*> getData();
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathTree();
    public slots:
        void changeAttr();
    private:
        ShaderPatron* shader_build;
        void getPoints(std::vector<runnel::Point*>& points);
        void createTree(arbol* parent);
        void reviewPoints();
        std::unordered_map<int, int> point_counter;
        std::vector<runnel::Point*> points_order;
        float max_water;
        Terrain* terr;
        CallaghanNetworkConf conf;
        std::vector<arbol*> arbolitos;
        void chooseMoreDepthPoint(std::vector<runnel::Point*>& points, runnel::Point *pto);

};

#endif // BUILDTREECALLAGHAN_H
