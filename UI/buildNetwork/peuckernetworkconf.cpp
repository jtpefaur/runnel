#include "peuckernetworkconf.h"
#include "ui_peuckernetworkconf.h"

PeuckerNetworkConf::PeuckerNetworkConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeuckerNetworkConf)
{
    ui->setupUi(this);
    QObject::connect(ui->order_threshold_button, SIGNAL(clicked()), this, SIGNAL(changeOrderThreshold()));
}

PeuckerNetworkConf::~PeuckerNetworkConf()
{
    delete ui;
}

int PeuckerNetworkConf::getOrderThreshold() {
    return ui->order_threshold_value->value();
}
