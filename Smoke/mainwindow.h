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

    void on_glyphBox_activated(const QString &arg1);

    void on_iso_checkbox_toggled(bool checked);

    void on_isolineSlider_sliderMoved(int position);

    void on_isolinMinSlider_sliderMoved(int position);

    void on_isolineMaxSlider_sliderMoved(int position);

    void on_isolineNSlider_sliderMoved(int position);

    void on_isolineOptionDropdown_activated(const QString &arg1);

    void on_heightplot_checkbox_toggled(bool checked);

    void on_zoomSlider_sliderMoved(int position);

    void on_rotateSlider_sliderMoved(int position);

    void on_heightColorDataSetBox_activated(const QString &arg1);

    void on_isolineThicknessSlider_sliderMoved(int position);

    void on_jitterCheckbox_toggled(bool checked);

    void on_glyphColorComboBox_activated(const QString &arg1);

    void on_magnitudeCheckbox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
