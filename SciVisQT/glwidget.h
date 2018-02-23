#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <iostream>
#include <rfftw.h>
#include "simulation.h"
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    static bool isTransparent() { return m_transparent; }
    static void setTransparent(bool t) { m_transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void cleanup();
    void do_one_simulation_step();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    float max(float x, float y);
    float min(float x, float y);
    int clamp(float x);\
    void rainbow(float value,float* R,float* G,float* B);
    void heatMap(float value,float* R,float* G,float* B);
    float colorBands(float vy, int bands);
    void colorYellow(float value,float* R,float* G,float* B);
    void set_colormap(float vy);
    void direction_to_color(float x, float y, int method);
    void visualize();

private:
    void setupVertexAttribs();

    int   windowWidth, windowHeight;      //size of the graphics window, in pixels
    int   DIM;
    int   color_dir = 0;            //use direction color-coding or not
    float vec_scale = 1;			//scaling of hedgehogs
    int   draw_smoke = 0;           //draw the smoke or not
    int   draw_vecs = 1;            //draw the vector field or not

    const int COLOR_BLACKWHITE=0;   //different types of color mapping: black-and-white, rainbow, banded
    const int COLOR_RAINBOW=1;
    const int COLOR_HEAT=2;
    const int COLOR_YELLOW = 3;
    int   bands = 256;
    int   scalar_col = 0;           //method for scalar coloring
    int   frozen = 0;               //toggles on/off the animation

    Simulation simulation;

    QPoint m_lastPos;
    int m_lightPosLoc;
    static bool m_transparent;
};

#endif
