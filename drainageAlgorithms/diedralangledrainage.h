#ifndef DIEDRALANGLEDRAINAGE_H
#define DIEDRALANGLEDRAINAGE_H
#include "drainageAlgorithms/drainagealgorithms.h"
#include "painters/shaders/shaderangleedge.h"
#include "UI/drainageAlgorithm/anglediedralconf.h"

class DiedralAngleDrainage : public DrainageAlgorithms
{
        Q_OBJECT
    public:
        DiedralAngleDrainage();
        virtual ~DiedralAngleDrainage();
        virtual void run(Terrain *ter);
        virtual void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
        virtual void glewReady();
        virtual QString getName();
        virtual QWidget* getConf();

    public slots:
        void changeAttr();

    private:
        ShaderAngleEdge* shader;
        Terrain* terr;
        AngleDiedralConf conf;
};

#endif // DIEDRALANGLEDRAINAGE_H
