#include "anglediedralconf.h"
#include "ui_anglediedralconf.h"

AngleDiedralConf::AngleDiedralConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AngleDiedralConf)
{
    ui->setupUi(this);
}

AngleDiedralConf::~AngleDiedralConf()
{
    delete ui;
}
