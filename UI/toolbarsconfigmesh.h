#ifndef TOOLBARSCONFIGMESH_H
#define TOOLBARSCONFIGMESH_H

#include <QStackedWidget>
#include <QWidget>
#include "drainageAlgorithms/drainagealgorithms.h"
#include "buildNetwork/buildnetwork.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"
#include "patternsAlgorithms/algorithmpatron.h"
#include "fluvialTerraceAlgorithms/fluvialterracealgorithm.h"

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
        std::vector<PathWaterAlgorithm*> path_water_algorithms;
        std::vector<AlgorithmPatron*> patron_algorithms;
        std::vector<FluvialTerraceAlgorithm*> fluvial_terrace_algorithms;

        QStackedWidget* drainageWidget;
        QStackedWidget* networkWidget;
        QStackedWidget* waterPathWidget;
        QStackedWidget* patternWidget;
        QStackedWidget* fluvialTerraceWidget;

    private:
        Ui::ToolbarsConfigMesh *ui;
        void drainageIncludeAlgorithms();
        void networkIncludeAlgorithms();
        void pathWaterIncludeAlgorithms();
        void patronIncludeAlgorithms();
        void fluvialTerraceIncludeAlgorithms();

    signals:
        void selectDrainage(DrainageAlgorithms* value);
        void selectNetwork(BuildNetwork* value);
        void selectWater(PathWaterAlgorithm* value);
        void selectPatron(AlgorithmPatron* value);
        void selectFluvialTerrace(FluvialTerraceAlgorithm* value);
        void changeElevation(int value);
        void changeLandForm(int value);
        void showGradientVector(bool value);
        void showNormalVector(bool value);
        void showCoordinateAxis(bool value);

    public slots:
        void getDrainage();
        void getNetwork();
        void getWater();
        void getPatron();
        void getFluvialTerrace();
        void glewIsReady();
        void showPatternsInformation();

};

#endif // TOOLBARSCONFIGMESH_H
