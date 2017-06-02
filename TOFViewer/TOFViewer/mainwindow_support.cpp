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
 * \brief MainWindow::_isIn
 * \param name
 * \param vec
 * \return
 *
 *=============================================================================
 */
bool MainWindow::_isIn(QString name, vector<QString> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        if (vec[i] == name)
            return true;

    return false;
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_scanCamera
 *
 *=============================================================================
 */
void MainWindow::_scanCamera()
{
    std::vector<QString> remain;
    _device = _sys.scan();

    // Find all attached but disconnected cameras
    remain.clear();
    _disconnected.clear();

    for (uint i = 0; i < _device.size(); i++)
    {
        QString name = QString(_device[i]->serialNumber().c_str());

        if (_connected.find(name) == _connected.end())
            _disconnected[name] = _device[i];
        else
            remain.push_back(name);
    }

    // Find all newly detached cameras and delete their Grabber instance
    map< QString, Grabber* >::iterator itCamera;
    for (auto const&itCamera : _connected)
    {
        QString name = itCamera.first;
        if (!_isIn(name, remain))
        {
            if (_connected[name])
                delete _connected[name];
            _connected.erase(name);
        }
    }

    // Reestablish _currGrabber in case _currGrabber was disconnected
    QString key;
    if (!_findKeyByValue< std::map< QString, Grabber* >, QString, Grabber* >(_connected, _currGrabber, key))
    {
        if (_connected.size() > 0)
        {
            _setCurrGrabber(_connected.begin()->second);
        }
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_setCurrGrabber
 * \param grabber
 *
 *=============================================================================
 */
void MainWindow::_setCurrGrabber(Grabber *grabber)
{
    _currGrabber = grabber;
    _refreshSliders();
    _refreshParameterWindow();
    _currGrabber->setProfile(_currGrabber->getCurrentProfileName());
    _refreshProfileMenu();
    _refreshDataFlowWindow();
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::RawFrameToMat
 * \param frame
 * \param rows
 * \param cols
 * \param phase
 * \param amplitude
 *
 *=============================================================================
 */
void MainWindow::RawFrameToMat(ToFRawFrame *frame, int rows, int cols,
                                 cv::Mat &phase, cv::Mat &amplitude)
{
    for (auto i=0; i<rows; i++ )
    {
        for (auto j=0; j<cols; j++ )
        {
            int idx = i * cols +j;
            if (frame->phaseWordWidth() == 2)
                phase.at<float>(i,j) = (float)(((uint16_t *)frame->phase())[idx])/4095.0;
            else if (frame->phaseWordWidth() == 1)
                phase.at<float>(i,j) = (float)(((uint8_t *)frame->phase())[idx])/255.0;

            if (frame->amplitudeWordWidth() == 2)
                amplitude.at<float>(i,j) = (float)(((uint16_t *)frame->amplitude())[idx])/4095.0;
            else if (frame->amplitudeWordWidth() == 1)
                amplitude.at<float>(i,j) = (float)(((uint8_t *)frame->amplitude())[idx])/255.0;
        }
    }
}



/*!
 *=============================================================================
 *
 * \brief MainWindow::DepthFrameToMat
 * \param frame
 * \param rows
 * \param cols
 * \param depth
 * \param amplitude
 * \return
 *
 *=============================================================================
 */
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



/*!
 *=============================================================================
 *
 * \brief MainWindow::MatToQImage
 * \param mat
 * \param image
 * \return
 *
 *=============================================================================
 */
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


/*!
 *=============================================================================
 *
 * \brief MainWindow::MatToQPixmap
 * \param mat
 * \param pixMap
 * \return
 *
 *=============================================================================
 */
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





