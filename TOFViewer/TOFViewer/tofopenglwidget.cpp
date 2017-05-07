/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#include "tofopenglwidget.h"


ToFOpenGLWidget::ToFOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}


void ToFOpenGLWidget::initializeGL()
{
    std::cout << "initialized" << std::endl;

}

void ToFOpenGLWidget::resizeGL(int w, int h)
{
    std::cout << "resized" << std::endl;

}

void ToFOpenGLWidget::paintGL()
{
    std::cout << "Painted" << std::endl;
}
