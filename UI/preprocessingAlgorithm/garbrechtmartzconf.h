#ifndef GARBRECHTMARTZCONF_H
#define GARBRECHTMARTZCONF_H

#include <QWidget>

namespace Ui {
class GarbrechtMartzConf;
}

class GarbrechtMartzConf : public QWidget
{
    Q_OBJECT

public:
    explicit GarbrechtMartzConf(QWidget *parent = 0);
    ~GarbrechtMartzConf();

private:
    Ui::GarbrechtMartzConf *ui;
};

#endif // GARBRECHTMARTZCONF_H
