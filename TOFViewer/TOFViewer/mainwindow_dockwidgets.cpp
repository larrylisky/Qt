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
 * \brief MainWindow::_setupDockWidgets
 *
 *=============================================================================
 */
void MainWindow::_setupDockWidgets()
{
    connect(_ui->infoDockWidget, SIGNAL(visibilityChanged(bool)) , this, SLOT(slotInfoDockWidgetVisChanged(bool)));
    connect(_ui->imageDockWidget, SIGNAL(visibilityChanged(bool)) , this, SLOT(slotImageDockWidgetVisChanged(bool)));
    connect(_ui->settingsDockWidget, SIGNAL(visibilityChanged(bool)) , this, SLOT(slotSettingsDockWidgetVisChanged(bool)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotInfoDockWidgetVisChanged
 * \param checked
 *
 *=============================================================================
 */
void MainWindow::slotInfoDockWidgetVisChanged(bool checked)
{
    QList<QAction *> list = _ui->menuWindows->actions();
    for (int i=0; i < list.size(); i++)
    {
        QAction *action = list[i];
        if (action->text() == QString("Information"))
        {
            action->setChecked(checked);
            return;
        }
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotImageDockWidgetVisChanged
 * \param checked
 *
 *=============================================================================
 */
void MainWindow::slotImageDockWidgetVisChanged(bool checked)
{
    QList<QAction *> list = _ui->menuWindows->actions();
    for (int i=0; i < list.size(); i++)
    {
        QAction *action = list[i];
        if (action->text() == QString("Camera Images"))
        {
            action->setChecked(checked);
            return;
        }
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSettingsDockWidgetVisChanged
 * \param checked
 *
 *=============================================================================
 */
void MainWindow::slotSettingsDockWidgetVisChanged(bool checked)
{
    QList<QAction *> list = _ui->menuWindows->actions();
    for (int i=0; i < list.size(); i++)
    {
        QAction *action = list[i];
        if (action->text() == QString("Settings"))
        {
            action->setChecked(checked);
            return;
        }
    }
}




