#include "glwidget.h"
#include <QMouseEvent>
#include <math.h>
#include <QTimer>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    DIM = 50;
    simulation.init_simulation(DIM);
    QTimer *timer = new QTimer;
    timer->start();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(do_one_simulation_step()));
}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
    //glClearColor(0, 0, 0, 1);
}

float GLWidget::max(float x, float y)
{ return x < y ? y : x; }

float GLWidget::min(float x, float y)
{ return x > y ? y : x; }

int GLWidget::clamp(float x)
{ return ((x)>=0.0?((int)(x)):(-((int)(1-(x))))); }

//rainbow: Implements a color palette, mapping the scalar 'value' to a rainbow color RGB
void GLWidget::rainbow(float value,float* R,float* G,float* B)
{
    const float dx=0.8;
    if (value<0)
        value=0;
    if (value>1)
        value=1;
    value = (6-2*dx)*value+dx;
    *R = max(0.0,(3-fabs(value-4)-fabs(value-5))/2);
    *G = max(0.0,(4-fabs(value-2)-fabs(value-4))/2);
    *B = max(0.0,(3-fabs(value-1)-fabs(value-2))/2);
}

//Implementation of our own colormap
void GLWidget::heatMap(float value,float* R,float* G,float* B)
{
   if (value<0) value=0; if (value>1) value=1;
   *R = clamp((4*value)-0.25);
   *G = clamp((4*value)-0.5);
   *B = clamp((4*value)-0.75);
}

void GLWidget::colorYellow(float value,float* R,float* G,float* B)
{
   if (value<0) value=0; if (value>1) value=1;
   *R = clamp((3*value)-0.33);
   *G = clamp((3*value)-0.33);
   *B = clamp((3*value)-0.67);
}


float GLWidget::colorBands(float vy, int bands){
    int NLEVELS = bands;
    vy *= NLEVELS;
    vy = (int)(vy);
    vy/= NLEVELS;
    return vy;
}


//set_colormap: Sets three different types of colormaps
void GLWidget::set_colormap(float vy)
{
    vy = colorBands(vy, bands);
    float R,G,B;
    if (scalar_col==COLOR_BLACKWHITE){
        R = G = B = vy;
    }
    else if (scalar_col==COLOR_RAINBOW)
        rainbow(vy,&R,&G,&B);
    else if (scalar_col==COLOR_HEAT)
        heatMap(vy,&R,&G,&B);
    else if (scalar_col==COLOR_YELLOW)
        colorYellow(vy,&R,&G,&B);
    glColor3f(R,G,B);
}

//direction_to_color: Set the current color by mapping a direction vector (x,y), using
//                    the color mapping method 'method'. If method==1, map the vector direction
//                    using a rainbow colormap. If method==0, simply use the white color
void GLWidget::direction_to_color(float x, float y, int method)
{
    float r,g,b,f;
    if (method)
    {
        f = atan2(y,x) / 3.1415927 + 1;
        r = f;
        if(r > 1)
            r = 2 - r;
        g = f + .66667;
        if(g > 2)
            g -= 2;
        if(g > 1)
            g = 2 - g;
        b = f + 2 * .66667;
        if(b > 2)
            b -= 2;
        if(b > 1)
            b = 2 - b;
    }
    else
    {
        r = g = b = 1;
    }
    glColor3f(r,g,b);
}

//visualize: This is the main visualization function
void GLWidget::visualize(void)
{
    int        i, j, idx;
    fftw_real  wn = (fftw_real)2 / (fftw_real)(DIM + 1);   // Grid cell width
    fftw_real  hn = (fftw_real)2 / (fftw_real)(DIM + 1);  // Grid cell heigh
    if (draw_smoke)
    {
        int idx0, idx1, idx2, idx3;
        double px0, py0, px1, py1, px2, py2, px3, py3;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLES);
        for (j = 0; j < DIM - 1; j++)            //draw smoke
        {
            for (i = 0; i < DIM - 1; i++)
            {
                px0 = wn + (fftw_real)i * wn;
                py0 = hn + (fftw_real)j * hn;
                idx0 = (j * DIM) + i;


                px1 = wn + (fftw_real)i * wn;
                py1 = hn + (fftw_real)(j + 1) * hn;
                idx1 = ((j + 1) * DIM) + i;


                px2 = wn + (fftw_real)(i + 1) * wn;
                py2 = hn + (fftw_real)(j + 1) * hn;
                idx2 = ((j + 1) * DIM) + (i + 1);


                px3 = wn + (fftw_real)(i + 1) * wn;
                py3 = hn + (fftw_real)j * hn;
                idx3 = (j * DIM) + (i + 1);

                set_colormap(simulation.get_rho()[idx0]);    glVertex2f(px0-1, py0-1);
                set_colormap(simulation.get_rho()[idx1]);    glVertex2f(px1-1, py1-1);
                set_colormap(simulation.get_rho()[idx2]);    glVertex2f(px2-1, py2-1);


                set_colormap(simulation.get_rho()[idx0]);    glVertex2f(px0-1, py0-1);
                set_colormap(simulation.get_rho()[idx2]);    glVertex2f(px2-1, py2-1);
                set_colormap(simulation.get_rho()[idx3]);    glVertex2f(px3-1, py3-1);
            }
        }
        glEnd();
    }

    if (draw_vecs)
    {
        glBegin(GL_LINES);
        for (i = 0; i < DIM; i++)
            for (j = 0; j < DIM; j++)
            {
                idx = (j * DIM) + i;
                direction_to_color(simulation.get_vx()[idx],simulation.get_vy()[idx], color_dir);
                glVertex2f((wn + (fftw_real)i * wn) - 1, (hn + (fftw_real)j * hn) - 1);
                glVertex2f(((wn + (fftw_real)i * wn) + vec_scale * simulation.get_vx()[idx]) - 1, ((hn + (fftw_real)j * hn) + vec_scale * simulation.get_vy()[idx]) -1);
            }
        glEnd();
    }
}

void GLWidget::do_one_simulation_step()
{
    if(!frozen){
        simulation.set_forces(DIM);
        simulation.solve(DIM, simulation.get_vx(), simulation.get_vy(), simulation.get_vx0(),
                         simulation.get_vy0(), simulation.get_visc(), simulation.get_dt());
        simulation.diffuse_matter(DIM, simulation.get_vx(), simulation.get_vy(),
                                  simulation.get_rho(), simulation.get_rho0(), simulation.get_dt());
        update();
    }
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    visualize();
    glFlush();
}

void GLWidget::resizeGL(int w, int h)
{
    windowHeight = h;
    windowWidth = w;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    float mx = event->localPos().x();
    float my = event->localPos().y();
    simulation.drag(mx,my, DIM, width(), height());

}

void GLWidget::toggle_smoke(bool checked){
    draw_smoke = checked;
}

void GLWidget::toggle_vecs(bool checked){
    draw_vecs = checked;
}

void GLWidget::toggleColorDirection(bool checked){
    color_dir = checked;
}

void GLWidget::setColor(int c){
    scalar_col = c;
}

void GLWidget::setColorBands(int b){
    bands = b;
    printf("%d color bands \n", bands);
}

void GLWidget::setVecScale(int vs){
    vec_scale = vs;
}
