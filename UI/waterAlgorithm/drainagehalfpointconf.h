#ifndef DRAINAGEHALFPOINTCONF_H
#define DRAINAGEHALFPOINTCONF_H

#include <QWidget>

namespace Ui {
    class DrainageHalfPointConf;
}

class DrainageHalfPointConf : public QWidget
{
        Q_OBJECT

    public:
        explicit DrainageHalfPointConf(QWidget *parent = 0);
        ~DrainageHalfPointConf();

    private:
        Ui::DrainageHalfPointConf *ui;
};

#endif // DRAINAGEHALFPOINTCONF_H
