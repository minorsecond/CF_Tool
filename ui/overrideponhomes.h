#ifndef OVERRIDEPONHOMES_H
#define OVERRIDEPONHOMES_H

#include <QDialog>

namespace Ui {
class OverridePonHomes;
}

class OverridePonHomes : public QDialog
{
    Q_OBJECT

public:
    explicit OverridePonHomes(QWidget *parent = nullptr);
    ~OverridePonHomes();

private:
    Ui::OverridePonHomes *ui;
};

#endif // OVERRIDEPONHOMES_H
