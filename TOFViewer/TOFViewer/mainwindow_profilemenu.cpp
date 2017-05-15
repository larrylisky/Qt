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
 * \brief MainWindow::_refreshProfileMenu
 *
 *=============================================================================
 */
void MainWindow::_refreshProfileMenu()
{
    QMenu *submenu;
    QAction *action;

    if (_currGrabber)
    {
        connect(_ui->menuProfile, SIGNAL(aboutToShow()), this, SLOT(slotRefreshProfileMenu()));

#if 0
        _mtxTimer.lock();

        // Remove old list of connected cameras
        _ui->menuProfile->clear();
        _profileSubmenu.clear();


        // Create list of connected cameras
        map< QString, Grabber* >::iterator itCamera;
        for (auto const&itCamera : _attachedCamera)
        {
            QString name = itCamera.first;
            Grabber * grabber = itCamera.second;

            if (grabber != NULL)
            {
                submenu = _ui->menuProfiles->addMenu(name);
                _profileSubmenu.push_back(submenu);
                connect(submenu, SIGNAL(triggered(QAction*)), this, SLOT(slotSetProfiles(QAction*)));

                const Map<int, Voxel::String> &profiles = grabber->getProfiles();
                for (auto &p: profiles)
                {
                    QString profile_name = QString(p.second.c_str());
                    ConfigurationFile *c = grabber->getDepthCamera()->configFile.getCameraProfile(p.first);
                    if (c && c->getLocation() == ConfigurationFile::IN_CAMERA)
                        profile_name = profile_name + " (HW)";
                    action = submenu->addAction(profile_name);
                }
            }
        }

        _mtxTimer.unlock();
#endif

    } // if (_currGrabber)
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotRefreshProfilesMenu
 *
 *=============================================================================
 */
void MainWindow::slotRefreshProfileMenu()
{
    _refreshProfileMenu();
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSetProfiles
 * \param action
 *
 *=============================================================================
 */
void MainWindow::slotSetProfiles(QAction *action)
{
    std::cout << "slotSetProfiles()->" << action->text().toStdString() << std::endl;
}
