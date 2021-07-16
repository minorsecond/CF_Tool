#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility_functions.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create working directories on launch
    UtilityFunctions ut;
    ut.create_directories();

    // Populate the state selection combo boxes
    const std::vector<QString> states {"Alabama", "Alaska", "Arizona", "Arkansas", "California",
                                           "Colorado", "Connecticut", "Delaware", "Florida", "Georgia",
                                          "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas",
                                          "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts",
                                          "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana",
                                          "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico",
                                          "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma",
                                          "Oregon", "Pennsylvania", "Rhode Island", "South Carolina",
                                          "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virgina",
                                          "Washington", "West Virginia", "Wisconsin", "Wyoming"};
    for (const QString &state : states) {
        ui->WC_StateInput->addItem(state);
        ui->DA_StateInput->addItem(state);
    }

    // Slots
    connect(ui->WC_ProcuessButton, &QPushButton::clicked, this, &MainWindow::handle_cw_process_button);
}

void MainWindow::handle_cw_process_button() {
    /*
     * Handles actions when user presses the process button on the first tab (Create workspace).
     */

    UtilityFunctions ut;
    const std::string job_id = ui->WC_JobIDInput->text().toStdString();
    const std::string city = ui->WC_CityInput->text().toStdString();
    const std::string state {ui->WC_StateInput->currentText().toStdString()};

    const std::string home_path {ut.get_home_path()};

    std::string zip_path {ut.find_zip_file(job_id)};

    std::cout << zip_path << std::endl;
    // Extract files to C:\Users\USERNAME\Downloads\_tmp
    if (zip_path != "FILENOTFOUND") {
        ut.unzip_file(zip_path, job_id);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

