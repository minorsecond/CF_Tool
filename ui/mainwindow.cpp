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

    ui->NewJobButton->setDisabled(true);

    // Create working directories on launch
    UtilityFunctions ut;
    ut.create_directories();

    // Populate the state selection combo boxes
    const std::array<QString, 50> states {"Alabama", "Alaska", "Arizona", "Arkansas", "California",
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
    for (std::map<int, int>::const_iterator iter {utm_zones.begin()}; iter != utm_zones.end(); iter++) {
        ui->utmZoneBox->addItem(QString::fromStdString(std::to_string(iter->first)));
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

    const Job jobinfo (ui->JobIDInput->text().toStdString(), ui->CityInput->text().toStdString(), ui->StateInput->currentText().toStdString());

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

    if (!jobinfo.get_job_id().empty() && !jobinfo.get_city().empty() && !jobinfo.get_state().empty()) {
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

                reproject_layers(jobinfo, reproj_path, utm_zone);
                rename_address_files(reproj_path);
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

        ui->NewJobButton->setEnabled(true);
    } else {
        const std::string error_message {"Error: Not all required fields populated."};
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

    const Job jobinfo (ui->JobIDInput->text().toStdString());

    if (!jobinfo.get_job_id().empty()) {

        std::string completed_message {"Attributes created"};

        // Handle processing of demand points
        const std::string gis_path {jobinfo.find_gis_path()};

        if (gis_path == "FILENOTFOUND") {
            er.set_error_message("Error: could not find directory for job # " + jobinfo.get_job_id());
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
        for (const std::string &path : input_files) {
            if (!ut.file_exists(path)) {
                er.set_error_message("Warning: could not find " + path);
                er.exec();
                input_files.at(vector_counter) = "";
            }
            vector_counter++;
        }


        if (input_files[0].size() > 0) {  // Demand points. Skip if the path doesn't exist (it has been set to "" in previous loop)
            //OGRLayer *demand_points {ShapeEditor::shapefile_reader(demand_points_path)};
            std::unique_ptr<OGRLayer> demand_points{ShapeEditor::shapefile_reader(demand_points_path)};
            ShapeEditor::process_demand_points(demand_points.get());
            demand_points->SyncToDisk();
            //delete demand_points;  // delete pointer
        }

        if (input_files[1].size() > 0) {  // Access points
            //OGRLayer *access_points {ShapeEditor::shapefile_reader(access_points_path)};
            std::unique_ptr<OGRLayer> access_points{ShapeEditor::shapefile_reader(access_points_path)};

            ShapeEditor::process_access_points(access_points.get());
            access_points->SyncToDisk();
            //delete access_points;
        }

        if (input_files[2].size() > 0) {  // Poles
            //OGRLayer *poles {ShapeEditor::shapefile_reader(poles_path)};
            std::unique_ptr<OGRLayer> poles{ShapeEditor::shapefile_reader(poles_path)};

            ShapeEditor::process_poles(poles.get());
            poles->SyncToDisk();
            //delete poles;
        }

        if (input_files[3].size() > 0) { // Aerials
            //OGRLayer *aerials {ShapeEditor::shapefile_reader(aerials_path)};
            std::unique_ptr<OGRLayer> aerials{ShapeEditor::shapefile_reader(aerials_path)};

            ShapeEditor::process_aerial_connections(aerials.get());
            aerials->SyncToDisk();
            //delete aerials;
        }

        if (input_files[4].size() > 0) { // FDT Boundaries
            //OGRLayer *fdt_boundary {ShapeEditor::shapefile_reader(fdt_path)};
            std::unique_ptr<OGRLayer> fdt_boundary{ShapeEditor::shapefile_reader(fdt_path)};

            ShapeEditor::process_fdt_boundaries(fdt_boundary.get());
            fdt_boundary->SyncToDisk();
            //delete fdt_boundary;
        }

        confirm.set_confirmation_message(completed_message);
        confirm.exec();
        ui->CA_Done->setText(QString::fromStdString("Done"));
        ui->NewJobButton->setEnabled(true);
    } else {
        er.set_error_message("Error: Job ID not entered.");
        er.exec();
        return;
    }
}

void MainWindow::handle_da_process_button() {
    /*
     * Handles actions when user presses the process button on the third tab (Deliverable archiving)
     */
    UtilityFunctions ut;
    ErrorWindow er;
    ConfirmDialog confirm;

    const Job jobinfo (ui->JobIDInput->text().toStdString(), ui->CityInput->text().toStdString(), ui->StateInput->currentText().toStdString());

    if (!jobinfo.get_job_id().empty() && !jobinfo.get_city().empty() && !jobinfo.get_state().empty()) {
        if (!std::filesystem::exists(jobinfo.get_workspace_path())) {
            er.set_error_message("Warning: could not find workspace path for job # " + jobinfo.get_job_id());
            er.exec();
            return;
        }

        ut.zip_files(jobinfo);
        confirm.set_confirmation_message("Created deliverable archive");
        confirm.exec();
        ui->DA_Done->setText(QString::fromStdString("Done"));
        ui->NewJobButton->setEnabled(true);
    } else {
        er.set_error_message("Error: Not all required fields populated.");
        er.exec();
        return;
    }
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
    ui->NewJobButton->setDisabled(true);
}

void MainWindow::reproject_layers(const Job &jobinfo, const std::string &reproj_path, const std::string &utm_zone) {
    /*
     * Reproject all layers for job
     */

    UtilityFunctions ut;
    ShapeEditor shp;

    for (const auto &file : std::filesystem::directory_iterator(jobinfo.find_gis_path())) {
        const size_t lastindex {file.path().string().find_last_of(".") + 1};
        const std::string extension {file.path().string().substr(lastindex)}; // Get extension
        if (extension == "shp") {
            const std::string filename {file.path().filename().string()};
            const std::string out_path {reproj_path + "\\" + filename};
            if (!ut.file_exists(out_path)) {
                std::cout << "Reprojecting " << file.path().string() << std::endl;
                //OGRLayer *in_layer {ShapeEditor::shapefile_reader(file.path().string())};
                std::unique_ptr<OGRLayer> in_layer{ShapeEditor::shapefile_reader(file.path().string())};
                shp.reproject(in_layer.get(), std::stoi(utm_zone), out_path);
                //delete in_layer;
            }
        }
    }

}

void MainWindow::rename_address_files(const std::string &reproj_path) {
    /*
     * Rename address files
     */

    UtilityFunctions ut;
    ErrorWindow er;

    bool address_rename_error {false};
    for (const auto & file : std::filesystem::directory_iterator(reproj_path)) {
        const std::string filename {file.path().filename().string()};

        // Rename all files with address in the name to addresses.shp (including correct extensions)
        if (ut.search_string_for_substring(filename, "address")) {  //TODO: Be more specific as CGIS might return other files with address in name
            const size_t lastindex {file.path().string().find_last_of(".") + 1};
            const std::string extension {file.path().string().substr(lastindex)}; // Get extension
            std::cout << "Path: " << file.path().string() << std::endl;
            try {
                std::filesystem::rename(file.path().string(), reproj_path + "\\addresses." + extension);
            }  catch (std::filesystem::filesystem_error) {
                // Can't rename address file. User might have file open in GIS software
                address_rename_error = true;
            }
        }
    }

    if (address_rename_error) {
        // This is done to only display the address rename error once, since the renamer loop is run once for each
        // of the four shapefile files. Otherwise, the message would display four times.
        er.set_error_message("Error: Could not rename address shapefile. Close all relevant layers in GIS software and rerun.");
        er.exec();
        return;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
