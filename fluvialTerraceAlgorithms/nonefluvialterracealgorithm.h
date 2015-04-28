#ifndef NONEFLUVIALTERRACEALGORITHM_H
#define NONEFLUVIALTERRACEALGORITHM_H
#include "fluvialterracealgorithm.h"
#include "terrain.h"

class NoneFluvialTerraceAlgorithm : public FluvialTerraceAlgorithm
{
public:
    NoneFluvialTerraceAlgorithm();
    virtual ~NoneFluvialTerraceAlgorithm();
    virtual void run(glm::vec3, Terrain *);
    virtual void render(glm::mat4, float, glm::vec3);
    virtual void glewReady();
    virtual QString getName();
    virtual QWidget* getConf();
};

#endif // NONEFLUVIALTERRACEALGORITHM_H
