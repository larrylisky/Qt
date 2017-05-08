/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#include <iostream>
#include <stdlib.h>
#include <QOpenGLWidget>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>

#ifndef TOFOPENGLWIDGET_H
#define TOFOPENGLWIDGET_H

class ToFOpenGLWidget : public QOpenGLWidget
{
public:
    ToFOpenGLWidget(QWidget *parent);
    ~ToFOpenGLWidget();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
};

#endif // TOFOPENGLWIDGET_H
