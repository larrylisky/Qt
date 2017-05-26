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
    // Parameter slider edit field
    connect(_ui->unambDistEdit, SIGNAL(returnPressed()), this, SLOT(slotEditedUnambDist()));
    connect(_ui->frameRateEdit, SIGNAL(returnPressed()), this, SLOT(slotEditedFrameRate()));
    connect(_ui->integDutyCycleEdit, SIGNAL(returnPressed()), this, SLOT(slotEditedIntegDutyCycle()));
    connect(_ui->illumPwrEdit, SIGNAL(returnPressed()), this, SLOT(slotEditedIllumPwr()));

    // Parameter sliders
    connect(_ui->unambDistSlider, SIGNAL(valueChanged(int)), this, SLOT(slotSliderUnambDist(int)));
    connect(_ui->frameRateSlider, SIGNAL(valueChanged(int)), this, SLOT(slotSliderFrameRate(int)));
    connect(_ui->integDutyCycleSlider, SIGNAL(valueChanged(int)), this, SLOT(slotSliderIntegDutyCycle(int)));
    connect(_ui->illumPwrSlider, SIGNAL(valueChanged(int)), this, SLOT(slotSliderIllumPwr(int)));

    // Image gain sliders
    _ui->phaseRangeSlider->setMinimum(1);
    _ui->phaseRangeSlider->setMaximum(1000);
    connect(_ui->phaseRangeSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotPhaseGainSliderMoved(int)));

    _ui->ampRangeSlider->setMinimum(1);
    _ui->ampRangeSlider->setMaximum(4000);
    connect(_ui->ampRangeSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotAmpGainSliderMoved(int)));

    // zDepth limit sliders
    _ui->zLowerLimitSlider->setMinimum(0);
    _ui->zLowerLimitSlider->setMaximum(MAX_ZDEPTH);
    _ui->zLowerLimitSlider->setValue(0);
    _ui->zLowerLimitSlider->update();
    connect(_ui->zLowerLimitSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotLowerLimitSliderMoved(int)));

    _ui->zUpperLimitSlider->setMinimum(0);
    _ui->zUpperLimitSlider->setMaximum(MAX_ZDEPTH);
    _ui->zUpperLimitSlider->setValue(MAX_ZDEPTH);
    _ui->zUpperLimitSlider->update();
    connect(_ui->zUpperLimitSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotUpperLimitSliderMoved(int)));

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_refreshSliders
 *
 *=============================================================================
 */
void MainWindow::_refreshSliders()
{
    if (_currGrabber)
    {
        String name;
        ParameterPtr p;
        uint uvalue;
        UnsignedIntegerParameter *uintParam;
        Map<String, ParameterPtr> param = _currGrabber->getParameters();

        name = String("unambiguous_range");
        p = param[name];
        p->refresh();
        uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        if (uintParam->get(uvalue, true))
        {
            _ui->unambDistSlider->setMinimum(uintParam->lowerLimit());
            _ui->unambDistSlider->setMaximum(uintParam->upperLimit());
            _ui->unambDistSlider->setValue(uvalue);
            _ui->unambDistEdit->setText(QString(std::to_string(uvalue).c_str()));
        }

        name = String("illum_power_percentage");
        p = param[name];
        p->refresh();
        uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        if (uintParam->get(uvalue, true))
        {
            _ui->illumPwrSlider->setMinimum(uintParam->lowerLimit());
            _ui->illumPwrSlider->setMaximum(uintParam->upperLimit());
            _ui->illumPwrSlider->setValue(uvalue);
            _ui->illumPwrEdit->setText(QString(std::to_string(uvalue).c_str()));
        }

        name = String("intg_duty_cycle");
        p = param[name];
        p->refresh();
        uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        if (uintParam->get(uvalue, true))
        {
            _ui->integDutyCycleSlider->setMinimum(uintParam->lowerLimit());
            _ui->integDutyCycleSlider->setMaximum(uintParam->upperLimit());
            _ui->integDutyCycleSlider->setValue(uvalue);
            _ui->integDutyCycleEdit->setText(QString(std::to_string(uvalue).c_str()));
        }


        int value = (int)_currGrabber->getFramesPerSecond();
        _ui->frameRateEdit->setText(QString(std::to_string(value).c_str()));
        _ui->frameRateSlider->setValue(value);
        _ui->frameRateSlider->update();


    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSliderUnambDist
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotSliderUnambDist(int value)
{
    if (_currGrabber)
    {
        String name = String("unambiguous_range");
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        ParameterPtr p = param[name];
        p->refresh();

        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        uint v = (uint)value;

        if (uintParam->set(v))
        {
            if (uintParam->get(v, true))
            {
                _ui->unambDistEdit->setText(QString(std::to_string(v).c_str()));
                _ui->unambDistSlider->setValue((int)v);
                _ui->unambDistSlider->update();
            }
            else
            {
                logger(LOG_ERROR) << "Slider: cannot get " << name << std::endl;
            }
        }
        else
        {
            logger(LOG_ERROR) << "Slider: cannot set " << name << std::endl;
        }

    } // if (_currGrabber)
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSliderFrameRate
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotSliderFrameRate(int value)
{
    if (_currGrabber)
    {
        _currGrabber->setFrameRate((float)value);
        value = (int)_currGrabber->getFramesPerSecond();
        _ui->frameRateEdit->setText(QString(std::to_string(value).c_str()));
        _ui->frameRateSlider->setValue(value);
        _ui->frameRateSlider->update();
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSliderIntegDutyCycle
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotSliderIntegDutyCycle(int value)
{
    if (_currGrabber)
    {
        String name = String("intg_duty_cycle");
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        ParameterPtr p = param[name];
        p->refresh();

        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        uint v = (uint)value;

        if (uintParam->set(v))
        {
            if (uintParam->get(v, true))
            {
                _ui->integDutyCycleEdit->setText(QString(std::to_string(v).c_str()));
                _ui->integDutyCycleSlider->setValue((int)v);
                _ui->integDutyCycleSlider->update();
            }
            else
            {
                logger(LOG_ERROR) << "Slider: cannot get " << name << std::endl;
            }
        }
        else
        {
            logger(LOG_ERROR) << "Slider: cannot set " << name << std::endl;
        }

    } // if (_currGrabber)
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSliderIllumPwr
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotSliderIllumPwr(int value)
{
    if (_currGrabber)
    {
        String name = String("illum_power_percentage");
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        ParameterPtr p = param[name];
        p->refresh();

        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        uint v = (uint)value;

        if (uintParam->set(v))
        {
            if (uintParam->get(v, true))
            {
                _ui->illumPwrEdit->setText(QString(std::to_string(v).c_str()));
                _ui->illumPwrSlider->setValue((int)v);
                _ui->illumPwrSlider->update();
            }
            else
            {
                logger(LOG_ERROR) << "Slider: cannot get " << name << std::endl;
            }
        }
        else
        {
            logger(LOG_ERROR) << "Slider: cannot set " << name << std::endl;
        }

    } // if (_currGrabber)

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotEditedUnambDist
 *
 *=============================================================================
 */
void MainWindow::slotEditedUnambDist()
{
    bool ok = false;

    int value = _ui->unambDistEdit->text().toInt(&ok);
    if (ok)
    {
        slotSliderUnambDist(value);
    }
    else
    {
        String name = String("unambiguous_range");
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        ParameterPtr p = param[name];
        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());

        uint v;
        if (uintParam->get(v, true))
        {
            _ui->unambDistEdit->setText(QString(std::to_string(v).c_str()));
            _ui->unambDistSlider->setValue((int)v);
            _ui->unambDistSlider->update();
        }
    }

    std::cout << "unambiguous_range pressed (" << ok << ")" << std::endl;

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotEditedFrameRate
 *
 *=============================================================================
 */
void MainWindow::slotEditedFrameRate()
{
    bool ok;
    int value = _ui->frameRateEdit->text().toInt(&ok);

    if (_currGrabber && ok)
    {
        slotSliderFrameRate(value);
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotEditedIntegDutyCycle
 *
 *=============================================================================
 */
void MainWindow::slotEditedIntegDutyCycle()
{
    bool ok = false;

    int value = _ui->integDutyCycleEdit->text().toInt(&ok);
    if (ok)
    {
        slotSliderIntegDutyCycle(value);
    }
    else
    {
        String name = String("intg_duty_cycle");
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        ParameterPtr p = param[name];
        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());

        uint v;
        if (uintParam->get(v, true))
        {
            _ui->integDutyCycleEdit->setText(QString(std::to_string(v).c_str()));
            _ui->integDutyCycleSlider->setValue((int)v);
            _ui->integDutyCycleSlider->update();

        }
    }

    std::cout << "intg_duty_cycle pressed (" << ok << ")" << std::endl;

}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotEditedIllumPwr
 *
 *=============================================================================
 */
void MainWindow::slotEditedIllumPwr()
{
    bool ok = false;

    int value = _ui->illumPwrEdit->text().toInt(&ok);
    if (ok)
    {
        slotSliderIllumPwr(value);
    }
    else
    {
        // Display current frame rate
        String name = String("illum_power_percentage");
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        ParameterPtr p = param[name];
        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());

        uint v;
        if (uintParam->get(v, true))
        {
            _ui->illumPwrEdit->setText(QString(std::to_string(v).c_str()));
            _ui->illumPwrSlider->setValue((int)v);
            _ui->illumPwrSlider->update();
        }
    }

    std::cout << "IllumPwrEdit pressed (" << ok << ")" << std::endl;
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotPhaseGainSliderMoved
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotPhaseGainSliderMoved(int value)
{
    _phaseGain = value;
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotAmpGainSliderMoved
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotAmpGainSliderMoved(int value)
{
    _ampGain = (float)value/10.0;
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotLowerLimitSliderMoved
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotLowerLimitSliderMoved(int value)
{
    _zLowerLimit = value;
    if (_zLowerLimit > _zUpperLimit)
    {
        _zUpperLimit = _zLowerLimit;
        _ui->zUpperLimitSlider->setValue(_zUpperLimit);
        _ui->zUpperLimitSlider->update();
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotUpperLimitSliderMoved
 * \param value
 *
 *=============================================================================
 */
void MainWindow::slotUpperLimitSliderMoved(int value)
{
    _zUpperLimit = value;
    if (_zUpperLimit < _zLowerLimit)
    {
        _zLowerLimit = _zUpperLimit;
        _ui->zLowerLimitSlider->setValue(_zLowerLimit);
        _ui->zLowerLimitSlider->update();
    }
}
