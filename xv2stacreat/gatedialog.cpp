#include "gatedialog.h"
#include "ui_gatedialog.h"

#include "listdialog.h"

GateDialog::GateDialog(Xv2StageGate *gate, QWidget *parent) :
    gate(gate), QDialog(parent),
    ui(new Ui::GateDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->targetEdit->setValidator(new QIntValidator(-1, 0xFF, this));
    ui->u0CEdit->setValidator(new QIntValidator(this));
    ui->u10Edit->setValidator(new QIntValidator(this));

    GateToGui(*gate);
}

GateDialog::~GateDialog()
{
    delete ui;
}

void GateDialog::GateToGui(const Xv2StageGate &gate)
{
    ui->nameEdit->setText(Utils::StdStringToQString(gate.name));
    ui->targetEdit->setText(QString("%1").arg((int)gate.target_stage_idx));
    ui->u0CEdit->setText(QString("%1").arg(gate.unk_0C));
    ui->u10Edit->setText(QString("%1").arg(gate.unk_10));
}

void GateDialog::GuiToGate(Xv2StageGate &gate)
{
    gate.name = Utils::QStringToStdString(ui->nameEdit->text().trimmed());
    gate.target_stage_idx = (uint32_t) ui->targetEdit->text().toInt();
    gate.unk_0C = (uint32_t) ui->u0CEdit->text().toInt();
    gate.unk_10 = (uint32_t) ui->u10Edit->text().toInt();
}

void GateDialog::on_pushButton_clicked()
{
    ListDialog dialog(ListMode::STAGE, this);

    if (dialog.exec())
    {
        ui->targetEdit->setText(QString("%1").arg(dialog.GetResultInteger()));
    }
}

void GateDialog::on_buttonBox_accepted()
{
    GuiToGate(*gate);
}
