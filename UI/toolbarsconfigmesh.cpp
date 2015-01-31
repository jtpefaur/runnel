#include "toolbarsconfigmesh.h"
#include "ui_toolbarsconfigmesh.h"
#include <iostream>
#include "drainageAlgorithms/gradientdrainagecallaghanmark.h"
#include "drainageAlgorithms/nonedrainagealgorithm.h"

ToolbarsConfigMesh::ToolbarsConfigMesh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolbarsConfigMesh)
{
    ui->setupUi(this);
    QObject::connect(ui->river_button, SIGNAL(clicked()), this, SLOT(getDrainage()));
    QObject::connect(ui->path_water_button, SIGNAL(clicked()), this, SLOT(getWater()));
    QObject::connect(ui->drainage_patron_button, SIGNAL(clicked()), this, SLOT(getPatron()));
    //Add new gradient algorithms
    drainage_algorithm.push_back(new NoneDrainageAlgorithm());
    drainage_algorithm.push_back(new GradientDrainageCallaghanMark());
    this->drainageAlgorithms();

    //Add new water Movement

    //Add new Patron
}
void ToolbarsConfigMesh::drainageAlgorithms(){
    for( DrainageAlgorithms* item : drainage_algorithm){
        ui->river_value->addItem(item->getName());
        ui->river_conf->layout()->addWidget(item->getConf());
    }
}

ToolbarsConfigMesh::~ToolbarsConfigMesh()
{
    delete ui;
}

void ToolbarsConfigMesh::getDrainage(){
    int number = ui->river_value->currentIndex();
    DrainageAlgorithms* d = drainage_algorithm[number];
    emit selectDrainage(d);
}

void ToolbarsConfigMesh::getPatron(){
    QString name = ui->drainage_patron_value->currentText();
    emit selectPatron(name);
}

void ToolbarsConfigMesh::getWater(){
    QString name = ui->path_water_value->currentText();
    emit selectWater(name);
}

void ToolbarsConfigMesh::glewIsReady(){
    for( DrainageAlgorithms* item : drainage_algorithm){
        item->glewReady();
    }
}
