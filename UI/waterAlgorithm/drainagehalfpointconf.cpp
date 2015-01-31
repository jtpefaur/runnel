#include "drainagehalfpointconf.h"
#include "ui_drainagehalfpointconf.h"

DrainageHalfPointConf::DrainageHalfPointConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrainageHalfPointConf)
{
    ui->setupUi(this);
}

DrainageHalfPointConf::~DrainageHalfPointConf()
{
    delete ui;
}
