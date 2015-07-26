#include "garbrechtmartzconf.h"
#include "ui_garbrechtmartzconf.h"

GarbrechtMartzConf::GarbrechtMartzConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GarbrechtMartzConf)
{
    ui->setupUi(this);

    QObject::connect(ui->garbrecht_martz_button, SIGNAL(clicked()), this, SIGNAL(runFlatResolution()));
    QObject::connect(ui->garbrecht_martz_value, SIGNAL(valueChanged(double)), this, SIGNAL(changeIncrementation(double)));
}

GarbrechtMartzConf::~GarbrechtMartzConf()
{
    delete ui;
}
