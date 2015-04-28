#include "normalvectorsimilarityconf.h"
#include "ui_normalvectorsimilarityconf.h"

NormalVectorSimilarityConf::NormalVectorSimilarityConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NormalVectorSimilarityConf)
{
    ui->setupUi(this);
    QObject::connect(ui->threshold_button, SIGNAL(clicked()), this, SIGNAL(changeAttr()));
}

NormalVectorSimilarityConf::~NormalVectorSimilarityConf()
{
    delete ui;
}

float NormalVectorSimilarityConf::getAngleThreshold(){
    return ui->threshold_value->value();
}
