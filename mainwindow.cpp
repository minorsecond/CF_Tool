/*
 * Application for assisting Comsof work.
 * This tool creates workspace directories (Desktop/Workpaces & Documents/State/City/JobID),
 * Manipulates shapefile attribute tables to prepare them for Comsof, and
 * archives output data to prepare it for submittal to CGIS.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility_functions.h"
#include "errorwindow.h"

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
    ErrorWindow er;
    const std::string job_id = ui->WC_JobIDInput->text().toStdString();
    const std::string city = ui->WC_CityInput->text().toStdString();
    const std::string state {ui->WC_StateInput->currentText().toStdString()};

    const std::string home_path {ut.get_home_path()};
    std::cout << "Job ID: " << job_id << " City: " << city << " State: " << state << std::endl;

    std::string zip_path {ut.find_zip_file(job_id)};

    std::cout << zip_path << std::endl;

    if (!job_id.empty() && !city.empty() && !state.empty()) {
        // Extract files to C:\Users\USERNAME\Downloads\_tmp
        if (zip_path != "FILENOTFOUND") {
            ut.build_working_dirs(job_id, city, state);
            ut.unzip_file(zip_path);
        } else {
            const std::string error_message {"Couldn't find zip file in downloads directory."};
            std::cout << error_message << std::endl;
            er.set_error_message(error_message);
            er.exec();
        }

        ut.move_extracted_files(job_id, city, state);  // Move files to working directory
    } else {
        const std::string error_message {"Not all required fields populated"};
        std::cout << error_message << std::endl;
        er.set_error_message(error_message);
        er.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

