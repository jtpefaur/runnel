#include "noneconf.h"
#include "ui_noneconf.h"

NoneConf::NoneConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoneConf)
{
    ui->setupUi(this);
}

NoneConf::~NoneConf()
{
    delete ui;
}
