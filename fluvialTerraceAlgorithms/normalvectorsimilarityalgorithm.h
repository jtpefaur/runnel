#ifndef NORMALVECTORSIMILARITYALGORITHM_H
#define NORMALVECTORSIMILARITYALGORITHM_H
#include "terrain.h"
#include "fluvialTerraceAlgorithms/fluvialterracealgorithm.h"

class NormalVectorSimilarityAlgorithm : public FluvialTerraceAlgorithm
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
        runnel::Triangle* baseTriangle;
        // TODO Add shader, etc.

    public slots:
        void changeThreshold();
};

#endif // NORMALVECTORSIMILARITYALGORITHM_H
