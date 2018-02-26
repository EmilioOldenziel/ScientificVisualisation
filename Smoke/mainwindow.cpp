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
}

// turn gliphs on/off
void MainWindow::on_vecs_checkbox_toggled(bool checked)
{
    ui->openGLWidget->toggle_vecs(checked);
    ui->vecsColor_checkbox->setEnabled(checked);

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
    if(arg1 == "yellow"){
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
    ui->openGLWidget->setVecScale(position);
    ui->vecScaleLabel->setText(QStringLiteral("Vector scaling: %1").arg(position));
}

// slider for gliph coloring
void MainWindow::on_vecsColor_checkbox_clicked(bool checked)
{
    ui->openGLWidget->toggleColorDirection(checked);
}
