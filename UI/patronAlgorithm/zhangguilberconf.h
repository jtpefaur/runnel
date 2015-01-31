#ifndef ZHANGGUILBERCONF_H
#define ZHANGGUILBERCONF_H

#include <QWidget>

namespace Ui {
    class ZhangGuilberConf;
}

class ZhangGuilberConf : public QWidget
{
        Q_OBJECT

    public:
        explicit ZhangGuilberConf(QWidget *parent = 0);
        ~ZhangGuilberConf();

    private:
        Ui::ZhangGuilberConf *ui;
};

#endif // ZHANGGUILBERCONF_H
