#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <rfftw.h>
#include "simulation.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

public slots:
     void do_one_simulation_step();
     void toggle_smoke(bool checked);
     void toggle_vecs(bool checked);

     void setColor(int c);
     void setColorBands(int b);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    float max(float x, float y);
    float min(float x, float y);
    int clamp(float x);
    void rainbow(float value,float* R,float* G,float* B);
    void heatMap(float value,float* R,float* G,float* B);
    float colorBands(float vy, int bands);
    void colorYellow(float value,float* R,float* G,float* B);
    void set_colormap(float vy);
    void direction_to_color(float x, float y, int method);
    void visualize();

private:
    int   windowWidth, windowHeight;      //size of the graphics window, in pixels
    int   DIM;
    int   color_dir = 0;            //use direction color-coding or not
    float vec_scale = 1;			//scaling of hedgehogs
    int   draw_smoke = 1;           //draw the smoke or not
    int   draw_vecs = 1;            //draw the vector field or not

    const int COLOR_BLACKWHITE=0;   //different types of color mapping: black-and-white, rainbow, banded
    const int COLOR_RAINBOW=1;
    const int COLOR_HEAT=2;
    const int COLOR_YELLOW = 3;
    int   bands = 256;

    int   scalar_col = 1;           //method for scalar coloring
    int   frozen = 0;               //toggles on/off the animation

    Simulation simulation;

    QPoint m_lastPos;
};

#endif // GLWIDGET_H
