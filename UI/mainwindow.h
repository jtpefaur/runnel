#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <iostream>
#include "toolbarsconfigmesh.h"
#include "runnelcontroller.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        RunnelController runnel_controller;
        void connectSignalForRunnel();
        void confColorRunnel();
        std::unordered_map<std::string, glm::vec3> conf;
        QString getColor(glm::vec3 value);
        void changeColor(std::string name);
        void connectGoogleMapWeb();
        void connectColor();
        void gradientConf();

        ~MainWindow();

     public slots:
        void getObtainNameFile(QString extension_name, std::string type_file);
        void getTypeRunnelForObtainNameFile();
        void getTypeTIFFForObtainNameFile();
        void changeColor1();
        void changeColor2();
        void changeColor3();
        void changeColor4();
        void changeColor5();
        void changeColor6();
        void changeColor7();
        void changeColor8();
        void changeColor9();




    signals:
        void OpenFile();

    private:
        Ui::MainWindow *ui;
        ToolbarsConfigMesh ui_toolbar;
};

#endif // MAINWINDOW_H
