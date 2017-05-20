/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


/*!
 *=============================================================================
 *
 * \brief MainWindow::_setupFileMenu
 *
 *=============================================================================
 */
void MainWindow::_setupFileMenu()
{
    _connectSubmenu = _ui->menuFile->addMenu("Connect Camera");
    _connectSubmenu->setIcon(QIcon(QPixmap(":/res/images/connectButton.png")));
    _disconnectSubmenu = _ui->menuFile->addMenu("Disconnect Camera");
    _disconnectSubmenu->setIcon(QIcon(QPixmap(":/res/images/disconnectButton.png")));

    _refreshCameraMenu();

    connect(_connectSubmenu, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowCamera()));
    connect(_connectSubmenu, SIGNAL(triggered(QAction*)), this, SLOT(slotConnectCamera(QAction *)));
    connect(_disconnectSubmenu, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowCamera()));
    connect(_disconnectSubmenu, SIGNAL(triggered(QAction*)), this, SLOT(slotDisconnectCamera(QAction *)));

    QAction *exitAction = _ui->menuFile->addAction("&Exit");
    exitAction->setIcon(QIcon(QPixmap(":/res/images/exitButton.png")));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(slotExit()));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_refreshCameraMenu
 *
 *=============================================================================
 */
void MainWindow::_refreshCameraMenu()
{
    std::vector<QString> remain;

    _mtxTimer.lock();

    _scanCamera();


    map< QString, QAction* >::iterator itAction;
    for (auto const&itAction : _cameraSelectAction)
        if (itAction.second)
            _ui->mainToolBar->removeAction(itAction.second);
    _cameraSelectAction.clear();


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
    std::map< QString, DevicePtr>::iterator itDevice;
    for (auto const&itDevice : _disconnected)
        _connectSubmenu->addAction(itDevice.first);

    std::map< QString, Grabber* >::iterator itCamera;
    for (auto const&itCamera : _connected)
    {
        QString name = itCamera.first;
        _disconnectSubmenu->addAction(name);

        QAction *action = _ui->mainToolBar->addAction(name);
        action->setCheckable(true);
        _cameraSelectAction[name] = action;

    }


    // Current camera's button should show 'selected'
    if (_currGrabber)
    {
        QString grabberName;
        if (_findKeyByValue< std::map<QString, Grabber*>, QString, Grabber* >(
                    _connected, _currGrabber, grabberName))
        {
            _cameraSelectAction[grabberName]->setChecked(true);
        }
    }

    _mtxTimer.unlock();

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotAboutToShowCamera
 *
 *=============================================================================
 */
void MainWindow::slotAboutToShowCamera()
{
    _refreshCameraMenu();
}



/*!
 *=============================================================================
 *
 * \brief MainWindow::slotConnectCamera
 * \param action
 *
 *=============================================================================
 */
void MainWindow::slotConnectCamera(QAction *action)
{
    QString name = action->text();

    map<QString, DevicePtr >::iterator it;
    for (auto const&it : _disconnected)
    {
        if (it.first == name)
        {
            DepthCameraPtr dc = _sys.connect(it.second);
            if (dc)
            {
                Grabber *grabber = new Grabber(dc, Grabber::FRAMEFLAG_ALL, _sys);
                _connected[name] = grabber;
                std::cout << "Connected to " << name.toStdString() << std::endl;
                _currGrabber = _connected[name];
                for (int i=0; i < 5; i++)
                {
                    if (_currGrabber->isInitialized() && !_currGrabber->isRunning())
                    {
                        _currGrabber->start();
                        _videoTimer->start(15);
                        _refreshParameterWindow();

                        goto ret;
                    }
                    sleep(1);
                }
            }
        }
    }
ret:
    _refreshCameraMenu();
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotDisconnectCamera
 * \param action
 *
 *=============================================================================
 */
void MainWindow::slotDisconnectCamera(QAction *action)
{
    bool findNewGrabber = false;

    map< QString, Grabber* >::iterator it;
    for (auto const&it : _connected)
    {
        QString name = it.first;
        if (name == action->text())
        {
            Grabber *grabber = it.second;

            if (grabber == _currGrabber)
                findNewGrabber = true;

            _sys.disconnect(grabber->getDepthCamera());
            delete grabber;
            _connected.erase(name);
            std::cout << "Disconnected from " << action->text().toStdString() << std::endl;
            goto ret;
        }
    }

ret:
    if (findNewGrabber)
    {
        if (_connected.size() > 0)
        {
            _currGrabber = _connected.begin()->second;
            std::cout << "Found new grabber" << std::endl;
        }
        else
        {
            _currGrabber = NULL;
            std::cout << "No more grabbers" << std::endl;
        }
        _refreshParameterWindow();

    }

    _refreshCameraMenu();
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotExit
 *
 *=============================================================================
 */
void MainWindow::slotExit()
{
    map< QString, Grabber* >::iterator itCamera;
    for (auto const&itCamera : _connected)
    {
        Grabber *grabber = itCamera.second;
        if (grabber)
        {
            _sys.disconnect(grabber->getDepthCamera());
            grabber->stop();
            delete grabber;
        }
    }
    exit(0);
}
