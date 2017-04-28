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

    scanCameras();

    initMenu();

    initSignalSlot();
}


void MainWindow::initMenu()
{
    map< QString, Grabber* >::iterator it;

    _connectSubmenu = _ui->menuFile->addMenu("Connect Camera");
    connect(_connectSubmenu, SIGNAL(triggered(QAction*)), this, SLOT(slotConnectCamera(QAction *)));

    _disconnectSubmenu = _ui->menuFile->addMenu("Disconnect Camera");
    connect(_disconnectSubmenu, SIGNAL(triggered(QAction*)), this, SLOT(slotDisconnectCamera(QAction *)));

    for (auto const&it : _attachedCamera)
    {
        if (it.second == NULL)
            _connectSubmenu->addAction(it.first);
        else
            _disconnectSubmenu->addAction(it.first);
    }

    QAction *exitAction = _ui->menuFile->addAction("&Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(slotExit()));
}



void MainWindow::initSignalSlot()
{
    // Connect sliders
    connect(_ui->unambDistSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateUnambDist(int)));
    connect(_ui->frameRateSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateFrameRate(int)));
    connect(_ui->integDutyCycleSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIntegDutyCycle(int)));
    connect(_ui->illumPwrSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIllumPwr(int)));
}


uint MainWindow::scanCameras()
{
    std::vector<DevicePtr> device = _sys.scan();

    for (uint i = 0; i < device.size(); i++)
    {
        DepthCameraPtr dc = _sys.connect(device[i]);
        if (dc)
        {
            std::string serial;
            if (dc->getSerialNumber(serial))
                if (_attachedCamera[QString(serial.c_str())] != NULL )
                    continue;
        }
        _sys.disconnect(dc);
    }
    return _attachedCamera.size();
}


MainWindow::~MainWindow()
{
    delete _ui;
}


void MainWindow::slotConnectCamera(QAction *action)
{
    std::vector<DevicePtr> device = _sys.scan();

    for (uint i = 0; i < device.size(); i++)
    {
        DepthCameraPtr dc = _sys.connect(device[i]);
        if (dc)
        {
            std::string serial;
            Grabber *grabber = new Grabber(dc, DepthCamera::FRAME_DEPTH_FRAME, _sys);
            if (grabber->getSerialNumber(serial))
            {
                QString name = QString(serial.c_str());
                if (name == action->text())
                {
                    _attachedCamera[name] = grabber;
                    _disconnectSubmenu->addAction(action->text());
                    _connectSubmenu->removeAction(action);
                    std::cout << "Connected to " << name.toStdString() << std::endl;
                    return;
                }
            }
        }
        _sys.disconnect(dc);
    }
}


void MainWindow::slotDisconnectCamera(QAction *action)
{
    Grabber *grabber = _attachedCamera[action->text()];
    if (grabber)
    {
        _disconnectSubmenu->removeAction(action);
        _sys.disconnect(grabber->getDepthCamera());
        _attachedCamera[action->text()] = NULL;
        _connectSubmenu->addAction(action->text());
        _disconnectSubmenu->removeAction(action);
        delete grabber;
        std::cout << "Disconnected from " << action->text().toStdString() << std::endl;
    }
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

