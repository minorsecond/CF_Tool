#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QWidget *parent = nullptr);
    void set_confirmation_message(const std::string message);
    ~ConfirmDialog();

private:
    Ui::ConfirmDialog *ui;

private slots:
    void exitSlot();
};

#endif // CONFIRMDIALOG_H
