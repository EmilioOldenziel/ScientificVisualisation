#include "glwidget.h"
#include <QMouseEvent>
#include <math.h>
#include <QTimer>
#include <QColor>
#include <QVector3D>
#include <QGenericMatrix>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    DIM = 50;
    simulation.init_simulation(DIM);
    QTimer *timer = new QTimer;
    timer->start();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(do_one_simulation_step()));
    jitter_x = (float *) malloc(10000*sizeof(float));
    jitter_y = (float *) malloc(10000*sizeof(float));
    for(int i = 0; i < 10000; i++){
        float random_numb = (rand()%10)/10.0;
        if(rand()%2)
            random_numb *= -1;
        jitter_x[i] = random_numb;
        jitter_y[i]= random_numb;
    }
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


void GLWidget::draw_isoline(float threshold, fftw_real wn, fftw_real hn){
    set_colormap(threshold);
    glLineWidth(iso_thickness);
    glBegin(GL_LINES);
    float p1, p2, p3, p4;
    float x1, y1, x2, y2;
    int i, j, i_sim, j_sim, idx;
    std::bitset<4> marching;
    long marching_case;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
        {
            marching.reset();

            p1 = simulation.get_rho()[j*DIM + i];
            p2 = simulation.get_rho()[j*DIM + i+1];
            p3 = simulation.get_rho()[(j+1)*DIM + i + 1];
            p4 = simulation.get_rho()[(j+1)*DIM + i];

            if(p1>threshold) marching.flip(3);
            if(p2>threshold) marching.flip(2);
            if(p3>threshold) marching.flip(1);
            if(p4>threshold) marching.flip(0);

            marching_case = marching.to_ulong();
            switch (marching_case){
                case 0: break;
                case 1:  x1 =  i*wn;         y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = (j+1)*hn;  break;
                case 2:  x1 = (i+1)*wn;      y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = (j+1)*hn; break;
                case 3:  x1 =  i*wn;         y1 = (j+0.5)*hn;    x2 = (i+1)*wn;      y2 = (j+0.5)*hn;break;
                case 4:  x1 = (i+1)*wn;      y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = j*hn; break;
                case 5:  x1 = i*wn;          y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = j*hn; break;
                case 6:  x1 = (i+0.5)*wn;    y1 = j*hn;          x2 = (i+0.5)*wn;    y2 = (j+1)*hn; break;
                case 7:  x1 = i*wn;          y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = j*hn; break;
                case 8:  x1 = i*wn;          y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = j*hn; break;
                case 9:  x1 = (i+0.5)*wn;    y1 = j*hn;          x2 = (i+0.5)*wn;    y2 = (j+1)*hn; break;
                case 10: x1 = i*wn;          y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = (j+1)*hn;  break;
                case 11: x1 = (i+1)*wn;      y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = j*hn; break;
                case 12: x1 = i*wn;          y1 = (j+0.5)*hn;    x2 = (i+1)*wn;      y2 = (j+0.5)*hn; break;
                case 13: x1 = (i+1)*wn;      y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = (j+1)*hn; break;
                case 14: x1 = i*wn;          y1 = (j+0.5)*hn;    x2 = (i+0.5)*wn;    y2 = (j+1)*hn;  break;
                case 15: break;
            }

            glVertex2f(x1-1, y1-1);
            glVertex2f(x2-1, y2-1);

            if(marching_case == 5){
                x1 = (i+1)*wn;      y1 = (j+0.5)*hn;
                x2 = (i+0.5)*wn;    y2 = (j+1)*hn;
                glVertex2f(x1-1, y1-1);
                glVertex2f(x2-1, y2-1);
            }
            if(marching_case == 10){
                x1 = (i+1)*wn;      y1 = (j+0.5)*hn;
                x2 = (i+0.5)*wn;    y2 = j*hn;
                glVertex2f(x1-1, y1-1);
                glVertex2f(x2-1, y2-1);
            }

        }
    glEnd();
    glLineWidth(1.0);

}


void GLWidget::draw_hedgehog(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j)
{
    float jx = 0;
    float jy = 0;

    if(jitter){
        jx = jitter_x[(i*glyph_sample_amt_x) + j] * 0.1 * wn;
        jy = jitter_y[(i*glyph_sample_amt_x) + j] * 0.1 * hn;
    }

    glBegin(GL_LINES);
    glVertex2f((wn + (fftw_real)i * wn) - 1 + jx, (hn + (fftw_real)j * hn) - 1 + jy);
    glVertex2f(((wn + (fftw_real)i * wn) + vec_scale * vector.x()) - 1 + jx, ((hn + (fftw_real)j * hn) + vec_scale * vector.y()) -1 + jy);
    glEnd();
}

void GLWidget::draw_arrow(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j)
{

    float jx = 0;
    float jy = 0;

    if(jitter){
        jx = jitter_x[(i*glyph_sample_amt_x) + j] * 0.1 * wn;
        jy = jitter_y[(i*glyph_sample_amt_x) + j] * 0.1 * hn;
    }

    float x1 = (wn + (fftw_real)i * wn) - 1;
    float y1 = (hn + (fftw_real)j * hn) - 1;
    float x2 = ((wn + (fftw_real)i * wn) + vec_scale * vector.x()) - 1.0;
    float y2 = ((hn + (fftw_real)j * hn) + vec_scale * vector.y()) - 1.0;

    glBegin(GL_LINES);
    glVertex2f(x1 + jx, y1+jy);
    glVertex2f(x2 +jx, y2+jy);
    glEnd();

    glBegin(GL_TRIANGLES);
    QVector3D perp_left =  QVector3D(-vector.y(), vector.x(), 0);
    QVector3D perp_right = QVector3D(-perp_left.x(), -perp_left.y(), 0);

    perp_left  *= vec_scale* 0.2;
    perp_right *= vec_scale *0.2;

    perp_left +=  QVector3D(((wn + (fftw_real)i * wn)) + vec_scale *0.6 * vector.x() - 1, ((hn + (fftw_real)j * hn))+ vec_scale* 0.6 * vector.y() -1, 0);
    perp_right += QVector3D(((wn + (fftw_real)i * wn)) + vec_scale *0.6 * vector.x() - 1, ((hn + (fftw_real)j * hn))+ vec_scale* 0.6 * vector.y() -1, 0);

    glVertex2f(x2+jx, y2+jy);
    glVertex2f(perp_left.x()+jx,perp_left.y()+jy);
    glVertex2f(perp_right.x()+jx,perp_right.y()+jy);

    glEnd();
}

void GLWidget::draw_cone(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j)
{

    float jx = 0;
    float jy = 0;

    if(jitter){
        jx = jitter_x[(i*glyph_sample_amt_x) + j] * 0.1 * wn;
        jy = jitter_y[(i*glyph_sample_amt_x) + j] * 0.1 * hn;
    }

    float x1 = (wn + (fftw_real)i * wn) - 1 ;
    float y1 = (hn + (fftw_real)j * hn) - 1 ;
    float x2 = ((wn + (fftw_real)i * wn) + vec_scale * vector.x()) - 1.0 ;
    float y2 = ((hn + (fftw_real)j * hn) + vec_scale * vector.y()) - 1.0 ;

    glBegin(GL_LINES);
    glVertex2f(x1 + jx, y1 + jy);
    glVertex2f(x2 + jx, y2 + jy);
    glEnd();

    glBegin(GL_TRIANGLES);
    QVector3D perp_left =  QVector3D(-vector.y(), vector.x(), 0);
    QVector3D perp_right = QVector3D(-perp_left.x(), -perp_left.y(), 0);

    perp_left  *= vec_scale * 0.2;
    perp_right *= vec_scale * 0.2;

    perp_left +=  QVector3D(((wn + (fftw_real)i * wn)) + - 1, ((hn + (fftw_real)j * hn))+  -1, 0);
    perp_right += QVector3D(((wn + (fftw_real)i * wn)) + - 1, ((hn + (fftw_real)j * hn))+  -1, 0);

    glVertex2f(x2 + jx, y2 + jy);
    glVertex2f(perp_left.x() + jx,perp_left.y() + jy);
    glVertex2f(perp_right.x() + jx,perp_right.y() + jy);

    glEnd();
}

QVector3D GLWidget::interpolation(QVector3D pos_to_visualise, QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4){

    QVector3D interpolated = QVector3D(0,0,0);

    float dist1 = pos_to_visualise.distanceToPoint(QVector3D(0.0,0.0,0.0));
    float dist2 = pos_to_visualise.distanceToPoint(QVector3D(1.0,0.0,0.0));
    float dist3 = pos_to_visualise.distanceToPoint(QVector3D(1.0,1.0,0.0));
    float dist4 = pos_to_visualise.distanceToPoint(QVector3D(0.0,1.0,0.0));

    float weight_1 = 1/dist1;
    float weight_2 = 1/dist2;
    float weight_3 = 1/dist3;
    float weight_4 = 1/dist4;

    interpolated = interpolated + (weight_1*p1);
    interpolated = interpolated + (weight_2*p2);
    interpolated = interpolated + (weight_3*p3);
    interpolated = interpolated + (weight_4*p4);

   return interpolated.normalized()*vec_scale;
}

float GLWidget::color_interpolation(QVector3D pos_to_visualise, float v1, float v2, float v3, float v4){

    float interpolated = 0.0;

    float dist1 = pos_to_visualise.distanceToPoint(QVector3D(0.0,0.0,0.0));
    float dist2 = pos_to_visualise.distanceToPoint(QVector3D(1.0,0.0,0.0));
    float dist3 = pos_to_visualise.distanceToPoint(QVector3D(1.0,1.0,0.0));
    float dist4 = pos_to_visualise.distanceToPoint(QVector3D(0.0,1.0,0.0));

    float weight_1 = 1/dist1;
    float weight_2 = 1/dist2;
    float weight_3 = 1/dist3;
    float weight_4 = 1/dist4;

    interpolated = interpolated + (weight_1*v1);
    interpolated = interpolated + (weight_2*v2);
    interpolated = interpolated + (weight_3*v3);
    interpolated = interpolated + (weight_4*v4);

   return interpolated/4;
}


//draw colorbar besides the simulation
void GLWidget::drawColorBar()
{
    glPushMatrix();
    glBegin(GL_QUADS);

    float offset = (0.85);
    float bar_width = (0.1);
    float bar_height = (0.0075);

    //draw background
    set_colormap(0.5);
    glVertex3f(offset-0.005,              -100*bar_height-0.005,       0.0);
    glVertex3f((offset+bar_width+0.005),    -100*bar_height-0.005,       0.0);
    glVertex3f((offset+bar_width+0.005),    100*bar_height+0.005,   0.0);
    glVertex3f(offset-0.005,              100*bar_height+0.005,   0.0);

    // draw bars
    for(int i = -100; i<100; i++){
        set_colormap((i+100)*0.5*0.01);
        glVertex3f(offset,              i*bar_height,       0.0);
        glVertex3f((offset+bar_width),    i*bar_height,       0.0);
        glVertex3f((offset+bar_width),    (i+1)*bar_height,   0.0);
        glVertex3f(offset,              (i+1)*bar_height,   0.0);
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
    float band_width = 1.0/10;
    if (value<0.0) value=0.0; if (value>1.0) value=1.0;
    int val = (value / band_width);
    if (val % 2){
        *R = *G = *B = 1.0;
    }else{
        *R = *G = *B = 0.0;
    }
}


float GLWidget::colorBands(float vy, int bands){
    float start_vy = vy;
    vy = clamp(vy, 0.0, 0.999999);
    vy *= bands;
    vy = (int)(vy);
    vy /= bands;
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
{    fftw_real* vx = simulation.get_vx();

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
void GLWidget::direction_to_color(float x, float y)
{
    float r,g,b,f;
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
   glColor3f(r,g,b);
}

float GLWidget:: get_scalar(int i){
    switch(scalar_data_set){
        //rho
        case 0: return simulation.get_rho()[i];
        //velocity
        case 1: {
            fftw_real* vx = simulation.get_vx();
            fftw_real* vy = simulation.get_vy();
            return simulation.get_length(vx[i], vy[i]);
        };
        //force
        case 2: {
            fftw_real* fx = simulation.get_fx();
            fftw_real* fy = simulation.get_fy();
            return simulation.get_length(fx[i], fy[i]);
        };

        //divergence
        case 3: {
            float div = clamp(simulation.get_divergence()[i], -1.0,1.0);
            return (div+1.0)*0.5;
        }
    }
}

float GLWidget:: get_scalar_height(int i){
    switch(scalar_data_set_height_color){
        //rho
        case 0: return simulation.get_rho()[i];
        //velocity
        case 1: {
            fftw_real* vx = simulation.get_vx();
            fftw_real* vy = simulation.get_vy();
            return simulation.get_length(vx[i], vy[i]);
        };
        //force
        case 2: {
            fftw_real* fx = simulation.get_fx();
            fftw_real* fy = simulation.get_fy();
            return simulation.get_length(fx[i], fy[i]);
        };
        //divergence
        case 3: {
            float div = clamp(simulation.get_divergence()[i], -1.0,1.0);
            return (div+1.0)*0.5;
        }
    }
}

//visualize: This is the main visualization function
void GLWidget::visualize(void)
{
    int i, j;
    fftw_real  wn = (fftw_real)2.0 / (fftw_real)(DIM + 1);   // Grid cell width
    fftw_real  hn = (fftw_real)2.0 / (fftw_real)(DIM + 1);  // Grid cell heigh

    if (draw_smoke)
    {
        int idx0, idx1, idx2, idx3;
        double px0, py0, px1, py1, px2, py2, px3, py3;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(height_plot)
            glBegin(GL_QUADS);
        else
            glBegin(GL_TRIANGLES);


        for (j = 0; j < DIM-1; j++)            //draw smoke
        {
            for (i = 0; i < DIM-1; i++)
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

                if(height_plot){
                    set_colormap(get_scalar_height(idx0));    glVertex3f(px0-1, py0-1,get_scalar_height(idx0));
                    set_colormap(get_scalar_height(idx1));    glVertex3f(px1-1, py1-1,get_scalar_height(idx1));
                    set_colormap(get_scalar_height(idx2));    glVertex3f(px2-1, py2-1, get_scalar_height(idx2));
                    set_colormap(get_scalar_height(idx3));    glVertex3f(px3-1, py3-1, get_scalar_height(idx3));
                }else{
                    set_colormap(get_scalar(idx0));    glVertex2f(px0-1, py0-1);
                    set_colormap(get_scalar(idx1));    glVertex2f(px1-1, py1-1);
                    set_colormap(get_scalar(idx2));    glVertex2f(px2-1, py2-1);

                    set_colormap(get_scalar(idx0));    glVertex2f(px0-1, py0-1);
                    set_colormap(get_scalar(idx2));    glVertex2f(px2-1, py2-1);
                    set_colormap(get_scalar(idx3));    glVertex2f(px3-1, py3-1);
                }

            }
        }
        glEnd();
    }
    int i_sim, j_sim;
    fftw_real  wn_glyph = (fftw_real)2.0 / (fftw_real)(glyph_sample_amt_x + 1);   // Grid cell width
    fftw_real  hn_glyph = (fftw_real)2.0 / (fftw_real)(glyph_sample_amt_y + 1);  // Grid cell heigth
    if (draw_vecs)
    {
        for (i = 0; i < glyph_sample_amt_x; i++)
            for (j = 1; j < glyph_sample_amt_y; j++)
            {
                i_sim = floor((i/(float)glyph_sample_amt_x) * DIM);
                j_sim = floor((j/(float)glyph_sample_amt_y) * DIM);
                QVector3D p1, p2, p3, p4, vector;

                if(vector_data_set){
                    p1 = QVector3D(simulation.get_fx()[j_sim*DIM + i_sim],       simulation.get_fy()[j_sim*DIM + i_sim], 0.0);
                    p2 = QVector3D(simulation.get_fx()[j_sim*DIM + i_sim+1],     simulation.get_fy()[j_sim*DIM + i_sim+1], 0.0);
                    p3 = QVector3D(simulation.get_fx()[(j_sim+1)*DIM + i_sim+1], simulation.get_fy()[(j_sim+1)*DIM + i_sim+1], 0.0);
                    p4 = QVector3D(simulation.get_fx()[(j_sim+1)*DIM + i_sim],   simulation.get_fy()[(j_sim+1)*DIM + i_sim], 0.0);
                    vector = interpolation(QVector3D(i * wn_glyph, j * hn_glyph, 0), p1, p2, p3, p4);
                }
                else{
                    p1 = QVector3D(simulation.get_vx()[j_sim*DIM + i_sim],       simulation.get_vy()[j_sim*DIM + i_sim], 0.0);
                    p2 = QVector3D(simulation.get_vx()[j_sim*DIM + i_sim+1],     simulation.get_vy()[j_sim*DIM + i_sim+1], 0.0);
                    p3 = QVector3D(simulation.get_vx()[(j_sim+1)*DIM + i_sim+1], simulation.get_vy()[(j_sim+1)*DIM + i_sim+1], 0.0);
                    p4 = QVector3D(simulation.get_vx()[(j_sim+1)*DIM + i_sim],   simulation.get_vy()[(j_sim+1)*DIM + i_sim], 0.0);
                    vector = interpolation(QVector3D(i * wn_glyph, j * hn_glyph, 0), p1, p2, p3, p4);
                }
                switch(glyph_color){
                    case 0:
                        set_colormap(1);
                        break;
                    case 1:
                        direction_to_color(vector.x(),vector.y());
                        break;
                    case 2:
                        set_colormap(color_interpolation(QVector3D(i * wn_glyph, j * hn_glyph, 0), get_scalar(j_sim*DIM + i_sim), get_scalar(j_sim*DIM + i_sim+1), get_scalar((j_sim+1)*DIM + i_sim+1), get_scalar((j_sim+1)*DIM + i_sim)));
                        break;
                }

                switch(glyph_shape){
                    case 0: draw_hedgehog(vector, wn_glyph, hn_glyph, i, j); break;
                    case 1: draw_arrow(vector, wn_glyph, hn_glyph, i, j); break;
                    case 2: draw_cone(vector, wn_glyph, hn_glyph, i, j); break;
                }
            }
    }
    if (isolines){
        if(!isoline_option){
            draw_isoline(iso_threshold, hn, wn);
        }
        else{
            float interval = (iso_max - iso_min) / iso_N;
            float steps;
            for(steps = iso_min; steps<=iso_max; steps += interval){
                draw_isoline(steps, hn, wn);
            }
        }
    }
}

void GLWidget::do_one_simulation_step()
{
    if(!frozen){
        simulation.set_forces(DIM);
        if(scalar_data_set == 3 || scalar_data_set_height_color == 3) {
            simulation.calc_divergence(vector_data_set, DIM);
        }
        simulation.solve(DIM, simulation.get_vx(), simulation.get_vy(), simulation.get_vx0(),
                         simulation.get_vy0(), simulation.get_visc(), simulation.get_dt());
        simulation.diffuse_matter(DIM, simulation.get_vx(), simulation.get_vy(),
                                  simulation.get_rho(), simulation.get_rho0(), simulation.get_dt());
        update();
    }
}

void GLWidget::paintGL()
{

    if(height_plot){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-zoom_view, zoom_view, -zoom_view, zoom_view, -zoom_view, zoom_view);
        glRotatef(340.0, 1.0, 0.0, 0.0);
        glRotatef(rotate_view, 0.0, 1.0, 0.0);
        glRotatef(217.0, 0.0, 0.0, 1.0);
    }
    else{
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float light[4] = {1,1,1,0};
    glLightfv(GL_LIGHT0, GL_POSITION, light);
    glEnable(GL_COLOR_MATERIAL);
    visualize();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    drawColorBar();
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

void GLWidget::setColor(int c){
    scalar_col = c;
}

void GLWidget::setColorBands(int b){
    bands = b;
}

void GLWidget::setVecScale(float vs){
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

void GLWidget::setGlyphShape(int value){
    glyph_shape = value;
}


void GLWidget::setIsolineOption(int value){
    isoline_option = value;
}

void GLWidget::setIsolines(int value){
    isolines = value;
}


void GLWidget::setIsolineThreshold(float pos){
    iso_threshold = pos;
}

void GLWidget::setIsolineMin(float pos){
    iso_min = pos;
}

void GLWidget::setIsolineMax(float pos){
    iso_max = pos;
}

void GLWidget::setIsolineN(int pos){
    iso_N = pos;
}

void GLWidget::isolineThickness(int value){
    iso_thickness = value;
}


void GLWidget::setHeightPlot(bool checked){
        height_plot = checked;
}

void GLWidget::setZoomView(int value){
    zoom_view = value;
}

void GLWidget::setRotateView(int value){
    rotate_view = value;
}

void GLWidget::setHeightColorScalarDataSet(int value){
    scalar_data_set_height_color = value;
}

int GLWidget::getBands(){
    return bands;
}

void GLWidget::setJitter(bool checked){
    jitter = checked;
}
void GLWidget::setGlyphColor(int value){
    glyph_color = value;
}
