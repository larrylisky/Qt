/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#include <iostream>
#include <stdlib.h>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QMouseEvent>
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

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    void _drawAxis();

private:
    XYZIPointCloudFrame _frame;
    bool _leftButtonPressed, _rightButtonPressed;
    int _mouseX, _mouseY;
    float _zoomFactor;
    QPoint _startPos, _endPos;
    float _rotX, _rotY;
    float _arrowSize, _axisLength;
};

#endif // TOFOPENGLWIDGET_H
