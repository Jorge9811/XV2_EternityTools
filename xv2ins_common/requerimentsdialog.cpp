#include "requerimentsdialog.h"
#include "ui_requerimentsdialog.h"

#include <QFileDialog>

RequerimentsDialog::RequerimentsDialog(const QString &game_exe, QWidget *parent) :
    game_exe(game_exe), QDialog(parent),
    ui(new Ui::RequerimentsDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->gameEdit->setText(game_exe);   
}

RequerimentsDialog::~RequerimentsDialog()
{
    delete ui;
}

void RequerimentsDialog::on_gameButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select DBXV2.exe", "", "DBXV2.exe (DBXV2.exe)");

    if (file.isNull())
        return;

    ui->gameEdit->setText(file);
}

void RequerimentsDialog::on_buttonBox_accepted()
{
    game_exe = ui->gameEdit->text();    
}
