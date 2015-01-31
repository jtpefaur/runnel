#include "zhangguilberconf.h"
#include "ui_zhangguilberconf.h"

ZhangGuilberConf::ZhangGuilberConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZhangGuilberConf)
{
    ui->setupUi(this);
}

ZhangGuilberConf::~ZhangGuilberConf()
{
    delete ui;
}
