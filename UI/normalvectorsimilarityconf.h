#ifndef NORMALVECTORSIMILARITYCONF_H
#define NORMALVECTORSIMILARITYCONF_H

#include <QWidget>

namespace Ui {
class NormalVectorSimilarityConf;
}

class NormalVectorSimilarityConf : public QWidget
{
    Q_OBJECT

public:
    explicit NormalVectorSimilarityConf(QWidget *parent = 0);
    ~NormalVectorSimilarityConf();

    float getAngleThreshold();

signals:
    void changeAttr();

private:
    Ui::NormalVectorSimilarityConf *ui;

};

#endif // NORMALVECTORSIMILARITYCONF_H
