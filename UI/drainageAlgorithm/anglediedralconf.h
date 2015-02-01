#ifndef ANGLEDIEDRALCONF_H
#define ANGLEDIEDRALCONF_H

#include <QWidget>

namespace Ui {
    class AngleDiedralConf;
}

class AngleDiedralConf : public QWidget
{
        Q_OBJECT

    public:
        explicit AngleDiedralConf(QWidget *parent = 0);
        ~AngleDiedralConf();

        float getLineWidth();
        float getMaxAngle();
        float getMinAngle();

    signals:
        void changeAttr();

    private:
        Ui::AngleDiedralConf *ui;
};

#endif // ANGLEDIEDRALCONF_H
