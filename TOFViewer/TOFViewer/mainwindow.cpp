/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _setupMenu();
    _setupTimer(1000);
}


uint MainWindow::_numConnectedCameras()
{
    uint count = 0;
    map< QString, Grabber* >::iterator it;
    for (auto const&it : _attachedCamera)
    {
        if (it.second != NULL)
            count++;
    }
    return count;
}


void MainWindow::_updateCameraMenu()
{
    map< QString, Grabber* > plugged;    // Attached but not connected
    map< QString, Grabber* > connected;  // Attached and connected
    map< QString, Grabber* > unplugged;  // Unattached

    _mtxTimer.lock();

    _device = _sys.scan();

    // Get plugged set find connected
    for (uint i = 0; i < _device.size(); i++)
    {
        QString name = QString(_device[i]->serialNumber().c_str());
        plugged[name] = NULL;
        if (_attachedCamera.find(name) != _attachedCamera.end())
        {
            plugged[name] = connected[name] = _attachedCamera[name];
        }
    }

    // From unplugged set
    map< QString, Grabber* >::iterator itCamera;
    for (auto const&itCamera : connected)
    {
        QString name = itCamera.first;
        if ( _attachedCamera.find(name) == _attachedCamera.end() )
            unplugged[name] = _attachedCamera[name];
    }

    _attachedCamera = plugged;


    // Remove MainToolBar buttons and actions
    map< QString, QPushButton* >::iterator itButton;
    for (auto const&itButton :_cameraSelectButton)
        delete itButton.second;
    _cameraSelectButton.clear();

    map< QString, QAction* >::iterator itAction;
    for (auto const&itAction : _cameraSelectButtonAction)
        _ui->mainToolBar->removeAction(itAction.second);
    _cameraSelectButtonAction.clear();


    // Clear connected and disconnected lists
    QList<QAction *> list = _disconnectSubmenu->actions();
    while (!list.isEmpty())
    {
        QAction *action = list.front();
        _disconnectSubmenu->removeAction(action);
        list.pop_front();
        delete action;
    }

    list = _connectSubmenu->actions();
    while (!list.isEmpty())
    {
        QAction *action = list.front();
        _connectSubmenu->removeAction(action);
        list.pop_front();
        delete action;
    }


    // Add back the updated cameras to each list
    for (auto const&itCamera : _attachedCamera)
    {
        QString name = itCamera.first;
        if (itCamera.second == NULL)
        {
            _connectSubmenu->addAction(name);
        }
        else
        {
            QPushButton *button = new QPushButton(name);
            _cameraSelectButton[name] = button;
            _cameraSelectButtonAction[name] = _ui->mainToolBar->addAction(name);

            _disconnectSubmenu->addAction(name);
        }
    }

    _mtxTimer.unlock();
}


void MainWindow::_setupMenu()
{
    _connectSubmenu = _ui->menuFile->addMenu("Connect Camera");
    _connectSubmenu->setIcon(QIcon(QPixmap(":/res/images/connectButton.png")));
    _disconnectSubmenu = _ui->menuFile->addMenu("Disconnect Camera");
    _disconnectSubmenu->setIcon(QIcon(QPixmap(":/res/images/disconnectButton.png")));

    _updateCameraMenu();

    connect(_connectSubmenu, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowCamera()));
    connect(_connectSubmenu, SIGNAL(triggered(QAction*)), this, SLOT(slotConnectCamera(QAction *)));
    connect(_disconnectSubmenu, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowCamera()));
    connect(_disconnectSubmenu, SIGNAL(triggered(QAction*)), this, SLOT(slotDisconnectCamera(QAction *)));

    QAction *exitAction = _ui->menuFile->addAction("&Exit");
    exitAction->setIcon(QIcon(QPixmap(":/res/images/exitButton.png")));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(slotExit()));

    // Connect sliders
    connect(_ui->unambDistSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateUnambDist(int)));
    connect(_ui->frameRateSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateFrameRate(int)));
    connect(_ui->integDutyCycleSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIntegDutyCycle(int)));
    connect(_ui->illumPwrSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIllumPwr(int)));

    // Camera play control actions
    connect(_ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(slotMainToolBar(QAction *)));
}



void MainWindow::_setupTimer(int msec)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    _timer->start(msec);

    _videoTimer = new QTimer(this);
    connect(_videoTimer, SIGNAL(timeout()), this, SLOT(slotVideoTimeout()));
}


MainWindow::~MainWindow()
{
    delete _timer;
    delete _ui;
}


