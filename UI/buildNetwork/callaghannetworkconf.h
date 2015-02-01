#ifndef CALLAGHANNETWORKCONF_H
#define CALLAGHANNETWORKCONF_H

#include <QWidget>

namespace Ui {
    class CallaghanNetworkConf;
}

class CallaghanNetworkConf : public QWidget
{
        Q_OBJECT

    public:
        explicit CallaghanNetworkConf(QWidget *parent = 0);
        ~CallaghanNetworkConf();
        float getMaxWater();

    signals:
        void changeAttr();

    private:
        Ui::CallaghanNetworkConf *ui;
};

#endif // CALLAGHANNETWORKCONF_H
