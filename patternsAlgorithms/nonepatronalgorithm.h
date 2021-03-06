#ifndef NONEPATRONALGORITHM_H
#define NONEPATRONALGORITHM_H
#include "patternsAlgorithms/algorithmpatron.h"

class NonePatronAlgorithm : public AlgorithmPatron
{
    public:
        NonePatronAlgorithm();
        virtual ~NonePatronAlgorithm();
        virtual void run(Terrain *, std::vector<arbol*> &);
        virtual void render(glm::mat4, float);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();
        virtual std::vector<glm::vec3> getPathTree();

    private:
};

#endif // NONEPATRONALGORITHM_H
