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
#include <QStandardItemModel>


#include "CameraSystem.h"
#include "opencv2/opencv.hpp"
#include "grabber.h"

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

    void slotSetProfile(QAction *action);
    void slotAboutToShowProfileMenu();

    void slotWindowsAction(QAction *action);

    void slotInfoDockWidgetVisChanged(bool checked);
    void slotImageDockWidgetVisChanged(bool checked);
    void slotSettingsDockWidgetVisChanged(bool checked);

    void slotTabBarClicked(int);

protected:   
    void _setupMenu();
    void _setupFileMenu();
    void _setupWindowsMenu();
    void _setupSliders();
    void _setupMenuToolBar();
    void _setupProfileMenu();
    void _setupTimer();
    void _setupDockWidgets();
    void _setupParameterWindow();

    void _refreshCameraMenu();
    void _refreshProfileMenu();
    void _refreshParameterWindow();


    void _scanCamera();
    void _addMainToolBarButton(QString name);
    uint _numConnectedCameras();
    bool _isIn(QString name, vector<QString> &vec);

    template<typename MapT, typename KeyT, typename ValueT>
    bool _findKeyByValue(MapT &map, ValueT value, KeyT &key)
    {
        bool ret = false;
        typename MapT::iterator it;
        for (it = map.begin(); it != map.end(); ++it )
        {
            if (it->second == value)
            {
                key = it->first;
                ret = true;
                break;
            }
        }
        return ret;
    }

private:
    Ui::MainWindow *_ui;
    QMenu *_connectSubmenu;
    QMenu *_disconnectSubmenu;
    vector<QMenu *> _profileSubmenu;

    Voxel::CameraSystem _sys;
    std::vector< DevicePtr > _device;
    std::map< QString, DevicePtr> _disconnected;
    std::map< QString, Grabber* > _connected;
    std::map< QString, QAction* > _cameraSelectAction;
    Grabber * _currGrabber;

    QTimer *_timer;
    QTimer *_videoTimer;
    QMutex _mtxTimer;

    QStandardItemModel *_paramModel;


};

#endif // MAINWINDOW_H
