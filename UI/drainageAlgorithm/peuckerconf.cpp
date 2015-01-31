#include "peuckerconf.h"
#include "ui_peuckerconf.h"

PeuckerConf::PeuckerConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeuckerConf)
{
    ui->setupUi(this);
}

PeuckerConf::~PeuckerConf()
{
    delete ui;
}
