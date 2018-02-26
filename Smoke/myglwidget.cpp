class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) { }

protected:
    void initializeGL()
    {
        // Set up the rendering context, load shaders and other resources, etc.:
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        ...
    }

    void resizeGL(int w, int h)
    {
        // Update projection matrix and other size related settings:
        m_projection.setToIdentity();
    }

    void paintGL()
    {
        // Draw the scene:
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT);
    }

};
