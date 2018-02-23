#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

protected:

private slots:

private:
    GLWidget *glWidget;
    MainWindow *mainWindow;
};

#endif
