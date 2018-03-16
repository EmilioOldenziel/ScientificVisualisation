#include "glwidget.h"
#include <QMouseEvent>
#include <math.h>
#include <QTimer>
#include <QColor>

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
}

float GLWidget::max(float x, float y)
{ return x < y ? y : x; }

float GLWidget::min(float x, float y)
{ return x > y ? y : x; }

float GLWidget::clamp(float value, float min, float max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}

//draw colorbar besides the simulation
void GLWidget::drawColorBar()
{
    glPushMatrix();
    glBegin(GL_QUADS);

    float offset = (0.85);
    float bar_width = (0.1);
    float bar_height = (0.0075);

    for(int i = -100; i<= 100; i++){
        set_colormap((i+100)*0.5*0.01);
        glVertex3f(offset,              i*bar_height,       0.0);
        glVertex3f(offset+bar_width,    i*bar_height,       0.0);
        glVertex3f(offset,              (i+1)*bar_height,   0.0);
        glVertex3f(offset+bar_width,    (i+1)*bar_height,   0.0);
    }
    glEnd();
    glPopMatrix();
}



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

//Implementation of the heatmap colormap (page 155)
void GLWidget::heatMap(float value,float* R,float* G,float* B)
{
   if (value<0) value=0; if (value>1) value=1;
   *R = max(0.0f, -((value-0.9)*(value-0.9))+1);
   *G = max(0.0f, -((value-1.5)*(value-1.5))+1);
   *B = 0;
}

//Implementation of the zebra colormap (page 156)
void GLWidget::zebra(float value,float* R,float* G,float* B)
{
    if (value<0) value=0; if (value>1) value=1;
    if (int ((value*100)) % 25){
        *R = *G = *B = 1;

    }else{
        *R = *G = *B = 0;

    }
}

float GLWidget::colorBands(float vy, int bands){
    int NLEVELS = bands;
    vy *= NLEVELS;
    vy = (int)(vy);
    vy/= NLEVELS;
    return vy;
}

void GLWidget::rgb2hsv(float r, float g, float b, float* H, float* S, float* V){
    float M = max(r, max(g, b));
    float m = min(r, min(g, b));
    float d = M-m;
    *V = M;
    *S = (M>0.00001)? d/M:0;
    if (*S==0)
        *H=0;
    else{
        if(r==M)
            *H = (g-b)/d;
        else{
            if (g==M)
                *H = 2 + (b-r)/d;
            else
                *H = 4 + (r-g)/d;
            }
        *H /= 6;
        if (*H<0)
            *H += 1;
    }
}

void GLWidget::hsv2rgb(float* R, float* G, float* B, float H, float S, float V){
    int hueCase = (int)(H*6);
    float frac = 6*H-hueCase;
    float lx = V*(1-S);
    float ly = V*(1-S*frac);
    float lz = V*(1-S*(1-frac));

    switch(hueCase){
        case 0:
        case 6: *R=V;  *G=lz; *B=lx; break;
        case 1: *R=ly; *G=V;  *B=lx; break;
        case 2: *R=lx; *G=V;  *B=lz; break;
        case 3: *R=lx; *G=ly; *B=V; break;
        case 4: *R=lz; *G=lx; *B=V; break   ;
        case 5: *R=V;  *G=lx; *B=ly; break;
    }
}



//set_colormap: Sets three different types of colormaps
void GLWidget::set_colormap(float vy)
{
    //Color scaling
    if(color_scaling && color_scale_max > color_scale_min)
        vy = color_scale_min + vy*(color_scale_max - color_scale_min);

    //Color clamping
    if (color_clamping && color_clamp_max > color_clamp_min)
        (vy < color_clamp_min ? vy = color_clamp_min: (vy > color_clamp_max ? vy = color_clamp_max : vy = vy));

    vy = colorBands(vy, bands);
    float R,G,B;
    if (scalar_col==COLOR_BLACKWHITE){
        R = G = B = vy;
    }
    else if (scalar_col==COLOR_RAINBOW)
        rainbow(vy,&R,&G,&B);
    else if (scalar_col==COLOR_HEAT)
        heatMap(vy,&R,&G,&B);
    else if (scalar_col==COLOR_ZEBRA)
        zebra(vy,&R,&G,&B);

    float H,S,V;
    rgb2hsv(R, G, B, &H, &S, &V);
    H = (hue/3.14)+H;
    H = clamp(H, 0.00, 1.00);
    S = saturation*S;
    S = clamp(S, 0.00, 1.00);
    hsv2rgb(&R, &G, &B, H, S, V);
    glColor3f(R, G, B);
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

float GLWidget:: get_scalar(int i){
    switch(scalar_data_set){
        case 0: return simulation.get_rho()[i];
        case 1: {
            fftw_real* vx = simulation.get_vx();
            fftw_real* vy = simulation.get_vy();
            return simulation.get_length(vx[i], vy[i]);
        };
        case 2: {
            fftw_real* fx = simulation.get_fx();
            fftw_real* fy = simulation.get_fy();
            return simulation.get_length(fx[i], fy[i]);
        };
    }

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


                set_colormap(get_scalar(idx0));    glVertex2f(px0-1, py0-1);
                set_colormap(get_scalar(idx1));    glVertex2f(px1-1, py1-1);
                set_colormap(get_scalar(idx2));    glVertex2f(px2-1, py2-1);


                set_colormap(get_scalar(idx0));    glVertex2f(px0-1, py0-1);
                set_colormap(get_scalar(idx2));    glVertex2f(px2-1, py2-1);
                set_colormap(get_scalar(idx3));    glVertex2f(px3-1, py3-1);
            }
        }
        glEnd();
    }

    if (draw_vecs)
    {
        float vector_x, vector_y, length;
        glBegin(GL_LINES);
        for (i = 0; i < DIM; i++)
            for (j = 0; j < DIM; j++)
            {
                idx = (j * DIM) + i;
                if(vector_data_set){
                    length = simulation.get_length(simulation.get_fx()[idx], simulation.get_fy()[idx]);
                    vector_x = (simulation.get_fx()[idx]/length - simulation.min_f)/(simulation.max_f - simulation.min_f);
                    vector_y = (simulation.get_fy()[idx]/length - simulation.min_f)/(simulation.max_f - simulation.min_f);

                }
                else{
                    length = simulation.get_length(simulation.get_vx()[idx], simulation.get_vy()[idx]);
                    vector_x = (simulation.get_vx()[idx]/length - simulation.min_v)/(simulation.max_v - simulation.min_v);
                    vector_y = (simulation.get_vy()[idx]/length - simulation.min_v)/(simulation.max_v - simulation.min_v);
                }
                direction_to_color(vector_x,vector_y,color_dir);
                glVertex2f((wn + (fftw_real)i * wn) - 1, (hn + (fftw_real)j * hn) - 1);
                glVertex2f(((wn + (fftw_real)i * wn) + vec_scale * vector_x) - 1, ((hn + (fftw_real)j * hn) + vec_scale * vector_y) -1);
            }
        glEnd();
    }
    drawColorBar();
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
}

void GLWidget::setVecScale(int vs){
    vec_scale = vs;
}

void GLWidget::setSaturation(float sat){
    saturation = sat;
}

void GLWidget::setHue(float h){
    hue = h;
}

void GLWidget::toggleColorScaling(bool checked){
    color_scaling = checked;
}

void GLWidget::setColorScaleMax(int max){
    color_scale_max = max*0.01;
}


void GLWidget::setColorScaleMin(int min){
    color_scale_min = (50-min)*0.01;
}


void GLWidget::toggleColorClamping(bool checked){
    color_clamping = checked;
}

void GLWidget::setColorClampMax(int max){
    color_clamp_max = max*0.01;
}

void GLWidget::setColorClampMin(int min){
    color_clamp_min = (50-min)*0.01;
}

void GLWidget:: setScalarDataSet(int value){
    scalar_data_set = value;
}

void GLWidget:: setVectorDataSet(int value){
    vector_data_set = value;
}

void GLWidget:: setGlyphsSampleAmountX(int value){
    glyph_sample_amt_x = value;
}

void GLWidget:: setGlyphsSampleAmountY(int value){
    glyph_sample_amt_y = value;
}
