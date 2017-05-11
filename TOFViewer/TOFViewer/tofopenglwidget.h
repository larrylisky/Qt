/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#include <iostream>
#include <stdlib.h>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "mainwindow.h"


#ifndef TOFOPENGLWIDGET_H
#define TOFOPENGLWIDGET_H

class ToFOpenGLWidget : public QOpenGLWidget
{
public:
    ToFOpenGLWidget(QWidget *parent);
    ~ToFOpenGLWidget();

    void drawPointCloud(XYZIPointCloudFrame *frame);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    XYZIPointCloudFrame _frame;
};

#endif // TOFOPENGLWIDGET_H
