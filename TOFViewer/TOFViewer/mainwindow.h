#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <QMenu>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QMutex>
#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QLineF>
#include <QRgb>

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

    void RawFrameToMat(ToFRawFrame *frame, int rows, int cols,
                        cv::Mat &phase, cv::Mat &amplitude);

    bool DepthFrameToMat(DepthFrame *frame, int rows, int cols,
                        cv::Mat &depth, cv::Mat &amplitude);

    bool MatToQImage(cv::Mat &mat, QImage &image);

    bool MatToQPixmap(cv::Mat &mat, QPixmap &pixMap );

public slots:
    void slotUpdateUnambDist(int value);
    void slotUpdateFrameRate(int value);
    void slotUpdateIntegDutyCycle(int value);
    void slotUpdateIllumPwr(int value);

    void slotConnectCamera(QAction *action);
    void slotDisconnectCamera(QAction *action);
    void slotAboutToShowCamera();
    void slotExit();

    void slotMainToolBar(QAction *);

    void slotTimeout();
    void slotVideoTimeout();


protected:
    void _updateCameraMenu();
    void _setupMenu();
    void _setupTimer(int msec);
    void _addMainToolBarButton(QString name);
    uint _numConnectedCameras();
    void grabberCallback(Grabber *grabber);

private:
    Ui::MainWindow *_ui;
    QMenu *_connectSubmenu;
    QMenu *_disconnectSubmenu;

    Voxel::CameraSystem _sys;
    std::vector< DevicePtr > _device;
    std::map< QString, Grabber* > _attachedCamera;
    std::map< QString, QPushButton* > _cameraSelectButton;
    std::map< QString, QAction* > _cameraSelectButtonAction;

    QTimer *_timer;
    QTimer *_videoTimer;
    QMutex _mtxTimer;
};

#endif // MAINWINDOW_H
