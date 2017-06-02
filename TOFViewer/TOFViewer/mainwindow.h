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

#define MAX_ZDEPTH      20000   // 20m

#define INFO_DATAFLOW_TAB   0
#define INFO_WATCH_TAB      1
#define INFO_RECORD_TAB     2
#define INFO_LOG_TAB        3

#define INFO_BOX_WIDTH      50
#define INFO_BOX_HEIGHT     30
#define INFO_LINE_WIDTH     20
#define INFO_ARROW_SIZE     5

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
    void slotSliderUnambDist(int value);
    void slotSliderFrameRate(int value);
    void slotSliderIntegDutyCycle(int value);
    void slotSliderIllumPwr(int value);

    void slotEditedUnambDist();
    void slotEditedFrameRate();
    void slotEditedIntegDutyCycle();
    void slotEditedIllumPwr();

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
    void slotParamSearchChanged(QString text);
    void slotViewDataChanged(int row, int col);

    void slotPhaseGainSliderMoved(int value);
    void slotAmpGainSliderMoved(int value);

    void slotLowerLimitSliderMoved(int value);
    void slotUpperLimitSliderMoved(int value);

    void slotInfoTabBarClicked(int tab);
    void slotSizeChangedUpdate(QSize s);

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
    void _setupInfoDockWidget();
    void _setupDataFlowWindow();

    void _refreshCameraMenu();
    void _refreshProfileMenu();
    void _refreshParameterWindow();
    void _refreshSliders();
    void _refreshDataFlowWindow();

    void _scanCamera();
    void _addMainToolBarButton(QString name);
    bool _isIn(QString name, vector<QString> &vec);
    void _updateParamModel(Grabber *grabber);
    void _setCurrGrabber(Grabber *grabber);
    QPoint _drawBox(QGraphicsScene &scene, QString label, QPoint &org, int w, int h);
    QPoint _drawArrow(QGraphicsScene &scene, QPoint &org, int length);

    template<typename T>
    QPoint _drawFilterChain(QGraphicsScene &scene, const FilterSet<T> &set,
                            QString name, QPoint org,
                            int boxWidth, int boxHeight, int lineLength)
    {
        QPoint p = _drawBox(scene, name, org, boxWidth, boxHeight);

        for (auto i = 0; i < set.size(); i++)
        {
            QString name = QString(set.getFilter(i).get()->name().c_str());
            p = _drawArrow(scene, p, lineLength);
            p = _drawBox(scene, name, p, boxWidth, boxHeight);
        }

        return p;
    }


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

    template<typename MapT, typename KeyT>
    void _findPrefix(MapT &inMap, KeyT &searchFor, MapT &outMap)
    {
        outMap.clear();

        typename MapT::const_iterator i;
        for (i = inMap.begin(); i != inMap.end(); i++)
        {
            const KeyT &key = i->first;
            if (key.compare(0, searchFor.size(), searchFor) == 0)
                outMap[i->first] = i->second;
        }
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

    QGraphicsScene _phaseScene;
    QGraphicsScene _ampScene;
    QGraphicsScene _dataflowScene;

    QTimer *_timer;
    QTimer *_videoTimer;
    QMutex _mtxTimer;

    int _phaseGain;
    float _ampGain;

    int _zLowerLimit, _zUpperLimit;
};

#endif // MAINWINDOW_H
