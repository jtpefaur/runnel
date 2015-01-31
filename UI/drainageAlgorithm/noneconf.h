#ifndef NONECONF_H
#define NONECONF_H

#include <QWidget>

namespace Ui {
    class NoneConf;
}

class NoneConf : public QWidget
{
        Q_OBJECT

    public:
        explicit NoneConf(QWidget *parent = 0);
        ~NoneConf();

    private:
        Ui::NoneConf *ui;
};

#endif // NONECONF_H
