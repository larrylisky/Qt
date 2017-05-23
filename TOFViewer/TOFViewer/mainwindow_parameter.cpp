/*
 * TI TOFViewer component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


#define TAB_SLIDER      0
#define TAB_PARAM       1

#define COLUMN_NAME     0
#define COLUMN_VALUE    1
#define COLUMN_UNIT     2

#define TYPE_BOOL       1
#define TYPE_INT        2
#define TYPE_UINT       3
#define TYPE_FLOAT      4
#define TYPE_ENUM       5


/*!
 *=============================================================================
 *
 * \brief MainWindow::_setupParameterWindow
 *
 *=============================================================================
 */
void MainWindow::_setupParameterWindow()
{
    connect(_ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotParamSearchChanged(QString)));
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
        Map<String, ParameterPtr> parm = _currGrabber->getParameters();
        std::map<String, ParameterPtr> params(parm.begin(), parm.end());  // sorted

        String prefix = _ui->lineEdit->text().toStdString();

        int rows = 0;
        for (auto itParam = params.begin(); itParam != params.end(); itParam++)
        {
            QString name = QString(itParam->first.c_str());
            if (prefix.size() == 0 || name.toStdString().compare(0, prefix.size(), prefix) == 0)
                rows++;
        }

        disconnect(_ui->tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(slotViewDataChanged(int, int)));
        _ui->tableWidget->clear();
        _ui->tableWidget->setRowCount(rows);
        _ui->tableWidget->setColumnCount(3);
        _ui->tableWidget->setHorizontalHeaderItem(COLUMN_NAME, new QTableWidgetItem(QString("Parameter")));
        _ui->tableWidget->setHorizontalHeaderItem(COLUMN_VALUE, new QTableWidgetItem(QString("Value")));
        _ui->tableWidget->setHorizontalHeaderItem(COLUMN_UNIT, new QTableWidgetItem(QString("Unit")));

        rows = 0;
        for (auto itParam = params.begin(); itParam != params.end(); itParam++)
        {
            QString name = QString(itParam->first.c_str());
            if (prefix.size() == 0 || name.toStdString().compare(0, prefix.size(), prefix) == 0)
            {
                ParameterPtr p = itParam->second;
                p->refresh();

                BoolParameter *boolParam = dynamic_cast<BoolParameter *>(p.get());
                IntegerParameter *intParam = dynamic_cast<IntegerParameter *>(p.get());
                UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
                FloatParameter *floatParam = dynamic_cast<FloatParameter *>(p.get());
                EnumParameter *enumParam = dynamic_cast<EnumParameter *>(p.get());

                QTableWidgetItem *item;

                if (boolParam)
                {
                    bool value;
                    if (!boolParam->get(value))
                    {
                        logger(LOG_ERROR) << "Failed to get parameter '"
                                          << name.toStdString() << "'" << std::endl;
                        return;
                    }
                    item = new QTableWidgetItem(name);
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_NAME, item);

                    item = new QTableWidgetItem(TYPE_BOOL);
                    item->setData(Qt::EditRole, QVariant(value));
                    _ui->tableWidget->setItem(rows, COLUMN_VALUE, item);

                    item = new QTableWidgetItem(QString(""));
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_UNIT, item);
                }
                else if (intParam)
                {
                    int value;
                    if(!intParam->get(value))
                    {
                        logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                        return;
                    }
                    item = new QTableWidgetItem(name);
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_NAME, item);

                    item = new QTableWidgetItem(TYPE_INT);
                    item->setData(Qt::EditRole, QVariant(value));
                    _ui->tableWidget->setItem(rows, COLUMN_VALUE, item);

                    item = new QTableWidgetItem(QString(intParam->unit().c_str()));
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_UNIT, item);

                }
                else if (uintParam)
                {
                    uint value;
                    if (!uintParam->get(value))
                    {
                        logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                        return;
                    }
                    item = new QTableWidgetItem(name);
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_NAME, item);

                    item = new QTableWidgetItem(TYPE_UINT);
                    item->setData(Qt::EditRole, QVariant(value));
                    _ui->tableWidget->setItem(rows, COLUMN_VALUE, item);

                    item = new QTableWidgetItem(QString(uintParam->unit().c_str()));
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_UNIT, item);

                }
                else if (floatParam)
                {
                    float value;
                    if(!floatParam->get(value))
                    {
                        logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                        return;
                    }
                    item = new QTableWidgetItem(name);
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_NAME, item);

                    item = new QTableWidgetItem(TYPE_FLOAT);
                    item->setData(Qt::EditRole, QVariant(value));
                    _ui->tableWidget->setItem(rows, COLUMN_VALUE, item);

                    item = new QTableWidgetItem(QString(floatParam->unit().c_str()));
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_UNIT, item);

                }
                else if (enumParam)
                {
                    int value;
                    if(!enumParam->get(value))
                    {
                        logger(LOG_ERROR) << "Failed to get parameter '" << name.toStdString() << "'" << std::endl;
                        return;
                    }
                    item = new QTableWidgetItem(name);
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_NAME, item);

                    item = new QTableWidgetItem(TYPE_ENUM);
                    item->setData(Qt::EditRole, QVariant(value));
                    _ui->tableWidget->setItem(rows, COLUMN_VALUE, item);

                    item = new QTableWidgetItem(QString("enum"));
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable); // non editable
                    _ui->tableWidget->setItem(rows, COLUMN_UNIT, item);
                }

                rows++;

            }  // if (compare)

        }  // for (it)

        _ui->tableWidget->update();
        connect(_ui->tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(slotViewDataChanged(int, int)));

    }  // if (_currGrabber)
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotTabBarClicked
 * \param tab
 *
 *=============================================================================
 */
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



/*!
 *=============================================================================
 *
 * \brief MainWindow::slotParamSearchChanged
 * \param text
 *
 *=============================================================================
 */
void MainWindow::slotParamSearchChanged(QString text)
{
    _refreshParameterWindow();
}



/*!
 *=============================================================================
 *
 * \brief MainWindow::slotViewDataChanged
 * \param index
 *
 *=============================================================================
 */
void MainWindow::slotViewDataChanged(int row, int col)
{
    if (_currGrabber)
    {
        Map<String, ParameterPtr> param = _currGrabber->getParameters();
        String name = _ui->tableWidget->item(row, COLUMN_NAME)->text().toStdString();
        ParameterPtr p = param[name];

        BoolParameter *boolParam = dynamic_cast<BoolParameter *>(p.get());
        IntegerParameter *intParam = dynamic_cast<IntegerParameter *>(p.get());
        UnsignedIntegerParameter *uintParam = dynamic_cast<UnsignedIntegerParameter *>(p.get());
        FloatParameter *floatParam = dynamic_cast<FloatParameter *>(p.get());
        EnumParameter *enumParam = dynamic_cast<EnumParameter *>(p.get());

        if (boolParam)
        {
            bool value = _ui->tableWidget->item(row, COLUMN_VALUE)->data(Qt::EditRole).toBool();
            boolParam->set(value);
        }
        else if (intParam)
        {
            int value = _ui->tableWidget->item(row, COLUMN_VALUE)->data(Qt::EditRole).toInt();
            intParam->set(value);
        }
        else if (uintParam)
        {
            uint value = _ui->tableWidget->item(row, COLUMN_VALUE)->data(Qt::EditRole).toUInt();
            uintParam->set(value);
        }
        else if (floatParam)
        {
            float value = _ui->tableWidget->item(row, COLUMN_VALUE)->data(Qt::EditRole).toFloat();
            floatParam->set(value);
        }
        else if (enumParam)
        {
            int value = _ui->tableWidget->item(row, COLUMN_VALUE)->data(Qt::EditRole).toInt();
            enumParam->set(value);
        }
        else
        {
            logger(LOG_ERROR) << "Invalid data type." << std::endl;
        }

        _refreshParameterWindow();

    }  // if (_currGrabber)
}
