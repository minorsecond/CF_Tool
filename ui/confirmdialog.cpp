#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);

    // Slots
    QObject::connect(ui->OKButton, SIGNAL(clicked()), this, SLOT(exitSlot()));
}

void ConfirmDialog::set_confirmation_message(const std::string message) {
    /*
     * Sets the confirmation message to be displayed.
     * @param message: Message to be shown.
     */

    ui->confirmationMessage->setText(QString::fromStdString(message));
}

void ConfirmDialog::exitSlot() {
    /*
     * Close the window on OK button click
     */

    ConfirmDialog::close();
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}
