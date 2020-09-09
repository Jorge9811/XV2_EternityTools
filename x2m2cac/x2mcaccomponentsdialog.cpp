#include "x2mcaccomponentsdialog.h"
#include "ui_x2mcaccomponentsdialog.h"

#include "debug.h"

X2mCacComponentsDialog::X2mCacComponentsDialog(uint32_t cac_race, uint32_t x2m_race, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::X2mCacComponentsDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QFont boldFont = ui->basicCheck->font();
    boldFont.setBold(true);

    ui->basicCheck->setFont(boldFont);
    ui->compCheck->setFont(boldFont);
    ui->statsCheck->setFont(boldFont);

    ui->statsCheck->setStyleSheet("QCheckBox:unchecked{ color: red; }QCheckBox:checked{ color: red; }");

    if (cac_race == CAC_HUM && x2m_race == CAC_SYM)
    {
        ui->keepRaceCheck->setText("Keep current sav race (HUM)");
    }
    else if (cac_race == CAC_SYM && x2m_race == CAC_HUM)
    {
        ui->keepRaceCheck->setText("Keep current sav race (SYM)");
    }
    else if (cac_race == CAC_HUF && x2m_race == CAC_SYF)
    {
        ui->keepRaceCheck->setText("Keep current sav race (HUF)");
    }
    else if (cac_race == CAC_SYF && x2m_race == CAC_HUF)
    {
        ui->keepRaceCheck->setText("Keep current sav race (SYF)");
    }
    else
    {
        ui->keepRaceCheck->setDisabled(true);
        ui->keepRaceCheck->setVisible(false);
    }
}

X2mCacComponentsDialog::~X2mCacComponentsDialog()
{
    delete ui;
}

bool X2mCacComponentsDialog::useBasic() const
{
    return ui->basicCheck->isChecked();
}

bool X2mCacComponentsDialog::useBodyComponents() const
{
    return ui->compCheck->isChecked();
}

bool X2mCacComponentsDialog::keepRace() const
{
    return (ui->keepRaceCheck->isEnabled() && ui->keepRaceCheck->isChecked());
}

bool X2mCacComponentsDialog::useCostumes() const
{
    return (ui->clothesCheck->isChecked());
}

bool X2mCacComponentsDialog::useSkills() const
{
    return (ui->skillsCheck->isChecked());
}

bool X2mCacComponentsDialog::useStats() const
{
    return (ui->statsCheck->isChecked());
}
