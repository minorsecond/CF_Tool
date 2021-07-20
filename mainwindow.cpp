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
#include "shapeeditor.h"

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
    connect(ui->AC_ProcessButton, &QPushButton::clicked, this, &MainWindow::handle_ac_process_button);
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

void MainWindow::handle_ac_process_button() {
    /*
     * Handles actions when user presses the process button on the second tab (Attribute Creation)
     */

    UtilityFunctions ut;
    ErrorWindow er;

    // Handle processing of demand points
    const std::string job_number {ui->AC_JobIDEntry->text().toStdString()};
    const std::string gis_path {ut.find_gis_path(job_number)};
    const std::string reproj_path {gis_path + "\\reprojected"};
    const std::string demand_points_path {reproj_path + "\\addresses.dbf"};
    const std::string access_points_path {reproj_path + "\\access_point.dbf"};
    const std::string poles_path {reproj_path + "\\poles.dbf"};
    const std::string aerials_path {reproj_path + "\\span_length.dbf"};
    const std::string fdt_path {reproj_path + "\\fdt_boundary.dbf"};

    std::vector<std::string> input_files {demand_points_path, access_points_path,
                                         poles_path, aerials_path, fdt_path};
    for (std::string path : input_files) {
        if (!ut.file_exists(path)) {
            er.set_error_message("Warning: could not find " + path);
            path = "";
        }
    }


    if (input_files[0].size() > 0) {  // Demand points. Skip if the path doesn't exist (it has been set to "" in previous loop)
        OGRLayer *demand_points {ShapeEditor::shapefile_reader(demand_points_path)};
        ShapeEditor::create_demand_point_fields(demand_points);
        ShapeEditor::process_demand_points("include", demand_points);  // Populate INCLUDE, PON_HOMES, and STREETNAME
    }
    /*
    if (input_files[1].size() > 0) {  // Access points
        OGRLayer *access_points {ShapeEditor::shapefile_reader(access_points_path)};
        ShapeEditor::process_access_points(access_points);
    }

    if (input_files[2].size() > 0) {  // Poles
        OGRLayer *poles {ShapeEditor::shapefile_reader(poles_path)};
        ShapeEditor::process_poles(poles);
    }

    if (input_files[3].size() > 0) { // Aerials
        OGRLayer *aerials {ShapeEditor::shapefile_reader(aerials_path)};
        ShapeEditor::process_aerial_connections(aerials);
    }

    if (input_files[4].size() > 0) { // FDT Boundaries
        OGRLayer *fdt_boundary {ShapeEditor::shapefile_reader(fdt_path)};
        ShapeEditor::process_fdt_boundaries(fdt_boundary);
    }
    */
}


MainWindow::~MainWindow()
{
    delete ui;
}

