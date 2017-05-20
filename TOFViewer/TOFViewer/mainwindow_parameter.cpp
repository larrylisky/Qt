/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TAB_SLIDER      0
#define TAB_PARAM       1


/*!
 *=============================================================================
 *
 * \brief MainWindow::_setupParameterWindow
 *
 *=============================================================================
 */
void MainWindow::_setupParameterWindow()
{
    connect(_ui->paramTabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(slotTabBarClicked(int)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_refreshParameterWindow
 *
 *=============================================================================
 */
void MainWindow::_refreshParameterWindow()
{
    if (_currGrabber)
    {      
        Ptr<RegisterProgrammer> programmer = _currGrabber->getProgrammer();
        Map<String, ParameterPtr> params = _currGrabber->getParameters();

        _paramModel = new QStandardItemModel(params.size(), 3, this);
        _paramModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
        _paramModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Value")));
        _paramModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Unit")));

        _ui->tableView->setModel(_paramModel);


        int row = 0;
        std::map< String, ParameterPtr> ordered(params.begin(), params.end()); // sort
        for (auto itParam = ordered.begin(); itParam != ordered.end(); itParam++)
        {
            QString name = QString(itParam->first.c_str());
            ParameterPtr p = itParam->second;
            p->refresh();

            BoolParameter *boolParam = dynamic_cast<BoolParameter *>(p.get());
            IntegerParameter *intParam = dynamic_cast<IntegerParameter *>(p.get());
            UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
            FloatParameter *floatParam = dynamic_cast<FloatParameter *>(p.get());
            EnumParameter *enumParam = dynamic_cast<EnumParameter *>(p.get());

            QModelIndex index = _paramModel->index(row, 0, QModelIndex());
            _paramModel->setData(index, QVariant(name));
            index = _paramModel->index(row, 1, QModelIndex());

            if (boolParam)
            {
                bool value;
                if (!boolParam->get(value))
                {
                    logger(LOG_ERROR) << "Failed to get parameter '"
                                      << name.toStdString() << "'" << std::endl;
                    return;
                }
                std::cout << name.toStdString() << " = " << (value?"true":"false");

                const Vector<String> &meaning = boolParam->valueMeaning();

                if(meaning.size() == 2 && meaning[value].size())
                    std::cout << " (" << meaning[value] << ")";

                std::cout << std::endl;

                index = _paramModel->index(row, 1, QModelIndex());
                _paramModel->setData(index, QVariant(value));
            }

            else if (intParam)
            {
                int value;
                if(!intParam->get(value))
                {
                    logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                    return;
                }
                std::cout << name.toStdString() << " = " << std::dec << value  << " " << intParam->unit() << std::endl;

                index = _paramModel->index(row, 1, QModelIndex());
                _paramModel->setData(index, QVariant(value));

                index = _paramModel->index(row, 2, QModelIndex());
                _paramModel->setData(index, QVariant(QString(intParam->unit().c_str())));


            }

            else if (uintParam)
            {
                uint value;
                if (!uintParam->get(value))
                {
                    logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                    return;
                }
                std::cout << name.toStdString() << " = " << std::dec << value << " " << uintParam->unit() << std::endl;

                index = _paramModel->index(row, 1, QModelIndex());
                _paramModel->setData(index, QVariant(value));

                index = _paramModel->index(row, 2, QModelIndex());
                _paramModel->setData(index, QVariant(QString(uintParam->unit().c_str())));
            }

            else if (floatParam)
            {
                float value;
                if(!floatParam->get(value))
                {
                    logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                    return;
                }
                std::cout << name.toStdString() << " = " << std::dec << value << " " << floatParam->unit() << std::endl;

                index = _paramModel->index(row, 1, QModelIndex());
                _paramModel->setData(index, QVariant(value));

                index = _paramModel->index(row, 2, QModelIndex());
                _paramModel->setData(index, QVariant(QString(floatParam->unit().c_str())));
            }

            else if (enumParam)
            {
                int value;
                if(!enumParam->get(value))
                {
                    logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                    return;
                }
                std::cout << name.toStdString() << " = "  << std::dec << value;

                const Vector<String> &meaning = enumParam->valueMeaning();

                if(meaning.size() > value && meaning[value].size())
                    std::cout << " (" << meaning[value] << ")";

                std::cout << std::endl;

                index = _paramModel->index(row, 1, QModelIndex());
                _paramModel->setData(index, QVariant(value));
            }

            row++;
        }
        _ui->tableView->update();
    }

}


void MainWindow::slotTabBarClicked(int tab)
{
    if (tab == TAB_SLIDER)
    {

    }
    else if (tab == TAB_PARAM)
    {
        _refreshParameterWindow();
    }
}
