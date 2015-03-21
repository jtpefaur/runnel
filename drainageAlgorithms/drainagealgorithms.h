#ifndef DRAINAGEALGORITHMS_H
#define DRAINAGEALGORITHMS_H
#include <QWidget>
#include <QObject>
#include "terrain.h"

class DrainageAlgorithms : public QObject
{
        Q_OBJECT
    public:
        DrainageAlgorithms();
        virtual ~DrainageAlgorithms();
        virtual void run(Terrain *ter) = 0;
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color) = 0;
        virtual void glewReady() = 0;
        virtual QString getName() = 0;
        virtual QWidget* getConf() = 0;
        virtual std::vector<glm::vec3> getPathTree() = 0;

    signals:
       void reload();




};

#endif // DRAINAGEALGORITHMS_H
