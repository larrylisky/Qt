/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#include "tofopenglwidget.h"


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::ToFOpenGLWidget
 * \param parent
 *
 *=============================================================================
 */
ToFOpenGLWidget::ToFOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    _leftButtonPressed = _rightButtonPressed = false;
    _zoomFactor = 1.0;
    _rotX = _rotY = 0.0;
    _arrowSize = 0.25;
    _axisLength = 5.0;
    _gain = 1.0;
    _zMin = 0;
    _zMax = FLT_MAX;
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::~ToFOpenGLWidget
 *
 *=============================================================================
 */
ToFOpenGLWidget::~ToFOpenGLWidget()
{

}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::initializeGL
 *
 *=============================================================================
 */
void ToFOpenGLWidget::initializeGL()
{
    GLfloat light0_position[] = { 20.0, 20.0, -20.0, 0.0 };
    GLfloat light1_position[] = { -20.0, 20.0, -20.0, 0.0 };

    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::resizeGL
 * \param w
 * \param h
 *
 *=============================================================================
 */
void ToFOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,10,0,0,0,0,1,0);
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::paintGL
 *
 *=============================================================================
 */
void ToFOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_CLAMP);

    glPushMatrix();
    glRotatef(_rotX, 1,0,0);
    glRotatef(_rotY+180, 0,1,0);

    glPushMatrix();
    glTranslatef(0.0,0.0,_zoomFactor);

    glPushMatrix();
    _drawAxis();
    glPopMatrix();

    glBegin(GL_POINTS);
        glPointSize(2);
        for (int i=0; i < _frame.size(); i++)
        {
            float z = _frame.points[i].z;
            if (z >= _zMin && z <= _zMax)
            {
                float amp = _frame.points[i].i * _gain;
                float x = -_frame.points[i].x;
                float y = -_frame.points[i].y;
                glColor3f(amp, amp, amp);
                glVertex3f(x, y, z);
            }
        }
    glEnd();

    glFlush();
    glPopMatrix();
    glPopMatrix();
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::drawPointCloud
 * \param frame
 *
 *=============================================================================
 */
void ToFOpenGLWidget::drawPointCloud(XYZIPointCloudFrame *frame,
                                     float gain, float zMin, float zMax)
{
    if (frame)
    {
        _frame = *frame;
        _gain = gain;
        _zMin = zMin;
        _zMax = zMax;
    }

    paintGL();
    update();
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::mousePressEvent
 * \param event
 *
 *=============================================================================
 */
void ToFOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == 1)
    {
        _leftButtonPressed = true;
        _startPos = event->pos();
    }
    else if (event->button() == 2)
    {
        _rightButtonPressed = true;
        _startPos = event->pos();
    }

}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::mouseReleaseEvent
 * \param event
 *
 *=============================================================================
 */
void ToFOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == 1)
    {
        _leftButtonPressed = false;
        _endPos = event->pos();
    }
    else if (event->button() == 2)
    {
        _rightButtonPressed = false;
        _endPos = event->pos();
    }
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::mouseMoveEvent
 * \param event
 *
 *=============================================================================
 */
void ToFOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_leftButtonPressed)
    {
        float speed = 0.5;
        _rotY = speed * (event->pos().x() - _startPos.x());
        _rotX = speed * (event->pos().y() - _startPos.y());
    }
}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::wheelEvent
 * \param event
 *
 *=============================================================================
 */
void ToFOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    _zoomFactor -= (float)event->delta()/1200.0;

}


/*!
 *=============================================================================
 *
 * \brief ToFOpenGLWidget::_drawAxis
 *
 *=============================================================================
 */
void ToFOpenGLWidget::_drawAxis()
{
    // x axis- red
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
        glVertex3f(-_axisLength, 0.0f, 0.0f);
        glVertex3f(_axisLength, 0.0f, 0.0f);

        // arrow
        glVertex3f(_axisLength, 0.0f, 0.0f);
        glVertex3f(_axisLength-_arrowSize, _arrowSize, 0.0f);

        glVertex3f(_axisLength, 0.0f, 0.0f);
        glVertex3f(_axisLength-_arrowSize, -_arrowSize, 0.0f);
    glEnd();
    glFlush();


    // y axis - green
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, -_axisLength, 0.0f);
        glVertex3f(0.0, _axisLength, 0.0f);

        // arrow
        glVertex3f(0.0, _axisLength, 0.0f);
        glVertex3f(_arrowSize, _axisLength-_arrowSize, 0.0f);

        glVertex3f(0.0, _axisLength, 0.0f);
        glVertex3f(-_arrowSize, _axisLength-_arrowSize, 0.0f);
    glEnd();
    glFlush();


    // z axis - blue
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0f ,-_axisLength );
        glVertex3f(0.0, 0.0f ,_axisLength);

        // arrow
        glVertex3f(0.0, 0.0f ,_axisLength );
        glVertex3f(0.0, _arrowSize ,_axisLength-_arrowSize );

        glVertex3f(0.0, 0.0f ,_axisLength );
        glVertex3f(0.0, -_arrowSize ,_axisLength-_arrowSize );
    glEnd();
    glFlush();
}
