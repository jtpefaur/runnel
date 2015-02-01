#ifndef NONEBUILDNETWORK_H
#define NONEBUILDNETWORK_H
#include "buildnetwork.h"

class NoneBuildNetwork : public BuildNetwork
{
    public:
        NoneBuildNetwork();
        virtual ~NoneBuildNetwork();
        virtual void run(Terrain*);
        virtual void render(glm::mat4, float, glm::vec3);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:

};

#endif // NONEBUILDNETWORK_H
