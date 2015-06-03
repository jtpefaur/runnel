#include "peuckernetworkconf.h"
#include "ui_peuckernetworkconf.h"

PeuckerNetworkConf::PeuckerNetworkConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeuckerNetworkConf)
{
    ui->setupUi(this);
    QObject::connect(ui->threshold_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
}

PeuckerNetworkConf::~PeuckerNetworkConf()
{
    delete ui;
}

int PeuckerNetworkConf::getOrderThreshold() {
    return ui->threshold_value->value();
}
