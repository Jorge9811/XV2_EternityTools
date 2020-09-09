#include <QPushButton>
#include "cssdialog.h"

#include "slotreorderdialog.h"
#include "ui_slotreorderdialog.h"

bool SlotReorderDialog::remember_before = false;

SlotReorderDialog::SlotReorderDialog(CharaListSlot *cslot, size_t idx, QWidget *parent) :
    QDialog(parent), cslot(cslot), idx(idx),
    ui(new Ui::SlotReorderDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for (size_t i = 0; i < cslot->entries.size(); i++)
    {
        if (i != idx)
        {
            ui->comboBox->addItem(CssDialog::GetCharaAndCostumeName(cslot->entries[i]), QVariant(i));
        }
    }

    ui->beforeCheckBox->setChecked(remember_before);
}

SlotReorderDialog::~SlotReorderDialog()
{
    delete ui;
}

void SlotReorderDialog::on_buttonBox_accepted()
{
    int index = ui->comboBox->currentIndex();

    if (index >= 0 && index < ui->comboBox->count())
    {
        size_t target_idx = ui->comboBox->itemData(index).toInt();
        bool before = ui->beforeCheckBox->isChecked();
        size_t del_idx;

        CharaListSlotEntry source = cslot->entries[idx];

        if (before)
        {
            cslot->entries.insert(cslot->entries.begin()+target_idx, source);
            del_idx = idx;

            if (target_idx < del_idx)
            {
                del_idx++;
            }
        }
        else
        {
            if (target_idx+1 == cslot->entries.size())
            {
                cslot->entries.push_back(source);
            }
            else
            {
                cslot->entries.insert(cslot->entries.begin()+target_idx+1, source);
            }

            del_idx = idx;

            if (target_idx+1 < del_idx)
            {
                del_idx++;
            }
        }

        cslot->entries.erase(cslot->entries.begin()+del_idx);
    }

    remember_before = ui->beforeCheckBox->isChecked();
}
