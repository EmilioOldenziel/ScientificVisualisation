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

    void on_saturationSlider_sliderMoved(int position);

    void on_hueSlider_sliderMoved(int position);

    void on_colorScalingBox_toggled(bool checked);

    void on_colorScalingMaxSlider_sliderMoved(int position);

    void on_colorScalingMinSlider_sliderMoved(int position);

    void on_colorClampingBox_toggled(bool checked);

    void on_colorClampingMinSlider_sliderMoved(int position);

    void on_colorClampingMaxSlider_sliderMoved(int position);

    void on_dataSetBox_activated(const QString &arg1);

    void on_vectorDataSetBox_activated(const QString &arg1);

    void on_glyphSliderY_sliderMoved(int position);

    void on_glyphSliderX_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
