#include "toolbarsconfigmesh.h"
#include "ui_toolbarsconfigmesh.h"
#include <iostream>
#include "drainageAlgorithms/gradientdrainagecallaghanmark.h"
#include "drainageAlgorithms/peuckerdrainagenetwork.h"
#include "drainageAlgorithms/nonedrainagealgorithm.h"
#include "drainageAlgorithms/diedralangledrainage.h"
#include "buildNetwork/nonebuildnetwork.h"
#include "buildNetwork/buildtreecallaghan.h"
#include "buildNetwork/buildtreepeucker.h"
#include "patternsAlgorithms/nonepatronalgorithm.h"
#include "patternsAlgorithms/zhangguilbertalgorithm.h"
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

    }
}

void ToolbarsConfigMesh::getDrainage(){
    int number = ui->river_value->currentIndex();
    DrainageAlgorithms* item = drainage_algorithms[number];
    if(item->getConf()){
        ui->river_conf->layout()->takeAt(0);
        ui->river_conf->layout()->addWidget(item->getConf());
    }
    emit selectDrainage(item);
}

void ToolbarsConfigMesh::networkIncludeAlgorithms(){
    network_algorithms.push_back(new NoneBuildNetwork());
    network_algorithms.push_back(new buildTreeCallaghan());
    network_algorithms.push_back(new BuildTreePeucker());
    for( BuildNetwork* item : network_algorithms){
        ui->network_value->addItem(item->getName());
    }
}

void ToolbarsConfigMesh::getNetwork(){
    int number = ui->network_value->currentIndex();
    BuildNetwork* item = network_algorithms[number];
    if(item->getConf()){
        ui->network_conf->layout()->takeAt(0);
        ui->network_conf->layout()->addWidget(item->getConf());
    }
    emit selectNetwork(item);
}

void ToolbarsConfigMesh::patronIncludeAlgorithms(){
    patron_algorithms.push_back(new NonePatronAlgorithm());
    patron_algorithms.push_back(new ZhangGuilbertAlgorithm());
    for( AlgorithmPatron* item : patron_algorithms){
        ui->drainage_patron_value->addItem(item->getName());
    }
}
void ToolbarsConfigMesh::getPatron(){
    int number = ui->drainage_patron_value->currentIndex();
    AlgorithmPatron* item = patron_algorithms[number];
    if(item->getConf()){
        ui->patron_conf->layout()->takeAt(0);
        ui->patron_conf->layout()->addWidget(item->getConf());
    }
    emit selectPatron(item);
}

void ToolbarsConfigMesh::pathWaterIncludeAlgorithms(){
    path_water_algorithms.push_back(new NonePathWaterAlgorithm());
    for( PathWaterAlgorithm* item : path_water_algorithms){
        ui->path_water_value->addItem(item->getName());
    }
}

void ToolbarsConfigMesh::getWater(){
    int number = ui->path_water_value->currentIndex();
    PathWaterAlgorithm* item = path_water_algorithms[number];
    if(item->getConf()){
        ui->path_conf->layout()->takeAt(0);
        ui->path_conf->layout()->addWidget(item->getConf());
    }
    emit selectWater(item);
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
