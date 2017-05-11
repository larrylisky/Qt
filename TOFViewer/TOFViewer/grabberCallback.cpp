/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::_grabberCallback(Grabber *grabber)
{  
    std::cout << "Frame: " << grabber->getFrameCount() << std::endl;
}


