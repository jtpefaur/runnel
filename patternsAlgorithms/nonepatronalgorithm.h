#ifndef NONEPATRONALGORITHM_H
#define NONEPATRONALGORITHM_H
#include "patternsAlgorithms/algorithmpatron.h"

class NonePatronAlgorithm : public AlgorithmPatron
{
    public:
        NonePatronAlgorithm();
        virtual ~NonePatronAlgorithm();
        virtual void run(Terrain *, std::vector<arbol*> &);
        virtual void render(glm::mat4, float, glm::vec3);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:
};

#endif // NONEPATRONALGORITHM_H
