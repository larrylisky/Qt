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
 * \brief MainWindow::_setupMenuToolBar
 *
 *=============================================================================
 */
void MainWindow::_setupMenuToolBar()
{
    _ui->mainToolBar->setStyleSheet("QToolButton:checked{background-color : gray;}");
    connect(_ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(slotMainToolBar(QAction *)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotMainToolBar
 * \param action
 *
 *=============================================================================
 */
void MainWindow::slotMainToolBar(QAction *action)
{
    if (action->text() == "Run")
    {
        map< QString, Grabber* >::iterator itCamera;
        for (auto const&itCamera : _connected)
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
        map< QString, Grabber* >::iterator itCamera;
        for (auto const&itCamera : _connected)
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
        _currGrabber = _connected[action->text()];
        _refreshParameterWindow();
    }
}



