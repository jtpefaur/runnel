#ifndef RWFLOODCONF_H
#define RWFLOODCONF_H

#include <QWidget>

namespace Ui {
class RWFloodConf;
}

class RWFloodConf : public QWidget
{
    Q_OBJECT

public:
    explicit RWFloodConf(QWidget *parent = 0);
    ~RWFloodConf();

    int getWaterThreshold();

signals:
    void changeAttr();

private:
    Ui::RWFloodConf *ui;
};

#endif // RWFLOODCONF_H
