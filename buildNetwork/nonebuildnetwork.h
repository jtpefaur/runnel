#ifndef NONEBUILDNETWORK_H
#define NONEBUILDNETWORK_H
#include "buildnetwork.h"

class NoneBuildNetwork : public BuildNetwork
{
    public:
        NoneBuildNetwork();
        virtual ~NoneBuildNetwork();
        virtual std::vector<arbol*> run(Terrain*);
        virtual void render(glm::mat4 matrix, float exag_z);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:

};

#endif // NONEBUILDNETWORK_H
