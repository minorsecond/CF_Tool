/*
 * Application for assisting Comsof work.
 * This tool creates workspace directories (Desktop/Workpaces & Documents/State/City/JobID),
 * Manipulates shapefile attribute tables to prepare them for Comsof, and
 * archives output data to prepare it for submittal to CGIS.
 */

#include "ui/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/utility_functions.h"
#include "ui/errorwindow.h"
#include "ui/confirmdialog.h"
#include "src/shapeeditor.h"

#include <iostream>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

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
        ui->StateInput->addItem(state);
    }

    // Populate the UTM zone combobox
    const int zones[10] {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    for (int zone : zones) {
        ui->utmZoneBox->addItem(QString::fromStdString(std::to_string(zone)));
    }

    // Slots
    connect(ui->WC_ProcuessButton, &QPushButton::clicked, this, &MainWindow::handle_cw_process_button);
    connect(ui->AC_ProcessButton, &QPushButton::clicked, this, &MainWindow::handle_ac_process_button);
    connect(ui->DA_ProcessButton, &QPushButton::clicked, this, &MainWindow::handle_da_process_button);
    connect(ui->NewJobButton, &QPushButton::clicked, this, &MainWindow::new_job_button);
}

void MainWindow::handle_cw_process_button() {
    /*
     * Handles actions when user presses the process button on the first tab (Create workspace).
     */

    UtilityFunctions ut;
    ShapeEditor shp;
    ErrorWindow er;
    ConfirmDialog confirm;

    Job jobinfo (ui->JobIDInput->text().toStdString(), ui->CityInput->text().toStdString(), ui->StateInput->currentText().toStdString());

    const std::string home_path {ut.get_home_path()};
    const std::string workspace_path {jobinfo.new_workspace_path()};
    const std::wstring workspace_path_ws {std::wstring(workspace_path.begin(), workspace_path.end())};

    const std::string utm_zone {ui->utmZoneBox->currentText().toStdString()};

    if (home_path == "PATHNOTFOUND") {  // Something bad happened
        er.set_error_message("Couldn't detect your home path. Contact developer.");
        er.exec();
        return;
    }

    std::string zip_path {ut.find_zip_file(jobinfo)};

    if (!jobinfo.job_id.empty() && !jobinfo.city.empty() && !jobinfo.state.empty()) {
        // Extract files to C:\Users\USERNAME\Downloads\_tmp
        if (zip_path != "FILENOTFOUND") {
            ut.build_working_dirs(jobinfo);
            ut.unzip_file(zip_path);

            ut.move_extracted_files(jobinfo);  // Move files to working directory
            ut.create_directory_recursively(workspace_path_ws);

            if (utm_zone != "Manual Reprojection") {
                // Copy files to reprojected directory
                const std::string reproj_path {jobinfo.find_gis_path() + "\\reprojected"};
                //ut.copy_files_in_dir(jobinfo.find_gis_path(), reproj_path);

                // Reproject layers
                for (const auto &file : std::filesystem::directory_iterator(jobinfo.find_gis_path())) {
                    const size_t lastindex {file.path().string().find_last_of(".") + 1};
                    const std::string extension {file.path().string().substr(lastindex)}; // Get extension
                    if (extension == "shp") {
                        const std::string filename {file.path().filename().string()};
                        const std::string out_path {reproj_path + "\\" + filename};
                        if (!ut.file_exists(out_path)) {
                            std::cout << "Reprojecting " << file.path().string() << std::endl;
                            OGRLayer *in_layer {ShapeEditor::shapefile_reader(file.path().string())};
                            shp.reproject(in_layer, std::stoi(utm_zone), out_path);
                            delete in_layer;
                        }
                    }
                }

                // Rename address files
                for (const auto & file : std::filesystem::directory_iterator(reproj_path)) {
                    const std::string filename {file.path().filename().string()};

                    // Rename all files with address in the name to addresses.shp (including correct extensions)
                    if (ut.search_string_for_substring(filename, "address")) {
                        const size_t lastindex {file.path().string().find_last_of(".") + 1};
                        const std::string extension {file.path().string().substr(lastindex)}; // Get extension
                        std::cout << "Extension: " << extension << std::endl;
                        std::filesystem::rename(file.path().string(), reproj_path + "\\addresses." + extension);
                    }
                }
            }
            confirm.set_confirmation_message("Workspaces created.");
            confirm.exec();
            ui->CW_Done->setText(QString::fromStdString("Done"));
        } else {
            const std::string error_message {"Couldn't find zip file in downloads directory."};
            std::cout << error_message << std::endl;
            er.set_error_message(error_message);
            er.exec();
        }
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
    ConfirmDialog confirm;
    Job jobinfo (ui->JobIDInput->text().toStdString());

    std::string completed_message {"Attributes created"};

    // Handle processing of demand points
    const std::string gis_path {jobinfo.find_gis_path()};

    if (gis_path == "FILENOTFOUND") {
        er.set_error_message("Error: could not find directory for job # " + jobinfo.job_id);
        er.exec();
        return;
    }

    const std::string reproj_path {gis_path + "\\reprojected"};
    const std::string demand_points_path {reproj_path + "\\addresses.dbf"};
    const std::string access_points_path {reproj_path + "\\access_point.dbf"};
    const std::string poles_path {reproj_path + "\\pole.dbf"};
    const std::string aerials_path {reproj_path + "\\span_length.dbf"};
    const std::string fdt_path {reproj_path + "\\fdt_boundary.dbf"};

    std::vector<std::string> input_files {demand_points_path, access_points_path,
                                         poles_path, aerials_path, fdt_path};

    // Check if input file exists. If it doesn't, set the path string to "". This will later be checked to
    // ensure that GDAL doesn't attempt to load a nonexisting file.
    size_t vector_counter {0};
    for (std::string path : input_files) {
        if (!ut.file_exists(path)) {
            er.set_error_message("Warning: could not find " + path);
            er.exec();
            input_files.at(vector_counter) = "";
        }
        vector_counter++;
    }


    if (input_files[0].size() > 0) {  // Demand points. Skip if the path doesn't exist (it has been set to "" in previous loop)
        OGRLayer *demand_points {ShapeEditor::shapefile_reader(demand_points_path)};

        if (ShapeEditor::find_field_index("INCLUDE", demand_points) == -1) {
            ShapeEditor::create_demand_point_fields(demand_points);
            ShapeEditor::process_demand_points("include", demand_points);  // Populate INCLUDE, PON_HOMES, and STREETNAME
            demand_points->SyncToDisk();
        } else {
            er.set_error_message("INCLUDE field already exists in addresses shapefile."
                " Recreate shapefile in reprojected directory and rerun to process.");
            er.exec();
            completed_message = "Attributes created. Skipped addresses.";
        }

        delete demand_points;  // delete pointer
    }

    if (input_files[1].size() > 0) {  // Access points
        OGRLayer *access_points {ShapeEditor::shapefile_reader(access_points_path)};
        ShapeEditor::process_access_points(access_points);
        access_points->SyncToDisk();
        delete access_points;
    }

    if (input_files[2].size() > 0) {  // Poles
        OGRLayer *poles {ShapeEditor::shapefile_reader(poles_path)};
        ShapeEditor::process_poles(poles);
        poles->SyncToDisk();
        delete poles;
    }

    if (input_files[3].size() > 0) { // Aerials
        OGRLayer *aerials {ShapeEditor::shapefile_reader(aerials_path)};
        ShapeEditor::process_aerial_connections(aerials);
        aerials->SyncToDisk();
        delete aerials;
    }

    if (input_files[4].size() > 0) { // FDT Boundaries
        OGRLayer *fdt_boundary {ShapeEditor::shapefile_reader(fdt_path)};
        ShapeEditor::process_fdt_boundaries(fdt_boundary);
        fdt_boundary->SyncToDisk();
        delete fdt_boundary;
    }

    confirm.set_confirmation_message(completed_message);
    confirm.exec();
    ui->CA_Done->setText(QString::fromStdString("Done"));
}

void MainWindow::handle_da_process_button() {
    /*
     * Handles actions when user presses the process button on the third tab (Deliverable archiving)
     */
    UtilityFunctions ut;
    ErrorWindow er;
    ConfirmDialog confirm;
    Job jobinfo (ui->JobIDInput->text().toStdString(), ui->CityInput->text().toStdString(), ui->StateInput->currentText().toStdString());

    if (!std::filesystem::exists(jobinfo.get_workspace_path())) {
        er.set_error_message("Warning: could not find workspace path for job # " + jobinfo.job_id);
        er.exec();
        return;
    }

    ut.zip_files(jobinfo);
    confirm.set_confirmation_message("Created deliverable archive");
    confirm.exec();
    ui->DA_Done->setText(QString::fromStdString("Done"));
}

void MainWindow::new_job_button() {
    /*
     * Reset inputs for new job
     */

    ui->JobIDInput->clear();
    ui->CityInput->clear();
    ui->StateInput->setCurrentIndex(0);
    ui->utmZoneBox->setCurrentIndex(0);
    ui->CW_Done->clear();
    ui->CA_Done->clear();
    ui->DA_Done->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}
