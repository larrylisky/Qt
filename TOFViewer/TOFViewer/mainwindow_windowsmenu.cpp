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
 * \brief MainWindow::_setupWindowsMenu
 *
 *=============================================================================
 */
void MainWindow::_setupWindowsMenu()
{
    connect(_ui->menuWindows, SIGNAL(triggered(QAction *)), this, SLOT(slotWindowsAction(QAction *)));
}



/*!
 *=============================================================================
 *
 * \brief MainWindow::slotWindowsAction
 * \param action
 *
 *=============================================================================
 */
void MainWindow::slotWindowsAction(QAction *action)
{
    QString name = action->text();

    if (name == "Camera Images")
    {
        if (action->isChecked())
            _ui->imageDockWidget->show();
        else
            _ui->imageDockWidget->hide();
    }
    else if (name == "Settings")
    {
        if (action->isChecked())
            _ui->settingsDockWidget->show();
        else
            _ui->settingsDockWidget->hide();
    }
    else if (name == "Information")
    {
        if (action->isChecked())
            _ui->infoDockWidget->show();
        else
            _ui->infoDockWidget->hide();
    }
}



