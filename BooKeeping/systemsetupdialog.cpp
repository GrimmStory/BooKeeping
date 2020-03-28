#include "systemsetupdialog.h"
#include "ui_systemsetupdialog.h"

systemSetupDialog::systemSetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::systemSetupDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
}

systemSetupDialog::~systemSetupDialog()
{
    delete ui;
}
