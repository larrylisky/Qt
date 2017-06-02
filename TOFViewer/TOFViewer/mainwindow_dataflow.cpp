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
 * \brief MainWindow::_setupInfoDockWidget
 *
 *=============================================================================
 */
void MainWindow::_setupInfoDockWidget()
{
    connect(_ui->infoTabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(slotInfoTabBarClicked(int)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotInfoTabBarClicked
 *
 *=============================================================================
 */
void MainWindow::slotInfoTabBarClicked(int tab)
{
    if (tab == INFO_DATAFLOW_TAB)
    {
        _refreshDataFlowWindow();
    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::setupDataFlow
 *
 *=============================================================================
 */
void MainWindow::_setupDataFlowWindow()
{
    connect(_ui->dataflowGraphicsView, SIGNAL(sizeChanged(QSize)),
            this, SLOT(slotSizeChangedUpdate(QSize)));
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_refreshDataFlowWindow
 *
 *=============================================================================
 */
void MainWindow::_refreshDataFlowWindow()
{
    if (_currGrabber)
    {
        const FilterSet<RawFrame> &unprocFilters = _currGrabber->getUnprocessedRawFilterSet();
        const FilterSet<RawFrame> &procFilters = _currGrabber->getProcessedRawFilterSet();
        const FilterSet<DepthFrame> &depthFilters = _currGrabber->getDepthFilterSet();

        QGraphicsTextItem *unprocText = _dataflowScene.addText(QString("Unprocessed"));
        QGraphicsTextItem *pclText = _dataflowScene.addText(QString("Point Cloud"));
        int minWidth = (int)max(pclText->boundingRect().width(), unprocText->boundingRect().width());
        int boxWidth = minWidth + 40;
        int boxHeight = boxWidth/2;
        int lineLength = boxWidth/4;
        int w = _ui->dataflowGraphicsView->geometry().width();
        int h = _ui->dataflowGraphicsView->geometry().height();
        int total = (4 + unprocFilters.size() + procFilters.size() + depthFilters.size()) * boxWidth
                    + 3* lineLength;

        _dataflowScene.clear();
        _dataflowScene.setSceneRect(0,0,w,h);
        QPoint p(max(0, (w-total)/2), h/2);
        p = _drawFilterChain<RawFrame>(_dataflowScene, unprocFilters, QString("Unprocessed"), p, boxWidth, boxHeight, lineLength);
        p = _drawArrow(_dataflowScene, p, lineLength);
        p = _drawFilterChain<RawFrame>(_dataflowScene, procFilters, QString("Processed"), p, boxWidth, boxHeight, lineLength);
        p = _drawArrow(_dataflowScene, p, lineLength);
        p = _drawFilterChain<DepthFrame>(_dataflowScene, depthFilters, QString("Depth"), p, boxWidth, boxHeight, lineLength);
        p = _drawArrow(_dataflowScene, p, lineLength);
        p = _drawBox(_dataflowScene, QString("Point Cloud"), p, boxWidth, boxHeight);

        _ui->dataflowGraphicsView->setScene(&_dataflowScene);
        _ui->dataflowGraphicsView->show();

    }
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_drawArrow
 * \param scene
 * \param org
 * \param length
 * \return
 *
 *=============================================================================
 */
QPoint MainWindow::_drawArrow(QGraphicsScene &scene, QPoint &org, int length)
{
    QPoint tip = QPoint(org.x()+length, org.y());

    scene.addLine(org.x(), org.y(), tip.x(), tip.y());
    scene.addLine(tip.x(), tip.y(), tip.x()-INFO_ARROW_SIZE, tip.y()-INFO_ARROW_SIZE);
    scene.addLine(tip.x(), tip.y(), tip.x()-INFO_ARROW_SIZE, tip.y()+INFO_ARROW_SIZE);
    scene.update();

    return tip;
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::_drawBox
 * \param scene
 * \param label
 * \param org
 * \param w
 * \param h
 * \return
 *
 *=============================================================================
 */
QPoint MainWindow::_drawBox(QGraphicsScene &scene, QString label, QPoint &org, int w, int h)
{
    QGraphicsSimpleTextItem *textItem = scene.addSimpleText(label);
    QGraphicsRectItem *rectItem = scene.addRect(org.x(), org.y()-h/2, w, h);
    int wLabel = (int)textItem->boundingRect().width();
    int hLabel = (int)textItem->boundingRect().height();
    textItem->setPos(org.x()+w/2-wLabel/2, org.y()-hLabel/2);
    scene.update();

    return QPoint(org.x()+w, org.y());
}


/*!
 *=============================================================================
 *
 * \brief MainWindow::slotSizeChangedUpdate
 * \param s
 *
 *=============================================================================
 */
void MainWindow::slotSizeChangedUpdate(QSize s)
{
    std::cout << "slotDataFlow" << std::endl;
    _refreshDataFlowWindow();
}


