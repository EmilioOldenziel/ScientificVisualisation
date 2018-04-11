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

void GLWidget::draw_hedgehog(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j)
{
    glBegin(GL_LINES);
    glVertex2f((wn + (fftw_real)i * wn) - 1, (hn + (fftw_real)j * hn) - 1);
    glVertex2f(((wn + (fftw_real)i * wn) + vec_scale * vector.x()) - 1, ((hn + (fftw_real)j * hn) + vec_scale * vector.y()) -1);
    glEnd();
}

void GLWidget::draw_arrow(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j)
{
    float x1 = (wn + (fftw_real)i * wn) - 1;
    float y1 = (hn + (fftw_real)j * hn) - 1;
    float x2 = ((wn + (fftw_real)i * wn) + vec_scale * vector.x()) - 1.0;
    float y2 = ((hn + (fftw_real)j * hn) + vec_scale * vector.y()) - 1.0;

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    glBegin(GL_TRIANGLES);
    QVector3D perp_left =  QVector3D(-vector.y(), vector.x(), 0);
    QVector3D perp_right = QVector3D(-perp_left.x(), -perp_left.y(), 0);

    perp_left  *= vec_scale* 0.2;
    perp_right *= vec_scale *0.2;

    perp_left +=  QVector3D(((wn + (fftw_real)i * wn)) + vec_scale *0.6 * vector.x() - 1, ((hn + (fftw_real)j * hn))+ vec_scale* 0.6 * vector.y() -1, 0);
    perp_right += QVector3D(((wn + (fftw_real)i * wn)) + vec_scale *0.6 * vector.x() - 1, ((hn + (fftw_real)j * hn))+ vec_scale* 0.6 * vector.y() -1, 0);

    glVertex2f(x2, y2);
    glVertex2f(perp_left.x(),perp_left.y());
    glVertex2f(perp_right.x(),perp_right.y());

    glEnd();
}

void GLWidget::draw_cone(QVector3D vector, fftw_real wn, fftw_real hn, int i, int j)
{
    float x1 = (wn + (fftw_real)i * wn) - 1;
    float y1 = (hn + (fftw_real)j * hn) - 1;
    float x2 = ((wn + (fftw_real)i * wn) + vec_scale * vector.x()) - 1.0;
    float y2 = ((hn + (fftw_real)j * hn) + vec_scale * vector.y()) - 1.0;

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    glBegin(GL_TRIANGLES);
    QVector3D perp_left =  QVector3D(-vector.y(), vector.x(), 0);
    QVector3D perp_right = QVector3D(-perp_left.x(), -perp_left.y(), 0);

    perp_left  *= vec_scale * 0.2;
    perp_right *= vec_scale * 0.2;

    perp_left +=  QVector3D(((wn + (fftw_real)i * wn)) + - 1, ((hn + (fftw_real)j * hn))+  -1, 0);
    perp_right += QVector3D(((wn + (fftw_real)i * wn)) + - 1, ((hn + (fftw_real)j * hn))+  -1, 0);

    glVertex2f(x2, y2);
    glVertex2f(perp_left.x(),perp_left.y());
    glVertex2f(perp_right.x(),perp_right.y());

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

float GLWidget::divergence(int i){

    fftw_real* vx = simulation.get_vx();
    fftw_real* vy = simulation.get_vy();

    float  wn = 2.0 / DIM;   // Grid cell width
    float  hn = 2.0 / DIM;  // Grid cell height

    float upper = simulation.get_length(vx[i-DIM], vy[i-DIM]);
    float below = simulation.get_length(vx[i+DIM], vy[i+DIM]);
    float left =  simulation.get_length(vx[i-1], vy[i-1]);
    float right = simulation.get_length(vx[i+1], vy[i+1]);

    float diff_x = (left-right)/wn;
    float diff_y = (upper-below)/hn;



    return (diff_x + diff_y + 1.0)/2.0;
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
    }
}

//visualize: This is the main visualization function
void GLWidget::visualize(void)
{
    int i, j, i_sim, j_sim, idx;
    fftw_real  wn = (fftw_real)2.0 / (fftw_real)(glyph_sample_amt_x + 1);   // Grid cell width
    fftw_real  hn = (fftw_real)2.0 / (fftw_real)(glyph_sample_amt_y + 1);  // Grid cell heigh
    if (draw_smoke)
    {
        int idx0, idx1, idx2, idx3;
        double px0, py0, px1, py1, px2, py2, px3, py3;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLES);
        for (j = 0; j < glyph_sample_amt_y - 1; j++)            //draw smoke
        {
            for (i = 0; i < glyph_sample_amt_x - 1; i++)
            {
                i_sim = floor((i/(float)glyph_sample_amt_x) * DIM);
                j_sim = floor((j/(float)glyph_sample_amt_y) * DIM);

                px0 = wn + (fftw_real)i * wn;
                py0 = hn + (fftw_real)j * hn;
                idx0 = (j_sim * DIM) + i_sim;

                px1 = wn + (fftw_real)i * wn;
                py1 = hn + (fftw_real)(j + 1) * hn;
                idx1 = ((j_sim + 1) * DIM) + i_sim;


                px2 = wn + (fftw_real)(i + 1) * wn;
                py2 = hn + (fftw_real)(j + 1) * hn;
                idx2 = ((j_sim + 1) * DIM) + (i_sim + 1);


                px3 = wn + (fftw_real)(i + 1) * wn;
                py3 = hn + (fftw_real)j * hn;
                idx3 = (j_sim * DIM) + (i_sim + 1);

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
        for (i = 0; i < glyph_sample_amt_x; i++)
            for (j = 0; j < glyph_sample_amt_y; j++)
            {
                i_sim = floor((i/(float)glyph_sample_amt_x) * DIM);
                j_sim = floor((j/(float)glyph_sample_amt_y) * DIM);
                QVector3D p1, p2, p3, p4, vector;

                if(vector_data_set){
                    p1 = QVector3D(simulation.get_fx()[j_sim*DIM + i_sim],       simulation.get_fy()[j_sim*DIM + i_sim], 0.0);
                    p2 = QVector3D(simulation.get_fx()[j_sim*DIM + i_sim+1],     simulation.get_fy()[j_sim*DIM + i_sim+1], 0.0);
                    p3 = QVector3D(simulation.get_fx()[(j_sim+1)*DIM + i_sim+1], simulation.get_fy()[(j_sim+1)*DIM + i_sim+1], 0.0);
                    p4 = QVector3D(simulation.get_fx()[(j_sim+1)*DIM + i_sim],   simulation.get_fy()[(j_sim+1)*DIM + i_sim], 0.0);
                    vector = interpolation(QVector3D(i * wn, j * hn, 0), p1, p2, p3, p4);
                }
                else{
                    p1 = QVector3D(simulation.get_vx()[j_sim*DIM + i_sim],       simulation.get_vy()[j_sim*DIM + i_sim], 0.0);
                    p2 = QVector3D(simulation.get_vx()[j_sim*DIM + i_sim+1],     simulation.get_vy()[j_sim*DIM + i_sim+1], 0.0);
                    p3 = QVector3D(simulation.get_vx()[(j_sim+1)*DIM + i_sim+1], simulation.get_vy()[(j_sim+1)*DIM + i_sim+1], 0.0);
                    p4 = QVector3D(simulation.get_vx()[(j_sim+1)*DIM + i_sim],   simulation.get_vy()[(j_sim+1)*DIM + i_sim], 0.0);
                    vector = interpolation(QVector3D(i * wn, j * hn, 0), p1, p2, p3, p4);
                }
                direction_to_color(vector.x(),vector.y(),color_dir);

                switch(glyph_shape){
                    case 0: draw_hedgehog(vector, wn, hn, i, j); break;
                    case 1: draw_arrow(vector, wn, hn, i, j); break;
                    case 2: draw_cone(vector, wn, hn, i, j); break;
                }
            }
    }
    if (isolines){
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);

        float p1, p2, p3, p4;
        float x1, y1, x2, y2;
        std::bitset<4> marching;
        long marching_case;
        for (i = 0; i < glyph_sample_amt_x; i++)
            for (j = 0; j < glyph_sample_amt_y; j++)
            {
                marching.reset();
                i_sim = floor((i/(float)glyph_sample_amt_x) * DIM);
                j_sim = floor((j/(float)glyph_sample_amt_y) * DIM);
                p1 = simulation.get_rho()[j_sim*DIM + i_sim];
                p2 = simulation.get_rho()[j_sim*DIM + i_sim+1];
                p3 = simulation.get_rho()[(j_sim+1)*DIM + i_sim+1];
                p4 = simulation.get_rho()[(j_sim+1)*DIM + i_sim];

                if(p1>iso_threshold) marching.flip(3);
                if(p2>iso_threshold) marching.flip(2);
                if(p3>iso_threshold) marching.flip(1);
                if(p4>iso_threshold) marching.flip(0);

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

void GLWidget::toggleDivergence(bool checked){
    diver = checked;
}

void GLWidget::setGlyphShape(int value){
    glyph_shape = value;
}

void GLWidget::setIsolines(int value){
    isolines = value;
}

void GLWidget::setIsolineThreshold(float pos){
    iso_threshold = pos;
}
