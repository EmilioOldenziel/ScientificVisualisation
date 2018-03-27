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

    // disable color bands slider and colormaps
    ui->bandsLabel->setEnabled(checked);
    ui->bandsSlider->setEnabled(checked);
    ui->colorBox->setEnabled(checked);
    ui->colorScalingBox->setEnabled(checked);
}

// turn gliphs on/off
void MainWindow::on_vecs_checkbox_toggled(bool checked)
{
    ui->openGLWidget->toggle_vecs(checked);
    ui->vecsColor_checkbox->setEnabled(checked);

    ui->vecScaleLabel->setEnabled(checked);
    ui->vecScaleSlider->setEnabled(checked);

}


void MainWindow::on_divergence_toggled(bool checked)
{
    ui->openGLWidget->toggleDivergence(checked);
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
        ui->openGLWidget->setColor(3);
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
    ui->vecScaleLabel->setText(QStringLiteral("Vector Scaling: %1").arg(pos));
}

// slider for gliph coloring
void MainWindow::on_vecsColor_checkbox_clicked(bool checked)
{
    ui->openGLWidget->toggleColorDirection(checked);
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
