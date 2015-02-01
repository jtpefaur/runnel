#include "anglediedralconf.h"
#include "ui_anglediedralconf.h"

AngleDiedralConf::AngleDiedralConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AngleDiedralConf)
{
    ui->setupUi(this);
    QObject::connect(ui->line_width_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
    QObject::connect(ui->max_angle_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
    QObject::connect(ui->min_angle_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
}

AngleDiedralConf::~AngleDiedralConf()
{
    delete ui;
}

float AngleDiedralConf::getLineWidth(){
    return ui->line_width_value->value();
}

float AngleDiedralConf::getMaxAngle(){
    return ui->max_angle_value->value();
}

float AngleDiedralConf::getMinAngle(){
    return ui->min_angle_value->value();
}
