#include "callaghannetworkconf.h"
#include "ui_callaghannetworkconf.h"

CallaghanNetworkConf::CallaghanNetworkConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallaghanNetworkConf)
{
    ui->setupUi(this);
    QObject::connect(ui->callaghan_network_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
}

CallaghanNetworkConf::~CallaghanNetworkConf()
{
    delete ui;
}


float CallaghanNetworkConf::getMaxWater(){
    return ui->callaghan_network_value->value();
}
