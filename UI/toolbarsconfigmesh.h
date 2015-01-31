#ifndef TOOLBARSCONFIGMESH_H
#define TOOLBARSCONFIGMESH_H

#include <QWidget>
#include "drainageAlgorithms/drainagealgorithms.h"

namespace Ui {
    class ToolbarsConfigMesh;
}

class ToolbarsConfigMesh : public QWidget
{
        Q_OBJECT

    public:
        explicit ToolbarsConfigMesh(QWidget *parent = 0);
        ~ToolbarsConfigMesh();
        std::vector<DrainageAlgorithms*> drainage_algorithm;
        void drainageAlgorithms();

    private:
        Ui::ToolbarsConfigMesh *ui;

    signals:
        void selectDrainage(DrainageAlgorithms* value);
        void selectPatron(QString value);
        void selectWater(QString value);

    public slots:
        void getDrainage();
        void getPatron();
        void getWater();
        void glewIsReady();

};

#endif // TOOLBARSCONFIGMESH_H
