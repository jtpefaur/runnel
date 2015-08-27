#ifndef RWFLOODNETWORKCONF_H
#define RWFLOODNETWORKCONF_H

#include <QWidget>

namespace Ui {
class RWFloodNetworkConf;
}

class RWFloodNetworkConf : public QWidget
{
    Q_OBJECT

public:
    explicit RWFloodNetworkConf(QWidget *parent = 0);
    ~RWFloodNetworkConf();

    int getWaterLevelThreshold();

signals:
    int changeWaterLevelThreshold();

private:
    Ui::RWFloodNetworkConf *ui;
};

#endif // RWFLOODNETWORKCONF_H
