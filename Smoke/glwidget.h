#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <rfftw.h>
#include "simulation.h"
#include <QVector3D>
#include <bitset>

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
     void toggleColorDirection(bool checked);
     void toggleColorScaling(bool checked);
     void toggleColorClamping(bool checked);
     void toggleDivergence(bool checked);


     void setColor(int c);
     void setColorBands(int b);
     void setVecScale(float vs);
     void setSaturation(float sat);
     void setHue(float h);
     void setColorScaleMax(int max);
     void setColorScaleMin(int min);
     void setColorClampMax(int max);
     void setColorClampMin(int min);
     void setScalarDataSet(int value);
     void setVectorDataSet(int value);
     void setGlyphsSampleAmountX(int value);
     void setGlyphsSampleAmountY(int value);
     void setGlyphShape(int value);
     void setIsolines(int value);
     void setIsolineThreshold(float pos);
     void setIsolineMin(float pos);
     void setIsolineMax(float pos);
     void setIsolineN(int pos);
     void setIsolineOption(int value);
     void setHeightPlot(bool value);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    float max(float x, float y);
    float min(float x, float y);
    void drawColorBar();
    float clamp(float value, float min, float max);
    void rainbow(float value,float* R,float* G,float* B);
    void heatMap(float value,float* R,float* G,float* B);
    void zebra(float value,float* R,float* G,float* B);
    float colorBands(float vy, int bands);
    void set_colormap(float vy);
    void direction_to_color(float x, float y, int method);
    void visualize();

    void rgb2hsv(float r, float g, float b, float* H, float* S, float* V);
    void hsv2rgb(float* R, float* G, float* B, float H, float S, float V);

    float get_scalar(int i);

    QVector3D interpolation(QVector3D pos_to_visualise, QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4);
    float divergence(int i);

    void draw_isoline(float threshold, fftw_real wn, fftw_real hn);

    void draw_hedgehog(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j);
    void draw_arrow(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j);
    void draw_cone(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j);


private:
    int   windowWidth, windowHeight;      //size of the graphics window, in pixels
    int   DIM;
    int   color_dir = 0;            //use direction color-coding or not
    float vec_scale = 0.2;			//scaling of hedgehogs
    int   draw_smoke = 1;           //draw the smoke or not
    int   draw_vecs = 1;            //draw the vector field or not
    int   isolines = 0;

    const int COLOR_BLACKWHITE=0;   //different types of color mapping: black-and-white, rainbow, banded
    const int COLOR_RAINBOW=1;
    const int COLOR_HEAT=2;
    const int COLOR_ZEBRA=3;
    int   bands = 256;
    float   saturation = 1.0;
    float   hue = 0.0;

    float color_scale_min = 0.0;
    float color_scale_max = 1.0;
    int color_scaling = 0;

    float color_clamp_min = 0.0;
    float color_clamp_max = 1.0;
    int color_clamping = 0;

    int   scalar_col = 0;           //method for scalar coloring
    int   frozen = 0;               //toggles on/off the animation

    int scalar_data_set = 0;        //setting of scalar data set: 0 == rho, 1 == velocity magnitude, 2 == force magnitude
    int vector_data_set = 0;        //setting of vector data set: 0 == velocity, 1 == force
    int glyph_sample_amt_x = 50;      //amount of samples of glyphs in x direction
    int glyph_sample_amt_y = 50;      //amount of samples of glyphs in y direction

    int diver = 0;

    int glyph_shape = 0;

    int isoline_option = 0;             //isoline option 0 == threshold, 1 == range
    float iso_threshold = 0.20;         //isoline threshold
    float iso_min = 0.0;                // minimum for isoline range
    float iso_max = 1.0;                // maximum for isoline range
    int iso_N = 1;                      // amount n isolines in the range

    bool height_plot = false;

    Simulation simulation;

    QPoint m_lastPos;
};

#endif // GLWIDGET_H
