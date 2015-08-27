#include "toolbarsconfigmesh.h"
#include "ui_toolbarsconfigmesh.h"
#include <iostream>
#include <QMessageBox>
#include "patternsdata.h"
#include "drainageAlgorithms/nonedrainagealgorithm.h"
#include "drainageAlgorithms/peuckerdrainagenetwork.h"
#include "drainageAlgorithms/diedralangledrainage.h"
#include "drainageAlgorithms/gradientdrainagecallaghanmark.h"
#include "drainageAlgorithms/rwfloodalgorithm.h"
#include "buildNetwork/nonebuildnetwork.h"
#include "buildNetwork/buildtreecallaghan.h"
#include "buildNetwork/buildtreepeucker.h"
#include "buildNetwork/buildtreerwflood.h"
#include "waterPathAlgorithms/nonepathwateralgorithm.h"
#include "waterPathAlgorithms/pathwatercallaghanalgorithm.h"
#include "waterPathAlgorithms/pathwatergradientalgorithm.h"
#include "patternsAlgorithms/nonepatronalgorithm.h"
#include "patternsAlgorithms/zhangguilbertalgorithm.h"
#include "fluvialTerraceAlgorithms/nonefluvialterracealgorithm.h"
#include "fluvialTerraceAlgorithms/normalvectorsimilarityalgorithm.h"

ToolbarsConfigMesh::ToolbarsConfigMesh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolbarsConfigMesh)
{
    ui->setupUi(this);

    drainageWidget = 0;
    networkWidget = 0;
    waterPathWidget = 0;
    patternWidget = 0;
    fluvialTerraceWidget = 0;

    QObject::connect(ui->river_button, SIGNAL(clicked()), this, SLOT(getDrainage()));
    QObject::connect(ui->network_button, SIGNAL(clicked()), this, SLOT(getNetwork()));
    QObject::connect(ui->path_water_button, SIGNAL(clicked()), this, SLOT(getWater()));
    QObject::connect(ui->drainage_patron_button, SIGNAL(clicked()), this, SLOT(getPatron()));
    QObject::connect(ui->fluvial_terrace_button, SIGNAL(clicked()), this, SLOT(getFluvialTerrace()));

    QObject::connect(ui->garbrecht_martz_conf, SIGNAL(changeIncrementation(double)), this, SLOT(changeIncrementation(double)));
    QObject::connect(ui->garbrecht_martz_conf, SIGNAL(runFlatResolution()), this, SLOT(runFlatResolution()));

    QObject::connect(ui->exaggeration_terrain_value, SIGNAL(valueChanged(int)), this, SIGNAL(changeElevation(int)));
    QObject::connect(ui->landform_value, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changeLandForm(int)));
    QObject::connect(ui->gradient_vector, SIGNAL(clicked(bool)), this, SIGNAL(showGradientVector(bool)));
    QObject::connect(ui->normal_vector, SIGNAL(clicked(bool)), this, SIGNAL(showNormalVector(bool)));
    QObject::connect(ui->coordinate_axis, SIGNAL(clicked(bool)), this, SIGNAL(showCoordinateAxis(bool)));
    QObject::connect(ui->patterns_information, SIGNAL(clicked()), this, SLOT(showPatternsInformation()));

    this->drainageIncludeAlgorithms();
    this->networkIncludeAlgorithms();
    this->pathWaterIncludeAlgorithms();
    this->patronIncludeAlgorithms();
    this->fluvialTerraceIncludeAlgorithms();
    this->setupFlatResolutionAlgorithm();
}

ToolbarsConfigMesh::~ToolbarsConfigMesh()
{
    // Clear conf's layouts to prevent double delete.
    while (ui->river_conf->layout()->takeAt(0) != 0);
    while (ui->path_conf->layout()->takeAt(0) != 0);
    while (ui->network_conf->layout()->takeAt(0) != 0);
    while (ui->patron_conf->layout()->takeAt(0) != 0);
    while (ui->fluvial_terrace_conf->layout()->takeAt(0) != 0);
    for(auto alg: drainage_algorithms)
        delete alg;
    for(auto alg: path_water_algorithms)
        delete alg;
    for(auto alg: patron_algorithms)
        delete alg;
    for(auto alg: network_algorithms)
        delete alg;
    for(auto alg: fluvial_terrace_algorithms)
        delete alg;
    delete flatResolutionAlgorithm;
    delete drainageWidget;
    delete networkWidget;
    delete patternWidget;
    delete waterPathWidget;
    delete fluvialTerraceWidget;
    delete ui;
}

void ToolbarsConfigMesh::drainageIncludeAlgorithms()
{
    drainageWidget = new QStackedWidget;

    drainage_algorithms.push_back(new NoneDrainageAlgorithm());
    drainage_algorithms.push_back(new PeuckerDrainageNetwork());
    drainage_algorithms.push_back(new DiedralAngleDrainage());
    drainage_algorithms.push_back(new GradientDrainageCallaghanMark());
    drainage_algorithms.push_back(new RWFloodAlgorithm());

    for (DrainageAlgorithms* item : drainage_algorithms) {
        ui->river_value->addItem(item->getName());
        if (item->getConf()) {
            drainageWidget->addWidget(item->getConf());
        }
    }

    ui->river_conf->layout()->addWidget(drainageWidget);
    drainageWidget->hide();
}

void ToolbarsConfigMesh::getDrainage()
{
    int number = ui->river_value->currentIndex();
    DrainageAlgorithms* item = drainage_algorithms[number];

    if (item->getConf()) {
        drainageWidget->setCurrentWidget(item->getConf());
        drainageWidget->show();
    } else {
        drainageWidget->hide();
    }

    emit selectDrainage(item);
}

void ToolbarsConfigMesh::networkIncludeAlgorithms()
{
    networkWidget = new QStackedWidget;

    network_algorithms.push_back(new NoneBuildNetwork());
    network_algorithms.push_back(new buildTreeCallaghan());
    network_algorithms.push_back(new BuildTreePeucker());
    network_algorithms.push_back(new BuildTreeRWFlood());

    for (BuildNetwork* item : network_algorithms) {
        ui->network_value->addItem(item->getName());
        if (item->getConf()) {
            networkWidget->addWidget(item->getConf());
        }
    }

    ui->network_conf->layout()->addWidget(networkWidget);
    networkWidget->hide();
}

void ToolbarsConfigMesh::getNetwork()
{
    int number = ui->network_value->currentIndex();
    BuildNetwork* item = network_algorithms[number];

    if (item->getConf()) {
        networkWidget->setCurrentWidget(item->getConf());
        networkWidget->show();
    } else {
        networkWidget->hide();
    }

    emit selectNetwork(item);
}

void ToolbarsConfigMesh::pathWaterIncludeAlgorithms()
{
    waterPathWidget = new QStackedWidget;

    path_water_algorithms.push_back(new NonePathWaterAlgorithm());
    path_water_algorithms.push_back(new PathWaterCallaghanAlgorithm());
    path_water_algorithms.push_back(new PathWaterGradientAlgorithm());

    for (PathWaterAlgorithm* item : path_water_algorithms) {
        ui->path_water_value->addItem(item->getName());
        if (item->getConf()) {
            waterPathWidget->addWidget(item->getConf());
        }
    }

    ui->path_conf->layout()->addWidget(waterPathWidget);
    waterPathWidget->hide();
}

void ToolbarsConfigMesh::getWater()
{
    int number = ui->path_water_value->currentIndex();
    PathWaterAlgorithm* item = path_water_algorithms[number];

    if (item->getConf()) {
        waterPathWidget->setCurrentWidget(item->getConf());
        waterPathWidget->show();
    } else {
        waterPathWidget->hide();
    }

    emit selectWater(item);
}

void ToolbarsConfigMesh::patronIncludeAlgorithms()
{
    patternWidget = new QStackedWidget;

    patron_algorithms.push_back(new NonePatronAlgorithm());
    patron_algorithms.push_back(new ZhangGuilbertAlgorithm());

    for (AlgorithmPatron* item : patron_algorithms) {
        ui->drainage_patron_value->addItem(item->getName());
        if (item->getConf()) {
            patternWidget->addWidget(item->getConf());
        }
    }

    ui->patron_conf->layout()->addWidget(patternWidget);
    patternWidget->hide();
}

void ToolbarsConfigMesh::getPatron()
{
    int number = ui->drainage_patron_value->currentIndex();
    AlgorithmPatron* item = patron_algorithms[number];

    if (item->getConf()) {
        patternWidget->setCurrentWidget(item->getConf());
        patternWidget->show();
    } else {
        patternWidget->hide();
    }

    emit selectPatron(item);
}

void ToolbarsConfigMesh::fluvialTerraceIncludeAlgorithms()
{
    fluvialTerraceWidget = new QStackedWidget;

    fluvial_terrace_algorithms.push_back(new NoneFluvialTerraceAlgorithm());
    fluvial_terrace_algorithms.push_back(new NormalVectorSimilarityAlgorithm());

    for (FluvialTerraceAlgorithm* item : fluvial_terrace_algorithms) {
        ui->fluvial_terrace_value->addItem(item->getName());
        if (item->getConf()) {
            fluvialTerraceWidget->addWidget(item->getConf());
        }
    }

    ui->fluvial_terrace_conf->layout()->addWidget(fluvialTerraceWidget);
    fluvialTerraceWidget->hide();
}

void ToolbarsConfigMesh::setupFlatResolutionAlgorithm()
{
    flatResolutionAlgorithm = new GarbrechtMartz();
}

void ToolbarsConfigMesh::getFluvialTerrace()
{
    int number = ui->fluvial_terrace_value->currentIndex();
    FluvialTerraceAlgorithm* item = fluvial_terrace_algorithms[number];

    if (item->getConf()) {
        fluvialTerraceWidget->setCurrentWidget(item->getConf());
        fluvialTerraceWidget->show();
    } else {
        fluvialTerraceWidget->hide();
    }

    emit selectFluvialTerrace(item);
}

void ToolbarsConfigMesh::glewIsReady()
{
    for (DrainageAlgorithms* item : drainage_algorithms) {
        item->glewReady();
    }
    for (BuildNetwork* item : network_algorithms) {
        item->glewReady();
    }
    for (PathWaterAlgorithm* item : path_water_algorithms) {
        item->glewReady();
    }
    for (AlgorithmPatron* item : patron_algorithms) {
        item->glewReady();
    }
    for (FluvialTerraceAlgorithm* item : fluvial_terrace_algorithms) {
        item->glewReady();
    }
}

void ToolbarsConfigMesh::changeIncrementation(double value)
{
    flatResolutionAlgorithm->changeIncrementation(value);
}

void ToolbarsConfigMesh::runFlatResolution()
{
    emit resolveFlats(flatResolutionAlgorithm);
}

void ToolbarsConfigMesh::showPatternsInformation()
{
    QMessageBox s;
    QString title = "Patterns Information";
    QString information;
    information+="<table>";

    for (int i = 0; i < (int)PatternsData::names_patterns.size(); ++i) {
        information+= "<tr>";

        information+= "<td valign=\"middle\" >";
        information+= "<img src=\"" + PatternsData::images[i] +"\" >";
        information+= "</td>";

        information+= "<td valign=\"middle\" >";
        information+= "<b>" + PatternsData::names_patterns[i] + "</b>";
        information+= "<br>";
        information+= PatternsData::descriptions[i];
        information+= "<br>";
        information+= "</td>";

        information+= "</tr>";
    }

    information+="</table>";
    s.about(this, title, information);
}
