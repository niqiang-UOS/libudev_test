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


void MainWindow::on_pushButton_clicked()
{
    UDevEnvironment environ;
    environ.EnableDev("/sys/devices/pci0000:00/0000:00:08.1/0000:03:00.3/usb1/1-3");
}


void MainWindow::on_pushButton_2_clicked()
{
    UDevEnvironment environ;
    environ.DisableDev("/sys/devices/pci0000:00/0000:00:08.1/0000:03:00.3/usb1/1-3");
}


void MainWindow::on_pushButton_3_clicked()
{
    UDevEnvironment environ;
    environ.RemoveDev("/sys/devices/pci0000:00/0000:00:08.1/0000:03:00.3/usb1/1-3");
}

