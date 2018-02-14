#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "logo.h"

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
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void cleanup();

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
    void visualize();
    void direction_to_color(float x, float y, int method);
    void set_colormap(float vy);
    void rainbow(float value,float* R,float* G,float* B);
    float max(float x, float y);

private:
    void setupVertexAttribs();

    int   winWidth, winHeight;      //size of the graphics window, in pixels
    int   color_dir = 0;            //use direction color-coding or not
    float vec_scale = 1000;			//scaling of hedgehogs
    int   draw_smoke = 0;           //draw the smoke or not
    int   draw_vecs = 1;            //draw the vector field or not
    const int COLOR_BLACKWHITE=0;   //different types of color mapping: black-and-white, rainbow, banded
    const int COLOR_RAINBOW=1;
    const int COLOR_BANDS=2;
    int   scalar_col = 0;           //method for scalar coloring
    int   frozen = 0;               //toggles on/off the animation

    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    Logo m_logo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    static bool m_transparent;
};

#endif
