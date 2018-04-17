#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// turn smoke on/off
void MainWindow::on_smoke_checkbox_toggled(bool checked)
{
    ui->openGLWidget->toggle_smoke(checked);
}

// turn gliphs on/off
void MainWindow::on_vecs_checkbox_toggled(bool checked)
{
    ui->openGLWidget->toggle_vecs(checked);
    ui->jitterCheckbox->setEnabled(checked);
    ui->vecScaleLabel->setEnabled(checked);
    ui->vecScaleSlider->setEnabled(checked);

}


// dropdown for colormap selection
void MainWindow::on_colorBox_activated(const QString &arg1)
{
    if(arg1 == "black-white"){
        ui->openGLWidget->setColor(0);
    }
    if(arg1 == "rainbow"){
        ui->openGLWidget->setColor(1);
    }
    if(arg1 == "heatmap"){
        ui->openGLWidget->setColor(2);
    }
    if(arg1 == "zebra"){
        ui->bandsLabel->setText(QStringLiteral("Number of bands: 10"));
        ui->bandsSlider->setEnabled(false);
        ui->openGLWidget->setColor(3);
    }
    else{
        ui->bandsSlider->setEnabled(true);
        ui->bandsLabel->setText(QStringLiteral("Number of bands: %1").arg(ui->openGLWidget->getBands()));
    }
}

// slider for amount of color bands
void MainWindow::on_bandsSlider_sliderMoved(int position)
{
    ui->openGLWidget->setColorBands(position);
    ui->bandsLabel->setText(QStringLiteral("Number of bands: %1").arg(position));
}

// slider for vector scale
void MainWindow::on_vecScaleSlider_sliderMoved(int position)
{
    float pos = position/100.0;
    ui->openGLWidget->setVecScale(pos);
    ui->vecScaleLabel->setText(QStringLiteral("Glyph Scaling: %1").arg(pos));
}

void MainWindow::on_saturationSlider_sliderMoved(int position)
{
    float pos;
    if(position != 0)
        pos = position/100.00;
    else
        pos = 0;

    ui->openGLWidget->setSaturation(pos);
    ui->saturationLabel->setText(QStringLiteral("Saturation: %1").arg(pos));
}

void MainWindow::on_hueSlider_sliderMoved(int position)
{
    float pos;
    if(position != 0)
        pos = position/100.00;
    else
        pos = 0;

    ui->openGLWidget->setHue(pos);
    ui->hueLabel->setText(QStringLiteral("Hue: %1").arg(pos));
}

void MainWindow::on_colorScalingBox_toggled(bool checked)
{
    ui->openGLWidget->toggleColorScaling(checked);
    ui->colorScalingMaxSlider->setEnabled(checked);
    ui->colorScalingMinSlider->setEnabled(checked);
}

void MainWindow::on_colorScalingMaxSlider_sliderMoved(int position)
{
    ui->openGLWidget->setColorScaleMax(position);
}

void MainWindow::on_colorScalingMinSlider_sliderMoved(int position)
{
    ui->openGLWidget->setColorScaleMin(position);
}

void MainWindow::on_colorClampingBox_toggled(bool checked)
{
    ui->openGLWidget->toggleColorClamping(checked);
    ui->colorClampingMaxSlider->setEnabled(checked);
    ui->colorClampingMinSlider->setEnabled(checked);
}

void MainWindow::on_colorClampingMinSlider_sliderMoved(int position)
{
    ui->openGLWidget->setColorClampMin(position);

}

void MainWindow::on_colorClampingMaxSlider_sliderMoved(int position)
{
    ui->openGLWidget->setColorClampMax(position);

}

void MainWindow::on_dataSetBox_activated(const QString &arg1)
{
    if(arg1 == "density"){
        ui->openGLWidget->setScalarDataSet(0);
    }
    if(arg1 == "fluid velocity magnitude"){
        ui->openGLWidget->setScalarDataSet(1);
    }
    if(arg1 == "force field magnitude"){
        ui->openGLWidget->setScalarDataSet(2);
    }
    if(arg1 == "divergence"){
        ui->openGLWidget->setScalarDataSet(3);
    }
}

void MainWindow::on_vectorDataSetBox_activated(const QString &arg1)
{
    if(arg1 == "velocity"){
        ui->openGLWidget->setVectorDataSet(0);
    }
    if(arg1 == "force"){
        ui->openGLWidget->setVectorDataSet(1);
    }
}

void MainWindow::on_glyphSliderX_sliderMoved(int position)
{
    ui->openGLWidget->setGlyphsSampleAmountX(position);
    ui->glyphLabelX->setText(QStringLiteral("Glyphs sampling X: %1").arg(position));
}

void MainWindow::on_glyphSliderY_sliderMoved(int position)
{
    ui->openGLWidget->setGlyphsSampleAmountY(position);
    ui->glyphLabelY->setText(QStringLiteral("Glyphs sampling Y: %1").arg(position));
}



void MainWindow::on_glyphBox_activated(const QString &arg1)
{
    if(arg1 == "hedgehogs"){
        ui->openGLWidget->setGlyphShape(0);
    }
    if(arg1 == "arrows"){
        ui->openGLWidget->setGlyphShape(1);
    }    if(arg1 == "cones"){
        ui->openGLWidget->setGlyphShape(2);
    }
}

void MainWindow::on_iso_checkbox_toggled(bool checked)
{
    if(checked){
        ui->isolineOptionDropdown->setEnabled(checked);
        ui->isolineSlider->setEnabled(checked);
        ui->openGLWidget->setIsolines(checked);
        ui->isolineThicknessSlider->setEnabled(checked);
    }
    else{
        ui->isolineOptionDropdown->setEnabled(checked);
        ui->isolineOptionDropdown->setCurrentIndex(0);
        ui->openGLWidget->setIsolineOption(0);
        ui->isolineSlider->setEnabled(checked);
        ui->openGLWidget->setIsolines(checked);
        ui->isolineNSlider->setEnabled(false);
        ui->isolinMinSlider->setEnabled(false);
        ui->isolineMaxSlider->setEnabled(false);
        ui->isolineThicknessSlider->setEnabled(false);
    }
}

void MainWindow::on_isolineSlider_sliderMoved(int position)
{
    float pos = position/100.0;
    ui->openGLWidget->setIsolineThreshold(pos);
    ui->isolineScaleLabel->setText(QStringLiteral("Isoline threshold: %1").arg(pos));
}

void MainWindow::on_isolinMinSlider_sliderMoved(int position)
{
    float pos = -1*(position-50)/100.0;
    ui->isolineMinLabel->setText(QStringLiteral("Min: %1").arg(pos));
    ui->openGLWidget->setIsolineMin(pos);
}

void MainWindow::on_isolineMaxSlider_sliderMoved(int position)
{
    float pos = position/100.0;
    ui->isolineMaxLabel->setText(QStringLiteral("Max: %1").arg(pos));
    ui->openGLWidget->setIsolineMax(pos);
}

void MainWindow::on_isolineNSlider_sliderMoved(int pos)
{
    ui->isolineNLabel->setText(QStringLiteral("Amount of isolines: %1").arg(pos));
    ui->openGLWidget->setIsolineN(pos);
}

void MainWindow::on_isolineOptionDropdown_activated(const QString &arg1)
{
    if(arg1 == "threshold"){
        ui->isolineSlider->setEnabled(true);
        ui->isolineNSlider->setEnabled(false);
        ui->isolinMinSlider->setEnabled(false);
        ui->isolineMaxSlider->setEnabled(false);
        ui->openGLWidget->setIsolineOption(0);
    }
    else{
        ui->isolineSlider->setEnabled(false);
        ui->isolineNSlider->setEnabled(true);
        ui->isolinMinSlider->setEnabled(true);
        ui->isolineMaxSlider->setEnabled(true);
        ui->openGLWidget->setIsolineOption(1);
    }
}

void MainWindow::on_heightplot_checkbox_toggled(bool checked)
{
    ui->zoomSlider->setEnabled(checked);
    ui->heightColorDataSetBox->setEnabled(checked);
    ui->rotateSlider->setEnabled(checked);
    ui->zoomSlider->setEnabled(checked);
    //put glyphs off
    ui->openGLWidget->toggle_vecs(!checked);
    ui->openGLWidget->toggle_vecs(!checked);
    ui->vecs_checkbox->setEnabled(!checked);
    ui->vecScaleSlider->setEnabled(!checked);
    ui->glyphSliderX->setEnabled(!checked);
    ui->glyphSliderY->setEnabled(!checked);
    ui->jitterCheckbox->setEnabled(!checked);
    ui->openGLWidget->toggle_vecs(!checked);
    ui->vectorDataSetBox->setEnabled(!checked);
    ui->glyphBox->setEnabled(!checked);
    //put isoliness off
    ui->openGLWidget->setIsolines(0);
    ui->iso_checkbox->setEnabled(!checked);
    ui->isolineSlider->setEnabled(!checked);
    ui->isolineNSlider->setEnabled(!checked);
    ui->isolinMinSlider->setEnabled(!checked);
    ui->isolineMaxSlider->setEnabled(!checked);

    ui->openGLWidget->setHeightPlot(checked);
}

void MainWindow::on_zoomSlider_sliderMoved(int position)
{
    ui->zoomLabel->setText(QStringLiteral("Zoom scale: %1").arg(position));
    ui->openGLWidget->setZoomView(position);
}

void MainWindow::on_rotateSlider_sliderMoved(int position)
{
    ui->rotateLabel->setText(QStringLiteral("Rotate: %1").arg(position));
    ui->openGLWidget->setRotateView(position);
}

void MainWindow::on_heightColorDataSetBox_activated(const QString &arg1)
{
    if(arg1 == "density"){
        ui->openGLWidget->setHeightColorScalarDataSet(0);
    }
    if(arg1 == "fluid velocity magnitude"){
        ui->openGLWidget->setHeightColorScalarDataSet(1);
    }
    if(arg1 == "force field magnitude"){
        ui->openGLWidget->setHeightColorScalarDataSet(2);
    }
    if(arg1 == "divergence"){
        ui->openGLWidget->setHeightColorScalarDataSet(3);
    }
}

void MainWindow::on_isolineThicknessSlider_sliderMoved(int position)
{
    ui->isolineThicknessLabel->setText(QStringLiteral("Isoline thickness: %1").arg(position));
    ui->openGLWidget->isolineThickness(position);
}

void MainWindow::on_jitterCheckbox_toggled(bool checked)
{
    ui->openGLWidget->setJitter(checked);
}

void MainWindow::on_glyphColorComboBox_activated(const QString &arg1)
{
    if(arg1 == "none"){
        ui->openGLWidget->setGlyphColor(0);
    }
    if(arg1 == "direction"){
        ui->openGLWidget->setGlyphColor(1);
    }
    if(arg1 == "scalar data set"){
        ui->openGLWidget->setGlyphColor(2);
    }
}
