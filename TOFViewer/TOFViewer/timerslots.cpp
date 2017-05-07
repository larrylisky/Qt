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

            if (flag & Grabber::FRAMEFLAG_XYZI_POINT_CLOUD_FRAME )
            {
                XYZIPointCloudFrame *xyziFrame = grabber->getXYZIFrame();
                if (xyziFrame != NULL)
                    delete xyziFrame;
            }


            if (flag & Grabber::FRAMEFLAG_DEPTH_FRAME )
            {
                DepthFrame *depthFrame = grabber->getDepthFrame();
                if (depthFrame)
                    delete depthFrame;
            }


            // Raw processed data comes back at ToFRawFrame that should be cast to
            // ToFRawFrameTemplate<uint16_t, uint8_t>
            if (flag & Grabber::FRAMEFLAG_RAW_PROCESSED )
            {
                Ptr<Frame> f = grabber->getRawFrameProcessed();

                if (f && f.get())
                {
                    ToFRawFrame *rawFrame = dynamic_cast<ToFRawFrame *>(f.get());

                    if (rawFrame)
                    {
                        int rows = rawFrame->size.height;
                        int cols = rawFrame->size.width;

                        cv::Mat phaseMat = cv::Mat::zeros(rows,cols, CV_32FC1);
                        cv::Mat ampMat = cv::Mat::zeros(rows, cols, CV_32FC1);

                        RawFrameToMat(rawFrame, rows, cols, phaseMat, ampMat);

                        ampMat = ampMat * 10;

                        QPixmap phase_image, amp_image;
                        int w = _ui->phaseGraphicsView->geometry().width();
                        int h = _ui->phaseGraphicsView->geometry().height();

                        MatToQPixmap(phaseMat, phase_image);
                        QGraphicsScene* phase_scene = new QGraphicsScene();
                        phase_scene->addPixmap(phase_image.scaled(QSize(w,h), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        _ui->phaseGraphicsView->setScene(phase_scene);
                        _ui->phaseGraphicsView->show();

                        MatToQPixmap(ampMat, amp_image);
                        QGraphicsScene* amp_scene = new QGraphicsScene();
                        amp_scene->addPixmap(amp_image.scaled(QSize(w,h), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        _ui->amplitudeGraphicsView->setScene(amp_scene);
                        _ui->amplitudeGraphicsView->show();
                    }
                }
            }

            // Raw unprocessed data comes back at RawDataFrame
            if (flag & Grabber::FRAMEFLAG_RAW_UNPROCESSED )
            {
                Ptr<Frame> f = grabber->getRawFrameUnprocessed();

                if (f && f.get())
                {
                    RawDataFrame *rawFrame = dynamic_cast<RawDataFrame *>(f.get());

                    if (rawFrame)
                    {

                    }
                }
            }
        }
    }
    _mtxTimer.unlock();
}

