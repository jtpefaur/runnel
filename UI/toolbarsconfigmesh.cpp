#include "toolbarsconfigmesh.h"
#include "ui_toolbarsconfigmesh.h"
#include <iostream>
#include "drainageAlgorithms/gradientdrainagecallaghanmark.h"
#include "drainageAlgorithms/peuckerdrainagenetwork.h"
#include "drainageAlgorithms/nonedrainagealgorithm.h"
#include "drainageAlgorithms/diedralangledrainage.h"
#include "buildNetwork/nonebuildnetwork.h"
#include "buildNetwork/buildtreecallaghan.h"
#include "patternsAlgorithms/nonepatronalgorithm.h"
#include "waterPathAlgorithms/nonepathwateralgorithm.h"


ToolbarsConfigMesh::ToolbarsConfigMesh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolbarsConfigMesh)
{
    ui->setupUi(this);
    QObject::connect(ui->drainage_patron_button, SIGNAL(clicked()), this, SLOT(getPatron()));
    QObject::connect(ui->river_button, SIGNAL(clicked()), this, SLOT(getDrainage()));
    QObject::connect(ui->path_water_button, SIGNAL(clicked()), this, SLOT(getWater()));
    QObject::connect(ui->network_button, SIGNAL(clicked()), this, SLOT(getNetwork()));

    this->drainageIncludeAlgorithms();
    this->pathWaterIncludeAlgorithms();
    this->networkIncludeAlgorithms();
    this->patronIncludeAlgorithms();
}

ToolbarsConfigMesh::~ToolbarsConfigMesh()
{
    // Clear conf's layouts to prevent double delete.
    while (ui->river_conf->layout()->takeAt(0) != 0);
    while (ui->path_conf->layout()->takeAt(0) != 0);
    while (ui->network_conf->layout()->takeAt(0) != 0);
    while (ui->patron_conf->layout()->takeAt(0) != 0);
    for(auto alg: drainage_algorithms)
        delete alg;
    for(auto alg: path_water_algorithms)
        delete alg;
    for(auto alg: patron_algorithms)
        delete alg;
    for(auto alg: network_algorithms)
        delete alg;
    delete ui;
}

void ToolbarsConfigMesh::drainageIncludeAlgorithms(){
    //Add new gradient algorithms
    drainage_algorithms.push_back(new NoneDrainageAlgorithm());
    drainage_algorithms.push_back(new PeuckerDrainageNetwork());
    drainage_algorithms.push_back(new DiedralAngleDrainage());
    drainage_algorithms.push_back(new GradientDrainageCallaghanMark());
    for( DrainageAlgorithms* item : drainage_algorithms){
        ui->river_value->addItem(item->getName());
        if(item->getConf())
            ui->river_conf->layout()->addWidget(item->getConf());
    }
}

void ToolbarsConfigMesh::getDrainage(){
    int number = ui->river_value->currentIndex();
    DrainageAlgorithms* d = drainage_algorithms[number];
    emit selectDrainage(d);
}

void ToolbarsConfigMesh::networkIncludeAlgorithms(){
    network_algorithms.push_back(new NoneBuildNetwork());
    network_algorithms.push_back(new buildTreeCallaghan());
    for( BuildNetwork* item : network_algorithms){
        ui->network_value->addItem(item->getName());
        if(item->getConf())
            ui->network_conf->layout()->addWidget(item->getConf());
    }
}

void ToolbarsConfigMesh::getNetwork(){
    int number = ui->network_value->currentIndex();
    BuildNetwork* d = network_algorithms[number];
    emit selectNetwork(d);
}

void ToolbarsConfigMesh::patronIncludeAlgorithms(){
    patron_algorithms.push_back(new NonePatronAlgorithm());
    for( AlgorithmPatron* item : patron_algorithms){
        ui->drainage_patron_value->addItem(item->getName());
        if(item->getConf())
            ui->patron_conf->layout()->addWidget(item->getConf());
    }
}
void ToolbarsConfigMesh::getPatron(){
    int number = ui->drainage_patron_value->currentIndex();
    AlgorithmPatron* d = patron_algorithms[number];
    emit selectPatron(d);
}

void ToolbarsConfigMesh::pathWaterIncludeAlgorithms(){
    path_water_algorithms.push_back(new NonePathWaterAlgorithm());
    for( PathWaterAlgorithm* item : path_water_algorithms){
        ui->path_water_value->addItem(item->getName());
        if(item->getConf())
            ui->path_conf->layout()->addWidget(item->getConf());
    }
}

void ToolbarsConfigMesh::getWater(){
    int number = ui->path_water_value->currentIndex();
    PathWaterAlgorithm* d = path_water_algorithms[number];
    emit selectWater(d);
}


void ToolbarsConfigMesh::glewIsReady(){
    for( DrainageAlgorithms* item : drainage_algorithms){
        item->glewReady();
    }
    for( BuildNetwork* item : network_algorithms){
        item->glewReady();
    }
    for( PathWaterAlgorithm* item : path_water_algorithms){
        item->glewReady();
    }
    for( AlgorithmPatron* item : patron_algorithms){
        item->glewReady();
    }
}
