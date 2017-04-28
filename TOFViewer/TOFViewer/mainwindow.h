#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <QMenu>
#include <QMainWindow>

#include "CameraSystem.h"
#include "opencv2/opencv.hpp"
#include "Grabber.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotUpdateUnambDist(int value);
    void slotUpdateFrameRate(int value);
    void slotUpdateIntegDutyCycle(int value);
    void slotUpdateIllumPwr(int value);

    void slotConnectCamera(QAction *action);
    void slotDisconnectCamera(QAction *action);
    void slotExit();


protected:
    uint scanCameras();
    void initMenu();
    void initSignalSlot();

private:
    Ui::MainWindow *_ui;
    QMenu *_connectSubmenu;
    QMenu *_disconnectSubmenu;

    Voxel::CameraSystem _sys;
    std::map< QString, Grabber* > _attachedCamera;
};

#endif // MAINWINDOW_H
