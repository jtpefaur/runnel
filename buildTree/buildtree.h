#ifndef BUILDTREE_H
#define BUILDTREE_H
#include <QString>
#include <QWidget>
#include "patternsAlgorithms/arbol.h"
#include "terrain.h"

class BuildTree
{
    public:
        BuildTree();
        virtual ~BuildTree();
        virtual void run(Terrain* ter) = 0;
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;

};

#endif // BUILDTREE_H
