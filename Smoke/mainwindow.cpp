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

void MainWindow::on_smoke_checkbox_toggled(bool checked)
{
    ui->openGLWidget->toggle_smoke(checked);
}

void MainWindow::on_vecs_checkbox_toggled(bool checked)
{
    ui->openGLWidget->toggle_vecs(checked);
}

void MainWindow::on_colorBox_activated(const QString &arg1)
{
    if(arg1 == "rainbow"){
        ui->openGLWidget->setColor(1);
    }
    if(arg1 == "heatmap"){
        ui->openGLWidget->setColor(2);
    }
    if(arg1 == "grayscale"){
        ui->openGLWidget->setColor(0);
    }
    if(arg1 == "yellow"){
        ui->openGLWidget->setColor(3);
    }
}

void MainWindow::on_bandsSlider_sliderMoved(int position)
{
    ui->openGLWidget->setColorBands(position);
    ui->bandsLabel->setText(QStringLiteral("Number of bands: %1").arg(position));
}
