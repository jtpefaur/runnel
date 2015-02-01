#ifndef BUILDNETWORK_H
#define BUILDNETWORK_H
#include <QString>
#include <QWidget>
#include "patternsAlgorithms/arbol.h"
#include "terrain.h"

class BuildNetwork
{
    public:
        BuildNetwork();
        virtual ~BuildNetwork();
        virtual void run(Terrain* ter) = 0;
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;

};

#endif // BUILDNETWORK_H
