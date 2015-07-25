#include "garbrechtmartzconf.h"
#include "ui_garbrechtmartzconf.h"

GarbrechtMartzConf::GarbrechtMartzConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GarbrechtMartzConf)
{
    ui->setupUi(this);
}

GarbrechtMartzConf::~GarbrechtMartzConf()
{
    delete ui;
}
