#ifndef ALGORITHMPATRON_H
#define ALGORITHMPATRON_H
#include <vector>
#include <QString>
#include <QWidget>
#include "terrain.h"
#include "patternsAlgorithms/arbol.h"

class AlgorithmPatron
{
    public:
        AlgorithmPatron();
        virtual ~AlgorithmPatron();
        void setValues();
        virtual void run(Terrain *ter, std::vector<arbol*> &drainage_tree) = 0;
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;

    protected:
        Terrain *terreno;
        std::vector<arbol*> drainage_trees;
};

#endif // ALGORITHMPATRON_H

