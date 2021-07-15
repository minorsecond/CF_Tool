#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility_functions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create working directories on launch
    UtilityFunctions ut;
    ut.create_directories();


}

MainWindow::~MainWindow()
{
    delete ui;
}

