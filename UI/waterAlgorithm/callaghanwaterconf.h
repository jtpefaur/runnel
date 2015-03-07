#ifndef CALLAGHANWATERCONF_H
#define CALLAGHANWATERCONF_H

#include <QWidget>

namespace Ui {
    class CallaghanWaterConf;
}

class CallaghanWaterConf : public QWidget
{
        Q_OBJECT

    public:
        explicit CallaghanWaterConf(QWidget *parent = 0);
        ~CallaghanWaterConf();
        float getDelta();

    private:
        Ui::CallaghanWaterConf *ui;
};

#endif // CALLAGHANWATERCONF_H
