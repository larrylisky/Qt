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
 * \brief MainWindow::_isIn
 * \param name
 * \param vec
 * \return
 *
 *=============================================================================
 */
bool MainWindow::_isIn(QString name, vector<QString> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        if (vec[i] == name)
            return true;

    return false;
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_scanCamera
 *
 *=============================================================================
 */
void MainWindow::_scanCamera()
{
    std::vector<QString> remain;
    _device = _sys.scan();

    // Find all attached but disconnected cameras
    remain.clear();
    _disconnected.clear();

    for (uint i = 0; i < _device.size(); i++)
    {
        QString name = QString(_device[i]->serialNumber().c_str());

        if (_connected.find(name) == _connected.end())
            _disconnected[name] = _device[i];
        else
            remain.push_back(name);
    }

    // Find all newly detached cameras and delete their Grabber instance
    map< QString, Grabber* >::iterator itCamera;
    for (auto const&itCamera : _connected)
    {
        QString name = itCamera.first;
        if (!_isIn(name, remain))
        {
            if (_connected[name])
                delete _connected[name];
            _connected.erase(name);
        }
    }

    // Reestablish _currGrabber in case _currGrabber was disconnected
    QString key;
    if (!_findKeyByValue< std::map< QString, Grabber* >, QString, Grabber* >(_connected, _currGrabber, key))
    {
        if (_connected.size() > 0)
        {
            _setCurrGrabber(_connected.begin()->second);
        }
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_setCurrGrabber
 * \param grabber
 *
 *=============================================================================
 */
void MainWindow::_setCurrGrabber(Grabber *grabber)
{
    _currGrabber = grabber;
    _refreshSliders();
    _refreshParameterWindow();
    _currGrabber->setProfile(_currGrabber->getCurrentProfileName());
    _refreshProfileMenu();
}
