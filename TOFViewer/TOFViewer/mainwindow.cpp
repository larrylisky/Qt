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

    _ampGain = 1.0;
    _phaseGain = 1.0;

    _zLowerLimit = 0;
    _zUpperLimit = MAX_ZDEPTH;

    _currGrabber = NULL;

    _setupFileMenu();
    _setupProfileMenu();
    _setupMenuToolBar();
    _setupWindowsMenu();
    _setupSliders();
    _setupTimer();

    _setupDockWidgets();
    _setupParameterWindow();

    std::cout << "_ui is " << this << std::endl;
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


