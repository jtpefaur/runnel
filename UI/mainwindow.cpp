#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sstream>

#include <QFileDialog>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    runnel_controller(),
    conf(0),
    ui(new Ui::MainWindow),
    ui_toolbar(parent)
{
    ui->setupUi(this);
    this->showMaximized();
    this->connectGoogleMapWeb();
    ui->malla_see->layout()->addWidget(&ui_toolbar);
    std::cout << "Start Aplication..." << std::endl;

    this->connectSignalForRunnel();
    conf = runnel_controller.getColorConf();
    this->confColorRunnel();


}

void MainWindow::connectGoogleMapWeb(){
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::JavascriptEnabled,true);
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    ui->mapa_google->layout()->addWidget( &runnel_controller.getGoogleMap());
    ui->malla_see->layout()->addWidget(&runnel_controller.getPainter());
}



MainWindow::~MainWindow()
{
    QWebSettings::clearMemoryCaches();
    while(ui->malla_see->layout()->takeAt(0));
    while(ui->mapa_google->layout()->takeAt(0));
    delete ui;
}

void MainWindow::connectSignalForRunnel(){

    QObject::connect(ui->actionTIFF, SIGNAL(triggered()),this , SLOT(getTypeTIFFForObtainNameFile()));
    QObject::connect(ui->actionRunnel, SIGNAL(triggered()),this , SLOT(getTypeRunnelForObtainNameFile()));
    QObject::connect(ui->actionSave_File, SIGNAL(triggered()), this, SLOT(saveDataFile()));
    QObject::connect(&ui_toolbar, SIGNAL(selectDrainage(DrainageAlgorithms*)), &runnel_controller, SLOT(changeSelectDrainage(DrainageAlgorithms*)));
    QObject::connect(&ui_toolbar, SIGNAL(selectPatron(AlgorithmPatron*)), &runnel_controller, SLOT(changeSelectPatron(AlgorithmPatron*)));
    QObject::connect(&ui_toolbar, SIGNAL(selectWater(PathWaterAlgorithm*)), &runnel_controller, SLOT(changeSelectWater(PathWaterAlgorithm*)));
    QObject::connect(&ui_toolbar, SIGNAL(selectNetwork(BuildNetwork*)), &runnel_controller, SLOT(changeSelectNetwork(BuildNetwork*)));
    QObject::connect(&ui_toolbar, SIGNAL(changeElevation(int)), &runnel_controller.getPainter(), SLOT(changeExag(int)));
    QObject::connect(&ui_toolbar, SIGNAL(changeLandForm(int)), &runnel_controller.getPainter(), SLOT(changeModelTerrain(int)));
    QObject::connect(&ui_toolbar, SIGNAL(showGradientVector(bool)), &runnel_controller.getPainter(), SLOT(showRenderGradientVector(bool)));
    QObject::connect(&ui_toolbar, SIGNAL(showNormalVector(bool)), &runnel_controller.getPainter(), SLOT(showRenderNormalVector(bool)));
    QObject::connect(&ui_toolbar, SIGNAL(showCoordinateAxis(bool)), &runnel_controller.getPainter(), SLOT(showRenderCoordinateAxis(bool)));
    QObject::connect(&runnel_controller, SIGNAL(glewIsReady()), &ui_toolbar, SLOT(glewIsReady()));
    this->connectColor();
}
void MainWindow::connectColor(){
    QObject::connect(ui->color_peucker_boton, SIGNAL(clicked()), this, SLOT(changeColor1()));
    QObject::connect(ui->color_angle_minimum_edge_button, SIGNAL(clicked()), this, SLOT(changeColor2()));
    QObject::connect(ui->color_angle_maximo_edge_button, SIGNAL(clicked()), this, SLOT(changeColor3()));
    QObject::connect(ui->color_angle_point_button, SIGNAL(clicked()), this, SLOT(changeColor4()));
    QObject::connect(ui->color_callaghan_button, SIGNAL(clicked()), this, SLOT(changeColor5()));
    QObject::connect(ui->color_drainage_button, SIGNAL(clicked()), this, SLOT(changeColor6()));
    QObject::connect(ui->color_normal_button, SIGNAL(clicked()), this, SLOT(changeColor7()));
    QObject::connect(ui->color_path_button, SIGNAL(clicked()), this, SLOT(changeColor8()));
    QObject::connect(ui->color_terrain_button, SIGNAL(clicked()), this, SLOT(changeColor9()));
}

void MainWindow::getTypeTIFFForObtainNameFile(){
    QString extension_tiff = "All Files (*);;tiff(*.tif)";
    std::cout << "Obtaining extension file " << std::endl;
    this->getObtainNameFile(extension_tiff, "TIFF");
}
void MainWindow::getTypeRunnelForObtainNameFile(){
    QString extension_runnel = "All Files (*);;Runnel(*.runnel)";
    std::cout << "Obtaining extension file... " << std::endl;
    this->getObtainNameFile(extension_runnel, "RUNNEL");
}

void MainWindow::getObtainNameFile(QString extension_name, std::string type_file){
    std::cout << "Obtaining name of file..." << std::endl;
    QString title_window = "Open File ";
    QString nameFile = QFileDialog::getOpenFileName(this, (title_window), "",extension_name );
    if (nameFile.size() == 0){
        return;
    }
    std::cout << "name of file is " << nameFile.toStdString() << std::endl;
    runnel_controller.obtainFileTerrain(nameFile, type_file);

}

void MainWindow::saveDataFile(){
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Save File"), "",
           tr("Runnel(*.runnel)"));

    if (fileName.size() == 0){
        return;
    }
    std::cout << "name of file is " << fileName.toStdString() << std::endl;
    runnel_controller.saveData(fileName);

}


void MainWindow::confColorRunnel(){

    ui->color_peucker->setStyleSheet(getColor(conf["shader_drainage_color_peucker"]));
    ui->color_callaghan->setStyleSheet(getColor(conf["shader_callaghan_color"]));
    ui->color_angle_minimum_edge->setStyleSheet(getColor(conf["shader_edge_color_min"]));
    ui->color_angle_maximo_edge->setStyleSheet(getColor(conf["shader_edge_color_max"]));
    ui->color_angle_point->setStyleSheet(getColor(conf["shader_point_color"]));
    ui->color_normal->setStyleSheet(getColor(conf["normal_color"]));
    ui->color_terrain->setStyleSheet(getColor(conf["shader_terrain_color"]));
    ui->color_drainage->setStyleSheet(getColor(conf["shader_drainage_color"]));
    ui->color_path->setStyleSheet(getColor(conf["color_gradient_path"]));
}


QString MainWindow::getColor(glm::vec3 value){
    std::stringstream color;
    color << "background-color:" << "rgb(" << value.x*255 << ", " << value.y*255 << "," << value.z*255 << ");";
    QString color_qstring =QString::fromUtf8(color.str().c_str());
    return color_qstring;
}
void MainWindow::changeColor(std::string name){
    QColor color = QColorDialog::getColor(Qt::white);

    if (color.isValid()){
       std::cout << color.toRgb().red() << " " << color.toRgb().green() << " " << color.toRgb().blue() << std::endl;
       conf[name] = glm::vec3(color.toRgb().red()/255.0f, color.toRgb().green()/255.0f, color.toRgb().blue()/255.0f);
       runnel_controller.changeColors(name, conf[name] );
    }
    this->confColorRunnel();
}





void MainWindow::changeColor1(){
    std::string name = "shader_drainage_color_peucker";
    this->changeColor(name);
}
void MainWindow::changeColor2(){
    std::string name = "shader_edge_color_min";
    this->changeColor(name);
}
void MainWindow::changeColor3(){
    std::string name = "shader_edge_color_max";
    this->changeColor(name);
}
void MainWindow::changeColor4(){
    std::string name = "shader_point_color";
    this->changeColor(name);
}
void MainWindow::changeColor5(){
    std::string name = "shader_callaghan_color";
    this->changeColor(name);
}
void MainWindow::changeColor6(){
    std::string name = "shader_drainage_color";
    this->changeColor(name);
}
void MainWindow::changeColor7(){
    std::string name = "normal_color";
    this->changeColor(name);
}
void MainWindow::changeColor8(){
    std::string name = "color_gradient_path";
    this->changeColor(name);
}
void MainWindow::changeColor9(){
    std::string name = "shader_terrain_color";
    this->changeColor(name);
}
