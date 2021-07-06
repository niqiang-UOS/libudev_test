#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udevmonitor.h"
#include "udevenvironment.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UDevMonitor *devMonitor = new UDevMonitor(this);
    devMonitor->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_m_AllDev_Btn_clicked()
{
    UDevEnvironment environ;
    environ.getAllDevice();
}


void MainWindow::on_m_Queue_Btn_clicked()
{
    UDevEnvironment environ;
    environ.getAllQueue();
}

