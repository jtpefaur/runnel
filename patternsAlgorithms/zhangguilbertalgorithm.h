#ifndef ZHANGGUILBERTALGORITHM_H
#define ZHANGGUILBERTALGORITHM_H
#include "algorithmpatron.h"
#include "painters/shaders/shaderpatron.h"
#include "UI/patronAlgorithm/zhangguilberconf.h"

class ZhangGuilbertAlgorithm : public AlgorithmPatron
{
    public:
        ZhangGuilbertAlgorithm();
        virtual ~ZhangGuilbertAlgorithm();
        virtual void run(Terrain *ter, std::vector<arbol*> &drainage_tree);
        virtual void render(glm::mat4 matrix, float exag_z);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    private:
        ZhangGuilberConf conf;
        ShaderPatron* shader;
        std::vector<std::string>  runAlgoritm();
        void getPoints(arbol* ar, std::vector<glm::vec3>& pointss);
        arbol* selectPrincipalStream(arbol* father);
        glm::vec3 getFinalPoint(arbol *ar);
        float lenghtFluente(arbol* padre);
        float catchmentElongation(arbol* ar);
        float lenghtRatio(arbol* ar);
        float sinuosity(arbol* ar);
        float angleUnion(arbol* ar);
        float chooseBestValue(float sq, float re);
        float vectorEqualDirection(glm::vec2 ini, glm::vec2 fin, glm::vec2 ini_com, glm::vec2 fin_com);



};

#endif // ZHANGGUILBERTALGORITHM_H
