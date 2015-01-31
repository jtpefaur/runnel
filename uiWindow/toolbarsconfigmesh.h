#ifndef TOOLBARSCONFIGMESH_H
#define TOOLBARSCONFIGMESH_H

#include <QWidget>

namespace Ui {
    class ToolbarsConfigMesh;
}

class ToolbarsConfigMesh : public QWidget
{
        Q_OBJECT

    public:
        explicit ToolbarsConfigMesh(QWidget *parent = 0);
        ~ToolbarsConfigMesh();

    private:
        Ui::ToolbarsConfigMesh *ui;

    signals:
        void selectDrainage(QString value);
        void selectPatron(QString value);
        void selectWater(QString value);

    public slots:
        void getDrainage();
        void getPatron();
        void getWater();

};

#endif // TOOLBARSCONFIGMESH_H
