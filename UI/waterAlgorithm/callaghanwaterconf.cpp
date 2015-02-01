#include "callaghanwaterconf.h"
#include "ui_callaghanwaterconf.h"

CallaghanWaterConf::CallaghanWaterConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallaghanWaterConf)
{
    ui->setupUi(this);
}

CallaghanWaterConf::~CallaghanWaterConf()
{
    delete ui;
}
