#ifndef BUILDNETWORK_H
#define BUILDNETWORK_H
#include <QString>
#include <QWidget>
#include "patternsAlgorithms/arbol.h"
#include "terrain.h"

class BuildNetwork : public QObject
{
        Q_OBJECT
    public:
        BuildNetwork();
        virtual ~BuildNetwork();
        virtual std::vector<arbol*> run(Terrain* ter) = 0;
        virtual std::vector<arbol*> getData() = 0;
        virtual void render(glm::mat4 matrix, float exag_z) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;

};

#endif // BUILDNETWORK_H
