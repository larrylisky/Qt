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
 * \brief MainWindow::_setupProfileMenu
 *
 *=============================================================================
 */
void MainWindow::_setupProfileMenu()
{
    connect(_ui->menuProfile, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowProfileMenu()));
    connect(_ui->menuProfile, SIGNAL(triggered(QAction*)), this, SLOT(slotSetProfile(QAction *)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_refreshProfileMenu
 *
 *=============================================================================
 */
void MainWindow::_refreshProfileMenu()
{
    _ui->menuProfile->clear();

    if (_currGrabber)
    {
        _mtxTimer.lock();

        const Map<int, Voxel::String> &profiles = _currGrabber->getProfiles();
        for (auto &p: profiles)
        {
            QString profileName = QString(p.second.c_str());
            ConfigurationFile *c = _currGrabber->getDepthCamera()->configFile.getCameraProfile(p.first);
            if (c && c->getLocation() == ConfigurationFile::IN_CAMERA)
            {
                QAction *action = _ui->menuProfile->addAction(profileName + "(HW)");
                action->setCheckable(true);
                if (profileName == QString(_currGrabber->getCurrentProfileName().c_str()))
                    action->setChecked(true);
                else
                    action->setChecked(false);
            }
        }

        _mtxTimer.unlock();

    } // if (_currGrabber)

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotRefreshProfilesMenu
 *
 *=============================================================================
 */
void MainWindow::slotAboutToShowProfileMenu()
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
void MainWindow::slotSetProfile(QAction *action)
{
    QString name = action->text();
    QStringList field = name.split("(");
    Voxel::String profileName = field[0].toStdString();

    if (_currGrabber)
    {
        bool rc = _currGrabber->setProfile(profileName);
        action->setChecked(true);
    }
}
