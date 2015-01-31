#ifndef PEUCKERCONF_H
#define PEUCKERCONF_H

#include <QWidget>

namespace Ui {
    class PeuckerConf;
}

class PeuckerConf : public QWidget
{
        Q_OBJECT

    public:
        explicit PeuckerConf(QWidget *parent = 0);
        ~PeuckerConf();

    private:
        Ui::PeuckerConf *ui;
};

#endif // PEUCKERCONF_H
