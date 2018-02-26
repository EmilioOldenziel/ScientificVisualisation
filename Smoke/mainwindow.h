#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_smoke_checkbox_toggled(bool checked);
    void on_vecs_checkbox_toggled(bool checked);

    void on_colorBox_activated(const QString &arg1);

    void on_bandsSlider_sliderMoved(int position);

    void on_vecScaleSlider_sliderMoved(int position);

    void on_vecsColor_checkbox_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
