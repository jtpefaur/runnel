#ifndef ALGORITHMPATRON_H
#define ALGORITHMPATRON_H
#include <vector>
#include <QString>
#include "terrain.h"
#include "patternsAlgorithms/arbol.h"

class AlgorithmPatron
{
    public:
        AlgorithmPatron();
        void setData(Terrain *ter, std::vector<arbol*> &drainage_tree);
        virtual std::vector<std::string> runAlgoritm();
    protected:
        Terrain *terreno;
        std::vector<arbol*> drainage_trees;
};

#endif // ALGORITHMPATRON_H

