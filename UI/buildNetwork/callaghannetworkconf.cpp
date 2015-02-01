#include "callaghannetworkconf.h"
#include "ui_callaghannetworkconf.h"

CallaghanNetworkConf::CallaghanNetworkConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallaghanNetworkConf)
{
    ui->setupUi(this);
}

CallaghanNetworkConf::~CallaghanNetworkConf()
{
    delete ui;
}
