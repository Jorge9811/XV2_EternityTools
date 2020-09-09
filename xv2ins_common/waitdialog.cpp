#include "waitdialog.h"
#include "ui_waitdialog.h"

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~(Qt::WindowContextHelpButtonHint|Qt::WindowCloseButtonHint));
}

WaitDialog::~WaitDialog()
{
    delete ui;
}
