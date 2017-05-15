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
 * \brief MainWindow::_setupSliders()
 *
 *=============================================================================
 */
void MainWindow::_setupSliders()
{
    connect(_ui->unambDistSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateUnambDist(int)));
    connect(_ui->frameRateSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateFrameRate(int)));
    connect(_ui->integDutyCycleSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIntegDutyCycle(int)));
    connect(_ui->illumPwrSlider, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateIllumPwr(int)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotUpdateUnambDist
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotUpdateUnambDist(int value)
{
    _ui->unambDistEdit->setText(QString(std::to_string(value).c_str()));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotUpdateFrameRate
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotUpdateFrameRate(int value)
{
    _ui->frameRateEdit->setText(QString(std::to_string(value).c_str()));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotUpdateIntegDutyCycle
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotUpdateIntegDutyCycle(int value)
{
    _ui->integDutyCycleEdit->setText(QString(std::to_string(value).c_str()));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotUpdateIllumPwr
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotUpdateIllumPwr(int value)
{
    _ui->illumPwrEdit->setText(QString(std::to_string(value).c_str()));
}


