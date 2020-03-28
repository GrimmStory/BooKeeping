#ifndef SYSTEMSETUPDIALOG_H
#define SYSTEMSETUPDIALOG_H

#include <QDialog>

namespace Ui {
class systemSetupDialog;
}

class systemSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit systemSetupDialog(QWidget *parent = 0);
    ~systemSetupDialog();

private:
    Ui::systemSetupDialog *ui;
};

#endif // SYSTEMSETUPDIALOG_H
