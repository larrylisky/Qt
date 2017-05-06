/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::slotTimeout()
{  
    _updateCameraMenu();
}


void MainWindow::slotVideoTimeout()
{
    _mtxTimer.lock();
    map< QString, Grabber* >::iterator itCamera;
    for (auto const&itCamera : _attachedCamera)
    {
        Grabber *grabber = itCamera.second;
        if (grabber != NULL)
        {           
            Grabber::FrameFlag flag = grabber->getFrameFlag();

#if 0
            if (flag & Grabber::FRAMEFLAG_XYZI_POINT_CLOUD_FRAME )
            {
                XYZIPointCloudFrame *xyziFrame = grabber->getXYZIFrame();
                std::cout << "xyziFrame = " << ((xyziFrame == NULL) ? "0":"1") << std::endl;
                if (xyziFrame != NULL)
                    delete xyziFrame;
            }
#endif

#if 0
            if (flag & Grabber::FRAMEFLAG_DEPTH_FRAME )
            {
                DepthFrame *depthFrame = grabber->getDepthFrame();
                std::cout << "depthFrame = " << ((depthFrame == NULL) ? "0":"1") << std::endl;
                if (depthFrame)
                    delete depthFrame;
            }
#endif

#if 1
            if (flag & Grabber::FRAMEFLAG_RAW_PROCESSED )
            {
                ToFRawFrameTemplate<uint16_t,uint8_t> *rawFrame = grabber->getRawFrameProcessed();
                std::cout << "rawFrameProcessed = " << ((rawFrame == NULL) ? "0":"1")
                          << "  Frame# " << grabber->getFrameCount() << std::endl;

                if (rawFrame)
                    delete rawFrame;
            }
#endif

#if 0

            if (flag & Grabber::FRAMEFLAG_RAW_UNPROCESSED )
            {
                ToFRawFrameTemplate<uint16_t,uint8_t> *rawFrame = grabber->getRawFrameUnprocessed();
                std::cout << "rawFrameUnprocessed = " << ((rawFrame == NULL) ? "0":"1") << std::endl;
                if (rawFrame)
                    delete rawFrame;
            }
#endif
        }
    }
    _mtxTimer.unlock();
}

