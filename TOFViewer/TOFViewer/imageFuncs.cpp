/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::RawFrameToMat(ToFRawFrame *frame, int rows, int cols,
                                 cv::Mat &phase, cv::Mat &amplitude)
{
    for (auto i=0; i<rows; i++ )
    {
        for (auto j=0; j<cols; j++ )
        {
            int idx = i * cols +j;
            if (frame->phaseWordWidth() == 2)
                phase.at<float>(i,j) = (float)(((uint16_t *)frame->phase())[idx])/4096.0;
            else if (frame->phaseWordWidth() == 1)
                phase.at<float>(i,j) = (float)(((uint8_t *)frame->phase())[idx])/256.0;

            if (frame->amplitudeWordWidth() == 2)
                amplitude.at<float>(i,j) = (float)(((uint16_t *)frame->amplitude())[idx])/4096.0;
            else if (frame->amplitudeWordWidth() == 1)
                amplitude.at<float>(i,j) = (float)(((uint8_t *)frame->amplitude())[idx])/256.0;
        }
    }
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
    case CV_32FC1:
    {
        cv::Mat img = mat;

        QImage im(img.cols, img.rows, QImage::Format_RGB32);

        for (int i = 0; i < img.cols; i++)
        {
            for (int j = 0; j < img.rows; j++)
            {
                int gray = (int)(img.at<float>(j, i)*255.0);
                QRgb pixel = qRgb(gray, gray, gray);
                im.setPixel(i, j, pixel);
            }
        }
        image = im;
        ret = true;
        break;
    }
    case CV_8UC4:
    {
        image = QImage( mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_ARGB32 );
        ret = true;
        break;
    }
    case CV_8UC3:
    {
        image = QImage( mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_RGB888 );
        image.rgbSwapped();
        ret = true;
        break;
    }
    case CV_8UC1:
    {
        image = QImage( mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_Grayscale8 );
        ret = true;
        break;
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



