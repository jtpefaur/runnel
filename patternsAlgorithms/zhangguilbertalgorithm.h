#ifndef ZHANGGUILBERTALGORITHM_H
#define ZHANGGUILBERTALGORITHM_H
#include "algorithmpatron.h"

class ZhangGuilbertAlgorithm : public AlgorithmPatron
{
    public:
        std::vector<std::string>  runAlgoritm();
        ZhangGuilbertAlgorithm();
        void getPoints(arbol* ar, std::vector<glm::vec3>& pointss);
        arbol* selectPrincipalStream(arbol* father);
        float lenghtFluente(arbol* padre);
        float catchmentElongation(arbol* ar);
        float lenghtRatio(arbol* ar);
        float sinuosity(arbol* ar);
        float angleUnion(arbol* ar);
        float chooseBestValue(float sq, float re);
        glm::vec3 getFinalPoint(arbol *ar);
        float vectorEqualDirection(glm::vec2 ini, glm::vec2 fin, glm::vec2 ini_com, glm::vec2 fin_com);



};

#endif // ZHANGGUILBERTALGORITHM_H
