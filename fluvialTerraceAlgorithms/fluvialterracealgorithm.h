#ifndef FLUVIALTERRACEALGORITHM_H
#define FLUVIALTERRACEALGORITHM_H
#include <QWidget>
#include "terrain.h"
#include "lib/glm/glm.hpp"

class FluvialTerraceAlgorithm : public QObject
{
    Q_OBJECT
    public:
        FluvialTerraceAlgorithm();
        virtual ~FluvialTerraceAlgorithm();
        virtual void run(glm::vec3 coords, Terrain *ter) = 0;
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;

   signals:
        void reload();
};

#endif // FLUVIALTERRACEALGORITHM_H
