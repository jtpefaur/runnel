#include "rwfloodconf.h"
#include "ui_rwfloodconf.h"

RWFloodConf::RWFloodConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RWFloodConf)
{
    ui->setupUi(this);
}

RWFloodConf::~RWFloodConf()
{
    delete ui;
}
