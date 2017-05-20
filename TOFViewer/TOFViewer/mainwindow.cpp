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
 * \brief MainWindow::MainWindow
 * \param parent
 *
 *=============================================================================
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _currGrabber = NULL;

    _setupFileMenu();
    _setupProfileMenu();
    _setupMenuToolBar();
    _setupWindowsMenu();
    _setupSliders();
    _setupTimer();

    _setupDockWidgets();
    _setupParameterWindow();

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::~MainWindow
 *
 *=============================================================================
 */
MainWindow::~MainWindow()
{
    delete _timer;
    delete _videoTimer;
    delete _ui;
}


