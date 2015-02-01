#include "peuckernetworkconf.h"
#include "ui_peuckernetworkconf.h"

PeuckerNetworkConf::PeuckerNetworkConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeuckerNetworkConf)
{
    ui->setupUi(this);
}

PeuckerNetworkConf::~PeuckerNetworkConf()
{
    delete ui;
}
