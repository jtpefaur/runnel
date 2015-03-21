#include "callaghanconf.h"
#include "ui_callaghanconf.h"

CallaghanConf::CallaghanConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallaghanConf)
{
    ui->setupUi(this);
    QObject::connect(ui->delta_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
    QObject::connect(ui->line_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
}

CallaghanConf::~CallaghanConf()
{
    delete ui;
}

float CallaghanConf::getWater(){
    return ui->delta_water->value();
}

float CallaghanConf::getLineWater(){
    return ui->line_water->value();
}


