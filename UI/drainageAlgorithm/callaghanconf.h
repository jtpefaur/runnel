#ifndef CALLAGHANCONF_H
#define CALLAGHANCONF_H

#include <QWidget>

namespace Ui {
    class CallaghanConf;
}

class CallaghanConf : public QWidget
{
        Q_OBJECT

    public:
        explicit CallaghanConf(QWidget *parent = 0);
        ~CallaghanConf();
        float getWater();
        float getLineWater();

    signals:
        void changeAttr();


    private:
        Ui::CallaghanConf *ui;
};

#endif // CALLAGHANCONF_H
