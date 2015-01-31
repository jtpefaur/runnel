#include "toolbarsconfigmesh.h"
#include "ui_toolbarsconfigmesh.h"
#include <iostream>

ToolbarsConfigMesh::ToolbarsConfigMesh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolbarsConfigMesh)
{
    ui->setupUi(this);
    QObject::connect(ui->river_button, SIGNAL(clicked()), this, SLOT(getDrainage()));
    QObject::connect(ui->path_water_button, SIGNAL(clicked()), this, SLOT(getWater()));
    QObject::connect(ui->drainage_patron_button, SIGNAL(clicked()), this, SLOT(getPatron()));
}

ToolbarsConfigMesh::~ToolbarsConfigMesh()
{
    delete ui;
}

void ToolbarsConfigMesh::getDrainage(){
    QString name = ui->river_value->currentText();
    emit selectDrainage(name);
}

void ToolbarsConfigMesh::getPatron(){
    QString name = ui->drainage_patron_value->currentText();
    emit selectPatron(name);
}

void ToolbarsConfigMesh::getWater(){
    QString name = ui->path_water_value->currentText();
    emit selectWater(name);
}
