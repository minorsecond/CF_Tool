#include "overrideponhomes.h"
#include "ui_overrideponhomes.h"

OverridePonHomes::OverridePonHomes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverridePonHomes)
{
    ui->setupUi(this);
}

OverridePonHomes::~OverridePonHomes()
{
    delete ui;
}
