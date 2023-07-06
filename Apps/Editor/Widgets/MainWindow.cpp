#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QRibbon.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QRibbon::install(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
