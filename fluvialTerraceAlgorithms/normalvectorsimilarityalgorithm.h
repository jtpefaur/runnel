#ifndef NORMALVECTORSIMILARITYALGORITHM_H
#define NORMALVECTORSIMILARITYALGORITHM_H
#include "terrain.h"
#include "fluvialTerraceAlgorithms/fluvialterracealgorithm.h"

class NormalVectorSimilarityAlgorithm : public fluvialTerraceAlgorithm
{
    public:
        NormalVectorSimilarityAlgorithm();
        virtual ~NormalVectorSimilarityAlgorithm();
        virtual void run(glm::vec3 point, Terrain *terr);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:
        Terrain* ter;
        // TODO Add shader, etc.
};

#endif // NORMALVECTORSIMILARITYALGORITHM_H
