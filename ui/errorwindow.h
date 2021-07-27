#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QDialog>

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = nullptr);
    ~ErrorWindow();
    void set_error_message(const std::string message);

private:
    Ui::ErrorWindow *ui;

private slots:
    void exitSlot();
};

#endif // ERRORWINDOW_H
