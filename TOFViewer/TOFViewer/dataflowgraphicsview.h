/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2017 Texas Instruments Inc.
 */

#ifndef DATAFLOWGRAPHICSVIEW_H
#define DATAFLOWGRAPHICSVIEW_H

#include <iostream>
#include <stdlib.h>
#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsView>
#include "mainwindow.h"

class DataflowGraphicsView : public QGraphicsView
{   
    Q_OBJECT

public:
    DataflowGraphicsView(QWidget * &widget);
    ~DataflowGraphicsView();

signals:
    void sizeChanged(QSize s);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QWidget *_parent;
};

#endif // DATAFLOWGRAPHICSVIEW_H
