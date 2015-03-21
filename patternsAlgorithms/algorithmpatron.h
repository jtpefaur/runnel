#ifndef ALGORITHMPATRON_H
#define ALGORITHMPATRON_H
#include <vector>
#include <QString>
#include <QWidget>
#include "terrain.h"
#include "patternsAlgorithms/arbol.h"

class AlgorithmPatron : public QObject
{
        Q_OBJECT

    public:
        AlgorithmPatron();
        virtual ~AlgorithmPatron();
        virtual void run(Terrain *ter, std::vector<arbol*> &drainage_tree) = 0;
        virtual void render(glm::mat4 matrix, float exag_z) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;
        virtual std::vector<glm::vec3> getPathTree() = 0;

    signals:
       void reload();

    protected:
        Terrain *terreno;
        std::vector<arbol*> drainage_trees;
};

#endif // ALGORITHMPATRON_H

