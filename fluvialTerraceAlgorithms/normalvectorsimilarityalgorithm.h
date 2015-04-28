#ifndef NORMALVECTORSIMILARITYALGORITHM_H
#define NORMALVECTORSIMILARITYALGORITHM_H
#include "fluvialterracealgorithm.h"
#include "UI/fluvialTerraceAlgorithm/normalvectorsimilarityconf.h"

#include <painters/shaders/shadernormalvectorsimilarity.h>

class NormalVectorSimilarityAlgorithm : public FluvialTerraceAlgorithm
{
    Q_OBJECT
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
        float angleThreshold;
        glm::vec3 clickedPoint;
        std::vector<glm::vec3> terraceVertices;
        NormalVectorSimilarityConf conf;
        ShaderNormalVectorSimilarity* shader;

    public slots:
        void changeAttr();
};

#endif // NORMALVECTORSIMILARITYALGORITHM_H
