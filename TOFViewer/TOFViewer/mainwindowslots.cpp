/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


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
                    Grabber *grabber = new Grabber(dc, Grabber::FRAMEFLAG_ALL, _sys);
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


void MainWindow::slotMainToolBar(QAction *action)
{
    if (action->text() == "Run")
    {
        std::cout << "slotMainToolBar() Run " << std::endl;
        map< QString, Grabber* >::iterator itCamera;
        for (auto const&itCamera : _attachedCamera)
        {
            Grabber *grabber = itCamera.second;

            if (grabber != NULL)
            {
                if (!grabber->isRunning())
                {
                    grabber->start();
                }
            }
        }
        _videoTimer->start(15);
    }
    else if (action->text() == "Pause")
    {
        std::cout << "slotMainToolBar() Pause " << std::endl;
        map< QString, Grabber* >::iterator itCamera;
        for (auto const&itCamera : _attachedCamera)
        {
            Grabber *grabber = itCamera.second;

            if (grabber != NULL)
            {
                if (grabber->isRunning())
                {
                    grabber->stop();
                }
            }
        }
        _videoTimer->stop();
    }
    else
    {
        std::cout << "slotMainToolBar() Toggle Camera " << action->text().toStdString() << std::endl;

    }
}


