#include "sloteditdialog.h"
#include "ui_sloteditdialog.h"

#include <QMessageBox>
#include "listdialog.h"
#include "debug.h"

SlotEditDialog::SlotEditDialog(CharaListSlotEntry *entry, EmbFile *edit_emb, QWidget *parent) :
    entry(entry), edit_emb(edit_emb), QDialog(parent),
    ui(new Ui::SlotEditDialog)
{
    ui->setupUi(this);
    ui->costumeEdit->setValidator(new QIntValidator(this));
    ui->presetEdit->setValidator(new QIntValidator(this));
    ui->unlockEdit->setValidator(new QIntValidator(this));

    EntryToGui();
}

SlotEditDialog::~SlotEditDialog()
{
    delete ui;
}

bool SlotEditDialog::GuiToEntry()
{
    std::string code = Utils::QStringToStdString(ui->codeEdit->text());

    if (code.length() != 3 && code != "AvatarCode")
    {
        DPRINTF("Code must be a 3-letter code or AvatarCode.\n");
        return false;
    }

    entry->code = code;

    if (code.length() == 3)
        entry->code = std::string("\"") + entry->code + std::string("\"");

    if (ui->costumeEdit->text().isEmpty())
    {
        DPRINTF("Costume index cannot be empty.\n");
        return false;
    }

    entry->costume_index = ui->costumeEdit->text().toInt();

    if (ui->presetEdit->text().isEmpty())
    {
        DPRINTF("Model preset cannot be empty.\n");
        return false;
    }

    entry->model_preset = ui->presetEdit->text().toInt();

    if (ui->unlockEdit->text().isEmpty())
    {
        DPRINTF("Unlock index cannot be empty.\n");
        return false;
    }

    entry->unlock_index = ui->unlockEdit->text().toInt();

    entry->flag_gk2 = ui->gk2Check->isChecked();

    if (ui->voicesEdit->text().isEmpty())
    {
        DPRINTF("[SLOT] Voices id list cannot be empty.\n");
        return false;
    }

    std::vector<std::string> voices;

    Utils::GetMultipleStrings(Utils::QStringToStdString(ui->voicesEdit->text()), voices);

    if (voices.size() == 2)
    {
        try
        {
            entry->voices_id_list[0] = std::stoi(voices[0]);
            entry->voices_id_list[1] = std::stoi(voices[1]);
        } catch (const std::invalid_argument &)
        {
            DPRINTF("Wrong format in voices id list (parse error).\n");
            return false;
        }
    }
    else
    {
        DPRINTF("Wrong format in voices id list (wrong number elements, expecting 2, got %Id)\n", voices.size());
        return false;
    }

    entry->dlc = Utils::QStringToStdString(ui->dlcComboBox->itemText(ui->dlcComboBox->currentIndex()));

    if (code == "AvatarCode" || code == "HUM")
    {

    }
    else if (!game_cms->FindEntryByName(code))
    {
        QString message = "That character doesn't have a cms entry. It is not a good idea to proceed.\n"
                          "Do you want to continue anyway?";

        if (QMessageBox::warning(this, "Proceed?", message,
                                 QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                 QMessageBox::No) != QMessageBox::Yes)
        {
            return false;
        }
    }

    else if ((code == "BLM" || code == "TOK" || code == "DND" || code == "DMG") && entry->costume_index == 0)
    {
        if (edit_emb->FindIndexByName(code + "_000.dds") == 0xFFFF)
        {
            QString message = "The character you have added doesn't have a selection screen portrait, which may cause the CSS to not appear.\n"
                              "However, I have a specific fix for this character, by copying their battle portrait dds.\n\n"
                              "Do you want me to apply the fix?";

            int result = QMessageBox::warning(this, "Apply fix?", message,
                                      QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel),
                                      QMessageBox::Yes);

            if (result == QMessageBox::Cancel)
                return false;

            if (result == QMessageBox::Yes)
            {
                EmbFile btl_port;

                if (!xv2fs->LoadFile(&btl_port, "data/ui/texture/" + code + ".emb"))
                {
                    DPRINTF("Failed to load battle porttrait file, cannot apply fix.\n");
                    return false;
                }

                uint16_t idx = btl_port.FindIndexByName(code + "_000.dds");
                if (idx == 0xFFFF)
                    idx = 0;

                edit_emb->AppendFile(btl_port, idx);
            }
        }
    }
    else if (!Xenoverse2::IsOriginalChara(code) || code == "SIN")
    {
        bool found = false;

        for (int i = 0; i < 32; i++)
        {
            if (edit_emb->FindIndexByName(code + "_" + Utils::ToStringAndPad(i, 3) + ".dds") != 0xFFFF)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            QString message = "I cannot find a selection portrait for that character. If you proceed, you will probably end with a broken CSS.\n\n"
                              "Do you want to continue anyway?";

            if (QMessageBox::warning(this, "Proceed?", message,
                                      QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                      QMessageBox::No) != QMessageBox::Yes)
            {
                return false;
            }
        }
    }


    return true;
}

void SlotEditDialog::EntryToGui()
{
    std::string code = entry->code;

    if (code.length() == 5 && code[0] == '"' && code[4] == '"')
        code = code.substr(1, 3);

    ui->codeEdit->setText(Utils::StdStringToQString(code));
    ui->costumeEdit->setText(QString("%1").arg(entry->costume_index));
    ui->presetEdit->setText(QString("%1").arg(entry->model_preset));
    ui->unlockEdit->setText(QString("%1").arg(entry->unlock_index));
    ui->gk2Check->setChecked(entry->flag_gk2);
    ui->voicesEdit->setText(QString("%1,%2").arg(entry->voices_id_list[0]).arg(entry->voices_id_list[1]));

    bool found = false;
    for (int i = 0; i < ui->dlcComboBox->count(); i++)
    {
        if (Utils::QStringToStdString(ui->dlcComboBox->itemText(i)) == entry->dlc)
        {
            ui->dlcComboBox->setCurrentIndex(i);
            found = true;
            break;
        }
    }

    if (!found)
        ui->dlcComboBox->setCurrentIndex(0);
}


void SlotEditDialog::on_buttonBox_accepted()
{
    if (GuiToEntry())
        accept();
}

void SlotEditDialog::on_pushButton_clicked()
{
    ListDialog dialog(ListMode::CHARASEL_FULL, this);

    if (dialog.exec())
    {
        int ret = dialog.GetResultInteger();

        CharaListSlotEntry *entry = chara_list->FindFromAbsolutePos(ret);

        if (entry)
        {
            *this->entry = *entry;
            EntryToGui();
        }
    }
}
