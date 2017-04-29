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
    _setupSignalSlot();
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

    // Clear connected and disconnected camera lists
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
            _cameraSelectButtonAction[name] = _ui->mainToolBar->addWidget(button);
            _disconnectSubmenu->addAction(name);
        }
    }
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
}



void MainWindow::_setupSignalSlot()
{
    // Connect sliders
    connect(_ui->unambDistSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateUnambDist(int)));
    connect(_ui->frameRateSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateFrameRate(int)));
    connect(_ui->integDutyCycleSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIntegDutyCycle(int)));
    connect(_ui->illumPwrSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIllumPwr(int)));
}


MainWindow::~MainWindow()
{
    delete _ui;
}


void MainWindow::slotAboutToShowCamera()
{
    _updateCameraMenu();
}


void MainWindow::slotConnectCamera(QAction *action)
{
    for (uint i = 0; i < _device.size(); i++)
    {
        QString name = QString(_device[i]->serialNumber().c_str());

        if (name == action->text())
        {
            if (_attachedCamera[name] == NULL) // look for unconnected cameras only
            {
                DepthCameraPtr dc = _sys.connect(_device[i]);
                if (dc)
                {
                    Grabber *grabber = new Grabber(dc, DepthCamera::FRAME_DEPTH_FRAME, _sys);
                    _attachedCamera[name] = grabber;
                    std::cout << "Connected to " << name.toStdString() << std::endl;
                    goto ret;
                }
            }
        }
    }
ret:
    _updateCameraMenu();
}


void MainWindow::slotDisconnectCamera(QAction *action)
{
    map< DevicePtr, Grabber* >::iterator it;
    for (auto const&it : _attachedCamera)
    {
        if (it.second != NULL) // look for connected cameras only
        {
            QString name = it.first;
            if (name == action->text())
            {
                Grabber *grabber = it.second;
                _sys.disconnect(grabber->getDepthCamera());
                delete grabber;
                _attachedCamera[name] = NULL;

                std::cout << "Disconnected from " << action->text().toStdString() << std::endl;
                goto ret;
            }
        }
    }
ret:
    _updateCameraMenu();
}



void MainWindow::slotExit()
{
    map< QString, Grabber* >::iterator it;

    for (auto const&it : _attachedCamera)
    {
        Grabber *grabber = it.second;

        if (grabber)
        {
            _sys.disconnect(grabber->getDepthCamera());
            delete grabber;
        }
    }
    exit(0);
}


void MainWindow::slotUpdateUnambDist(int value)
{
    _ui->unambDistEdit->setText(QString(std::to_string(value).c_str()));
}


void MainWindow::slotUpdateFrameRate(int value)
{
    _ui->frameRateEdit->setText(QString(std::to_string(value).c_str()));
}


void MainWindow::slotUpdateIntegDutyCycle(int value)
{
    _ui->integDutyCycleEdit->setText(QString(std::to_string(value).c_str()));
}


void MainWindow::slotUpdateIllumPwr(int value)
{
    _ui->illumPwrEdit->setText(QString(std::to_string(value).c_str()));
}

