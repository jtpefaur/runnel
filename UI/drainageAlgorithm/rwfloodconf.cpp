#include "rwfloodconf.h"
#include "ui_rwfloodconf.h"

RWFloodConf::RWFloodConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RWFloodConf)
{
    ui->setupUi(this);
    QObject::connect(ui->threshold_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
}

RWFloodConf::~RWFloodConf()
{
    delete ui;
}

int RWFloodConf::getWaterThreshold()
{
    return ui->threshold_value->value();
}
