#include "idbeffectdialog.h"
#include "ui_idbeffectdialog.h"

IdbEffectDialog::IdbEffectDialog(QWidget *parent, const IdbEffect &effect) :
    QDialog(parent), effect(effect),
    ui(new Ui::IdbEffectDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->typeEdit->setValidator(new QIntValidator(this));
    ui->actTypeEdit->setValidator(new QIntValidator(this));
    ui->numActEdit->setValidator(new QIntValidator(this));

    ui->timerEdit->setValidator(new QDoubleValidator(this));
    ui->ab1Edit->setValidator(new QDoubleValidator(this));
    ui->ab2Edit->setValidator(new QDoubleValidator(this));
    ui->ab3Edit->setValidator(new QDoubleValidator(this));
    ui->ab4Edit->setValidator(new QDoubleValidator(this));
    ui->ab5Edit->setValidator(new QDoubleValidator(this));
    ui->ab6Edit->setValidator(new QDoubleValidator(this));

    ui->u28Edit->setValidator(new QIntValidator(this));
    ui->chanceEdit->setValidator(new QIntValidator(this));

    ui->mul1Edit->setValidator(new QDoubleValidator(this));
    ui->mul2Edit->setValidator(new QDoubleValidator(this));
    ui->mul3Edit->setValidator(new QDoubleValidator(this));
    ui->mul4Edit->setValidator(new QDoubleValidator(this));
    ui->mul5Edit->setValidator(new QDoubleValidator(this));
    ui->mul6Edit->setValidator(new QDoubleValidator(this));

    ui->u48Edit->setValidator(new QIntValidator(this));
    ui->u4CEdit->setValidator(new QIntValidator(this));
    ui->u50Edit->setValidator(new QIntValidator(this));
    ui->u54Edit->setValidator(new QIntValidator(this));
    ui->u58Edit->setValidator(new QIntValidator(this));
    ui->u5CEdit->setValidator(new QIntValidator(this));

    ui->heaEdit->setValidator(new QDoubleValidator(this));
    ui->kiEdit->setValidator(new QDoubleValidator(this));
    ui->kiRecEdit->setValidator(new QDoubleValidator(this));
    ui->stmEdit->setValidator(new QDoubleValidator(this));
    ui->stmRecEdit->setValidator(new QDoubleValidator(this));
    ui->enStaErEdit->setValidator(new QDoubleValidator(this));
    ui->f78Edit->setValidator(new QDoubleValidator(this));

    ui->groundSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->airSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->boostSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->dashSpeedEdit->setValidator(new QDoubleValidator(this));

    ui->atkEdit->setValidator(new QDoubleValidator(this));
    ui->basicKiAtkEdit->setValidator(new QDoubleValidator(this));
    ui->strEdit->setValidator(new QDoubleValidator(this));
    ui->blaEdit->setValidator(new QDoubleValidator(this));
    ui->atkDmgEdit->setValidator(new QDoubleValidator(this));
    ui->kiDmgEdit->setValidator(new QDoubleValidator(this));
    ui->strDmgEdit->setValidator(new QDoubleValidator(this));
    ui->blaDmgEdit->setValidator(new QDoubleValidator(this));

    ui->fACEdit->setValidator(new QDoubleValidator(this));
    ui->fB0Edit->setValidator(new QDoubleValidator(this));
    ui->fB4Edit->setValidator(new QDoubleValidator(this));
    ui->fB8Edit->setValidator(new QDoubleValidator(this));
    ui->fBCEdit->setValidator(new QDoubleValidator(this));
    ui->fC0Edit->setValidator(new QDoubleValidator(this));
    ui->fC4Edit->setValidator(new QDoubleValidator(this));

    ui->fC8Edit->setValidator(new QDoubleValidator(this));
    ui->fCCEdit->setValidator(new QDoubleValidator(this));
    ui->fD0Edit->setValidator(new QDoubleValidator(this));
    ui->fD4Edit->setValidator(new QDoubleValidator(this));
    ui->fD8Edit->setValidator(new QDoubleValidator(this));
    ui->fDCEdit->setValidator(new QDoubleValidator(this));

    EffectToGui();
}

IdbEffectDialog::~IdbEffectDialog()
{
    delete ui;
}

bool IdbEffectDialog::Validate()
{
    QList<QLineEdit *> edits = this->findChildren<QLineEdit *>();

    for (QLineEdit *edit : edits)
    {
        if (edit->text().isEmpty())
        {
            DPRINTF("No value can be empty!. Fill all text inputs.\n");
            return false;
        }
    }

    return true;
}

void IdbEffectDialog::EffectToGui()
{
    ui->typeEdit->setText(QString("%1").arg((int32_t)effect.type));
    ui->actTypeEdit->setText(QString("%1").arg((int32_t)effect.activation_type));
    ui->numActEdit->setText(QString("%1").arg((int32_t)effect.num_act_times));

    ui->timerEdit->setText(QString("%1").arg(effect.timer));
    ui->ab1Edit->setText(QString("%1").arg(effect.ability_values[0]));
    ui->ab2Edit->setText(QString("%1").arg(effect.ability_values[1]));
    ui->ab3Edit->setText(QString("%1").arg(effect.ability_values[2]));
    ui->ab4Edit->setText(QString("%1").arg(effect.ability_values[3]));
    ui->ab5Edit->setText(QString("%1").arg(effect.ability_values[4]));
    ui->ab6Edit->setText(QString("%1").arg(effect.ability_values[5]));

    ui->u28Edit->setText(QString("%1").arg((int32_t)effect.unk_28));
    ui->chanceEdit->setText(QString("%1").arg((int32_t)effect.activation_chance));

    ui->mul1Edit->setText(QString("%1").arg(effect.multipliers[0]));
    ui->mul2Edit->setText(QString("%1").arg(effect.multipliers[1]));
    ui->mul3Edit->setText(QString("%1").arg(effect.multipliers[2]));
    ui->mul4Edit->setText(QString("%1").arg(effect.multipliers[3]));
    ui->mul5Edit->setText(QString("%1").arg(effect.multipliers[4]));
    ui->mul6Edit->setText(QString("%1").arg(effect.multipliers[5]));

    ui->u48Edit->setText(QString("%1").arg((int32_t)effect.unk_48[0]));
    ui->u4CEdit->setText(QString("%1").arg((int32_t)effect.unk_48[1]));
    ui->u50Edit->setText(QString("%1").arg((int32_t)effect.unk_48[2]));
    ui->u54Edit->setText(QString("%1").arg((int32_t)effect.unk_48[3]));
    ui->u58Edit->setText(QString("%1").arg((int32_t)effect.unk_48[4]));
    ui->u5CEdit->setText(QString("%1").arg((int32_t)effect.unk_48[5]));

    ui->heaEdit->setText(QString("%1").arg(effect.hea));
    ui->kiEdit->setText(QString("%1").arg(effect.ki));
    ui->kiRecEdit->setText(QString("%1").arg(effect.ki_recovery));
    ui->stmEdit->setText(QString("%1").arg(effect.stm));
    ui->stmRecEdit->setText(QString("%1").arg(effect.stamina_recovery));
    ui->enStaErEdit->setText(QString("%1").arg(effect.enemy_stamina_eraser));
    ui->f78Edit->setText(QString("%1").arg(effect.unk_78));

    ui->groundSpeedEdit->setText(QString("%1").arg(effect.ground_speed));
    ui->airSpeedEdit->setText(QString("%1").arg(effect.air_speed));
    ui->boostSpeedEdit->setText(QString("%1").arg(effect.boosting_speed));
    ui->dashSpeedEdit->setText(QString("%1").arg(effect.dash_speed));

    ui->atkEdit->setText(QString("%1").arg(effect.atk));
    ui->basicKiAtkEdit->setText(QString("%1").arg(effect.basic_ki_attack));
    ui->strEdit->setText(QString("%1").arg(effect.str));
    ui->blaEdit->setText(QString("%1").arg(effect.bla));
    ui->atkDmgEdit->setText(QString("%1").arg(effect.atk_damage));
    ui->kiDmgEdit->setText(QString("%1").arg(effect.ki_damage));
    ui->strDmgEdit->setText(QString("%1").arg(effect.str_damage));
    ui->blaDmgEdit->setText(QString("%1").arg(effect.bla_damage));

    ui->fACEdit->setText(QString("%1").arg(effect.unk_AC[0]));
    ui->fB0Edit->setText(QString("%1").arg(effect.unk_AC[1]));
    ui->fB4Edit->setText(QString("%1").arg(effect.unk_AC[2]));
    ui->fB8Edit->setText(QString("%1").arg(effect.unk_AC[3]));
    ui->fBCEdit->setText(QString("%1").arg(effect.unk_AC[4]));
    ui->fC0Edit->setText(QString("%1").arg(effect.unk_AC[5]));
    ui->fC4Edit->setText(QString("%1").arg(effect.unk_AC[6]));

    ui->fC8Edit->setText(QString("%1").arg(effect.unk_AC[7]));
    ui->fCCEdit->setText(QString("%1").arg(effect.unk_AC[8]));
    ui->fD0Edit->setText(QString("%1").arg(effect.unk_AC[9]));
    ui->fD4Edit->setText(QString("%1").arg(effect.unk_AC[10]));
    ui->fD8Edit->setText(QString("%1").arg(effect.unk_AC[11]));
    ui->fDCEdit->setText(QString("%1").arg(effect.unk_AC[12]));
}

void IdbEffectDialog::GuiToEffect()
{
    effect.type = (uint32_t) ui->typeEdit->text().toInt();
    effect.activation_type = (uint32_t) ui->actTypeEdit->text().toInt();
    effect.num_act_times = (uint32_t) ui->numActEdit->text().toInt();

    effect.timer = ui->timerEdit->text().toFloat();
    effect.ability_values[0] = ui->ab1Edit->text().toFloat();
    effect.ability_values[1] = ui->ab2Edit->text().toFloat();
    effect.ability_values[2] = ui->ab3Edit->text().toFloat();
    effect.ability_values[3] = ui->ab4Edit->text().toFloat();
    effect.ability_values[4] = ui->ab5Edit->text().toFloat();
    effect.ability_values[5] = ui->ab6Edit->text().toFloat();

    effect.unk_28 = (uint32_t) ui->u28Edit->text().toInt();
    effect.activation_chance = (uint32_t) ui->chanceEdit->text().toInt();

    effect.multipliers[0] = ui->mul1Edit->text().toFloat();
    effect.multipliers[1] = ui->mul2Edit->text().toFloat();
    effect.multipliers[2] = ui->mul3Edit->text().toFloat();
    effect.multipliers[3] = ui->mul4Edit->text().toFloat();
    effect.multipliers[4] = ui->mul5Edit->text().toFloat();
    effect.multipliers[5] = ui->mul6Edit->text().toFloat();

    effect.unk_48[0] = (uint32_t) ui->u48Edit->text().toInt();
    effect.unk_48[1] = (uint32_t) ui->u4CEdit->text().toInt();
    effect.unk_48[2] = (uint32_t) ui->u50Edit->text().toInt();
    effect.unk_48[3] = (uint32_t) ui->u54Edit->text().toInt();
    effect.unk_48[4] = (uint32_t) ui->u58Edit->text().toInt();
    effect.unk_48[5] = (uint32_t) ui->u5CEdit->text().toInt();

    effect.hea = ui->heaEdit->text().toFloat();
    effect.ki = ui->kiEdit->text().toFloat();
    effect.ki_recovery = ui->kiRecEdit->text().toFloat();
    effect.stm = ui->stmEdit->text().toFloat();
    effect.stamina_recovery = ui->stmRecEdit->text().toFloat();
    effect.enemy_stamina_eraser = ui->enStaErEdit->text().toFloat();
    effect.unk_78 = ui->f78Edit->text().toFloat();

    effect.ground_speed = ui->groundSpeedEdit->text().toFloat();
    effect.air_speed = ui->airSpeedEdit->text().toFloat();
    effect.boosting_speed = ui->boostSpeedEdit->text().toFloat();
    effect.dash_speed = ui->dashSpeedEdit->text().toFloat();

    effect.atk = ui->atkEdit->text().toFloat();
    effect.basic_ki_attack = ui->basicKiAtkEdit->text().toFloat();
    effect.str = ui->strEdit->text().toFloat();
    effect.bla = ui->blaEdit->text().toFloat();
    effect.atk_damage = ui->atkDmgEdit->text().toFloat();
    effect.ki_damage = ui->kiDmgEdit->text().toFloat();
    effect.str_damage = ui->strDmgEdit->text().toFloat();
    effect.bla_damage = ui->blaDmgEdit->text().toFloat();

    effect.unk_AC[0] = ui->fACEdit->text().toFloat();
    effect.unk_AC[1] = ui->fB0Edit->text().toFloat();
    effect.unk_AC[2] = ui->fB4Edit->text().toFloat();
    effect.unk_AC[3] = ui->fB8Edit->text().toFloat();
    effect.unk_AC[4] = ui->fBCEdit->text().toFloat();
    effect.unk_AC[5] = ui->fC0Edit->text().toFloat();
    effect.unk_AC[6] = ui->fC4Edit->text().toFloat();

    effect.unk_AC[7] = ui->fC8Edit->text().toFloat();
    effect.unk_AC[8] = ui->fCCEdit->text().toFloat();
    effect.unk_AC[9] = ui->fD0Edit->text().toFloat();
    effect.unk_AC[10] = ui->fD4Edit->text().toFloat();
    effect.unk_AC[11] = ui->fD8Edit->text().toFloat();
    effect.unk_AC[12] = ui->fDCEdit->text().toFloat();
}

void IdbEffectDialog::on_buttonBox_rejected()
{
    reject();
}

void IdbEffectDialog::on_buttonBox_accepted()
{
    if (!Validate())
        return;

    GuiToEffect();
    accept();
}
