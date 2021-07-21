#include "errorwindow.h"
#include "ui_errorwindow.h"

ErrorWindow::ErrorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    // Close slot
    QObject::connect(ui->OKButton, SIGNAL(clicked()), this, SLOT(exitSlot()));
}

void ErrorWindow::set_error_message(const std::string message) {
    /*
     * Sets the error message to be displayed
     * @param message: The message to be displayed
     */

    ui->ErrorMessage->setText(QString::fromStdString(message));
}

void ErrorWindow::exitSlot() {
    /*
     * Close the window when OK button clicked
     */

    ErrorWindow::close();
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}
