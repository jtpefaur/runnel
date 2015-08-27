#include "rwfloodnetworkconf.h"
#include "ui_rwfloodnetworkconf.h"

RWFloodNetworkConf::RWFloodNetworkConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RWFloodNetworkConf)
{
    ui->setupUi(this);
    QObject::connect(ui->water_level_threshold_button, SIGNAL(clicked()), this, SIGNAL(changeWaterLevelThreshold()));
}

RWFloodNetworkConf::~RWFloodNetworkConf()
{
    delete ui;
}

int RWFloodNetworkConf::getWaterLevelThreshold() {
    return ui->water_level_threshold_value->value();
}
