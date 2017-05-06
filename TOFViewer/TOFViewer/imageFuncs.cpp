/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


bool MainWindow::RawFrameToMat(ToFRawFrame *frame, int rows, int cols,
                                 cv::Mat &phase, cv::Mat &amplitude)
{
    bool ret = false;

    for (auto i=0; i<rows; i++ )
    {
        for (auto j=0; j<cols; j++ )
        {
            int idx = i * cols +j;
            if (frame->phaseWordWidth() == 4)
                phase.at<float>(i,j) = (float)(((uint32_t *)frame->phase())[idx]);
            else if (frame->phaseWordWidth() == 2)
                phase.at<float>(i,j) = (float)(((uint16_t *)frame->phase())[idx]);
            else if (frame->phaseWordWidth() == 1)
                phase.at<float>(i,j) = (float)(((uint8_t *)frame->phase())[idx]);

            if (frame->amplitudeWordWidth() == 4)
                amplitude.at<float>(i,j) = (float)(((uint32_t *)frame->amplitude())[idx]);
            else if (frame->amplitudeWordWidth() == 2)
                amplitude.at<float>(i,j) = (float)(((uint16_t *)frame->amplitude())[idx]);
            else if (frame->amplitudeWordWidth() == 1)
                amplitude.at<float>(i,j) = (float)(((uint8_t *)frame->amplitude())[idx]);
        }
    }
    return ret;
}


bool MainWindow::DepthFrameToMat(DepthFrame *frame, int rows, int cols,
                                 cv::Mat &depth, cv::Mat &amplitude)
{
    bool ret = false;

    for (auto i=0; i<rows; i++ )
    {
        for (auto j=0; j<cols; j++ )
        {
            int idx = i * cols +j;
            depth.at<float>(i,j) = frame->depth[idx];
            amplitude.at<float>(i,j) = frame->amplitude[idx];
        }
    }
    return ret;
}


bool MainWindow::MatToQImage(cv::Mat &mat, QImage &image)
{
    bool ret = false;

    switch (mat.type())
    {
    case CV_8UC4:
    {
        image = QImage( mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_ARGB32 );
    }
    case CV_8UC3:
    {
        image = QImage( mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_RGB888 );
        image.rgbSwapped();
    }
    case CV_8UC1:
    {
        image = QImage( mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_Grayscale8 );
    }
    default:
        break;
    }

    return ret;
}



bool MainWindow::MatToQPixmap(cv::Mat &mat, QPixmap &pixMap )
{
    bool ret = false;

    QImage qImage;

    if (MatToQImage(mat, qImage))
    {
        pixMap = QPixmap::fromImage(qImage);
        ret = true;
    }

    return ret;
}
