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
 * \brief MainWindow::_setupTimer
 * \param msec
 *
 *=============================================================================
 */
void MainWindow::_setupTimer()
{

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    _timer->start(1000);

    _videoTimer = new QTimer(this);
    connect(_videoTimer, SIGNAL(timeout()), this, SLOT(slotVideoTimeout()));

}



/*!
 *=============================================================================
 *
 * \brief MainWindow::slotTimeout
 *
 *=============================================================================
 */
void MainWindow::slotTimeout()
{  
    _refreshCameraMenu();
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotVideoTimeout
 *
 *=============================================================================
 */
void MainWindow::slotVideoTimeout()
{
    _mtxTimer.lock();

    Grabber *grabber = _currGrabber;
    if (grabber != NULL)
    {
        Grabber::FrameFlag flag = grabber->getFrameFlag();

        if (flag & Grabber::FRAMEFLAG_XYZI_POINT_CLOUD_FRAME )
        {
            XYZIPointCloudFrame *xyziFrame = grabber->getXYZIFrame();
            if (xyziFrame != NULL)
            {
                _ui->pclOpenGLWidget->drawPointCloud(xyziFrame,
                            _ampGain, (float)_zLowerLimit/1000.0, (float)_zUpperLimit/1000.0);
                delete xyziFrame;
            }
        }


        if (flag & Grabber::FRAMEFLAG_DEPTH_FRAME )
        {
            DepthFrame *depthFrame = grabber->getDepthFrame();
            if (depthFrame)
            {
                delete depthFrame;
            }
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
                    QPixmap phaseImage, ampImage;

                    int w = _ui->phaseGraphicsView->geometry().width();
                    int h = _ui->phaseGraphicsView->geometry().height();

                    RawFrameToMat(rawFrame, rows, cols, phaseMat, ampMat);

                    cv::Mat colorMat, grayMat;
                    phaseMat.convertTo(grayMat, CV_8UC1, 255.0);
                    grayMat = grayMat * _phaseGain / 200;
                    cv::applyColorMap(grayMat, colorMat, cv::COLORMAP_JET);
                    MatToQPixmap(colorMat, phaseImage);
                    _phaseScene.clear();
                    QGraphicsPixmapItem *item = _phaseScene.addPixmap(phaseImage);
                    _ui->phaseGraphicsView->fitInView(item, Qt::KeepAspectRatio);
                    _ui->phaseGraphicsView->setScene(&_phaseScene);

                    _ui->phaseGraphicsView->show();

                    ampMat = ampMat * _ampGain;
                    MatToQPixmap(ampMat, ampImage);
                    _ampScene.clear();
                    item = _ampScene.addPixmap(ampImage);
                    _ui->amplitudeGraphicsView->fitInView(item, Qt::KeepAspectRatio);
                    _ui->amplitudeGraphicsView->setScene(&_ampScene);
                    _ui->amplitudeGraphicsView->show();
                }
            }
        }

        // Raw unprocessed data comes back as RawDataFrame
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

    _mtxTimer.unlock();
}

