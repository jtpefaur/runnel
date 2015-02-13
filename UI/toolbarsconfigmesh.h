#ifndef TOOLBARSCONFIGMESH_H
#define TOOLBARSCONFIGMESH_H

#include <QWidget>
#include "drainageAlgorithms/drainagealgorithms.h"
#include "buildNetwork/buildnetwork.h"
#include "patternsAlgorithms/algorithmpatron.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"

namespace Ui {
    class ToolbarsConfigMesh;
}

class ToolbarsConfigMesh : public QWidget
{
        Q_OBJECT

    public:
        explicit ToolbarsConfigMesh(QWidget *parent = 0);
        ~ToolbarsConfigMesh();

        std::vector<DrainageAlgorithms*> drainage_algorithms;
        std::vector<BuildNetwork*> network_algorithms;
        std::vector<AlgorithmPatron*> patron_algorithms;
        std::vector<PathWaterAlgorithm*> path_water_algorithms;



    private:
        Ui::ToolbarsConfigMesh *ui;
        void drainageIncludeAlgorithms();
        void networkIncludeAlgorithms();
        void patronIncludeAlgorithms();
        void pathWaterIncludeAlgorithms();

    signals:
        void selectDrainage(DrainageAlgorithms* value);
        void selectNetwork(BuildNetwork* value);
        void selectPatron(AlgorithmPatron* value);
        void selectWater(PathWaterAlgorithm* value);
        void changeElevation(int value);
        void changeLandForm(int value);

    public slots:
        void getDrainage();
        void getPatron();
        void getWater();
        void getNetwork();
        void glewIsReady();

};

#endif // TOOLBARSCONFIGMESH_H
