#include "exportcacdialog.h"
#include "ui_exportcacdialog.h"

#include "debug.h"

ExportCacDialog::ExportCacDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportCacDialog)
{
    ui->setupUi(this);

    QFont boldFont = ui->infoCheck->font();
    boldFont.setBold(true);

    ui->infoCheck->setFont(boldFont);
    ui->looksCheck->setFont(boldFont);
    ui->statsCheck->setFont(boldFont);
    ui->statsCheck->setStyleSheet("QCheckBox:unchecked{ color: red; }QCheckBox:checked{ color: red; }");
}

ExportCacDialog::~ExportCacDialog()
{
    delete ui;
}

bool ExportCacDialog::DoInfo()
{
    return ui->infoCheck->isChecked();
}

bool ExportCacDialog::DoLooks()
{
    return ui->looksCheck->isChecked();
}

bool ExportCacDialog::DoEquip()
{
    return ui->equipmentCheck->isChecked();
}

bool ExportCacDialog::DoStats()
{
    return ui->statsCheck->isChecked();
}

bool ExportCacDialog::DoSkills()
{
    return ui->skillsCheck->isChecked();
}

void ExportCacDialog::on_buttonBox_accepted()
{
    if (!ui->infoCheck->isChecked() && !ui->looksCheck->isChecked() && !ui->equipmentCheck->isChecked() &&
        !ui->statsCheck->isChecked() && !ui->skillsCheck->isChecked())
    {
        DPRINTF("You haven't selected anything!\n");
        return;
    }

    accept();
}
