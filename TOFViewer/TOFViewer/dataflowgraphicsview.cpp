/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#include "dataflowgraphicsview.h"

/*!
 *=============================================================================
 *
 * \brief DataflowGraphicsView::DataflowGraphicsView
 *
 *=============================================================================
 */
DataflowGraphicsView::DataflowGraphicsView(QWidget * &widget)
{
    _parent = widget;
}


/*!
 *=============================================================================
 *
 * \brief DataflowGraphicsView::~DataflowGraphicsView
 *
 *=============================================================================
 */
DataflowGraphicsView::~DataflowGraphicsView()
{
}


/*!
 *=============================================================================
 *
 * \brief DataflowGraphicsView::resizeEvent
 * \param event
 *
 *=============================================================================
 */
void DataflowGraphicsView::resizeEvent(QResizeEvent *event)
{
    emit sizeChanged(event->size());
}



/*!
 *=============================================================================
 *
 * \brief DataflowGraphicsView::mousePressEvent
 * \param event
 *
 *=============================================================================
 */
void DataflowGraphicsView::mousePressEvent(QMouseEvent *event)
{
    std::cout << "Button " << event->button() << " pressed; Point("
              << event->x() << "," << event->y() << ")" << std::endl;
}


/*!
 *=============================================================================
 *
 * \brief DataflowGraphicsView::mouseReleaseEvent
 * \param event
 *
 *=============================================================================
 */
void DataflowGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    std::cout << "Button " << event->button() << " released; Point("
              << event->x() << "," << event->y() << ")" << std::endl;

}





