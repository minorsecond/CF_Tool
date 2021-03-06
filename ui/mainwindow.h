#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../src/job.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void reproject_layers(const Job &jobinfo, const std::string &reproj_path, const std::string &utm_zone);
    void rename_address_files(const std::string &reproj_path);

private slots:
    void handle_cw_process_button();
    void handle_ac_process_button();
    void handle_da_process_button();
    void new_job_button();
};
#endif // MAINWINDOW_H
