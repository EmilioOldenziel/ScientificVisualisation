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
