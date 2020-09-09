#include "dialog.h"
#include "ui_dialog.h"

#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "Config.h"
#include "Xenoverse2.h"
#include "Cac2X2m.h"
#include "listdialog.h"
#include "waitdialog.h"
#include "x2mcacslotmapdialog.h"
#include "x2mcaccomponentsdialog.h"
#include "debug.h"

#define PROGRAM_NAME    "X2m2Cac"
#define PROGRAM_VERSION "1.01"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(QString("%1 %2").arg(PROGRAM_NAME).arg(PROGRAM_VERSION));
}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::Initialize()
{
    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    GameRequirements();

    if (!config.lf_external_sav.isEmpty())
    {
        std::string path = Utils::QStringToStdString(config.lf_external_sav);

        if (Utils::FileExists(path))
        {
            ui->savEdit->setText(config.lf_external_sav);
        }
    }

    return true;
}

void Dialog::closeEvent(QCloseEvent *event)
{
    if (ProcessShutdown())
        event->accept();
    else
        event->ignore();
}

void Dialog::GameRequirements()
{
    config.Load();

    if (!config.ConfigureRequirements())
    {
        DPRINTF("ConfigureRequirements failed.\n");
        exit(-1);
    }

    Xenoverse2::InitFs(Utils::QStringToStdString(config.game_directory));

    if (!Xenoverse2::InitSystemFiles())
    {
        DPRINTF("InitSystemFiles failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCacCostumeNames())
    {
        DPRINTF("InitCacCostumeNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitIdb())
    {
        DPRINTF("InitIdb failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCac())
    {
        DPRINTF("InitCac failed.\n");
        exit(-1);
    }
}

bool Dialog::ProcessShutdown()
{
    config.Save();
    return true;
}

bool Dialog::X2mSlotToCacSlot(XV2Cac *cac, X2mFile *x2m, uint32_t x2m_race, uint32_t cac_slot, uint32_t x2m_slot, bool basic, bool body_components, bool keep_race, bool costumes, bool skills, bool stats, bool &face_base_set, bool &face_forehead_set, bool &hair_set, bool &eyes_set, bool &nose_set, bool &ears_set)
{
    BcsFile *bcs;
    XV2CacSet *set = &cac->sets[cac_slot];

    if (x2m_race == CAC_HUM || x2m_race == CAC_SYM)
    {
        bcs = game_hum_bcs;
    }
    else if (x2m_race == CAC_HUF || x2m_race == CAC_SYF)
    {
        bcs = game_huf_bcs;
    }
    else if (x2m_race == CAC_NMC)
    {
        bcs = game_nmc_bcs;
    }
    else if (x2m_race == CAC_FRI)
    {
        bcs = game_fri_bcs;
    }
    else if (x2m_race == CAC_MAM)
    {
        bcs = game_mam_bcs;
    }
    else
    {
        bcs = game_maf_bcs;
    }

    if (basic)
    {
        if (!x2m->UsesBodyShape())
        {
            DPRINTF("X2M doesn't have body shape data.\n");
            return false;
        }

        strncpy(cac->name, x2m->GetCharaName(XV2_LANG_ENGLISH).c_str(), sizeof(cac->name));
        int voice = X2m2Cac::GetVoice(x2m);

        if (voice < 0)
        {
            DPRINTF("Failed to get voice or voice.\n");
            return false;
        }

        cac->voice = (uint32_t)voice;
        cac->body_shape = x2m->GetBodyShape();
    }

    uint32_t colors[4];
    bool accesory;
    bool accesory_set = false;

    if (body_components)
    {
        if (cac->race != x2m_race)
        {
            if (!keep_race)
            {
                cac->race = x2m_race;
            }
        }

        if (!face_base_set)
        {
            if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_FACE_BASE, &cac->face_base, colors, &accesory))
            {
                DPRINTF("Failed to find face base.\n");
                return false;
            }

            cac->skin_color1 = (uint16_t)colors[0];

            if (cac->race == CAC_NMC || cac->race == CAC_FRI)
            {
                cac->skin_color2 = (uint16_t)colors[1];
                cac->skin_color3 = (uint16_t)colors[2];

                if (cac->race == CAC_FRI)
                {
                    cac->skin_color4 = (uint16_t)colors[3];
                }
                else
                {
                    cac->skin_color4 = 0xFFFF;
                }
            }
            else
            {
                cac->skin_color2 = cac->skin_color3 = cac->skin_color4 = 0xFFFF;
            }

            face_base_set = true;
        }

        if (!face_forehead_set)
        {
            if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_FACE_FOREHEAD, &cac->face_forehead, colors, &accesory))
            {
                DPRINTF("Failed to find face forehead.\n");
                return false;
            }

            cac->makeup_color1 = (uint16_t)colors[0];
            cac->makeup_color2 = (uint16_t)colors[1];
            cac->makeup_color3 = (uint16_t)colors[2];

            face_forehead_set = true;
        }

        if (!eyes_set)
        {
            if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_FACE_EYE, &cac->eyes, colors, &accesory))
            {
                DPRINTF("Failed to find eyes.\n");
                return false;
            }

            cac->eye_color = (uint16_t)colors[0];
            eyes_set = true;
        }

        else if (!nose_set)
        {
            if (cac->race == CAC_MAM || cac->race == CAC_MAF)
            {
                cac->nose = 0xFFFFFFFF;
            }
            else
            {
                if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_FACE_NOSE, &cac->nose, colors, &accesory))
                {
                    DPRINTF("Failed to find nose.\n");
                    return false;
                }
            }

            nose_set = true;
        }
        else if (!ears_set)
        {
            if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_FACE_EAR, &cac->ears, colors, &accesory))
            {
                DPRINTF("Failed to find ear.\n");
                return false;
            }

            ears_set = true;
        }
    }

    // Hair here due to the possibility of setting accesory

    if ((body_components && !hair_set) || costumes)
    {
        uint32_t my_hair;

        if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_HAIR, &my_hair, colors, &accesory))
        {
            DPRINTF("Failed to find hair.\n");
            return false;
        }

        if (!accesory)
        {
            if (body_components && !hair_set)
            {
                cac->hair = my_hair;

                if (cac->race == CAC_HUM || cac->race == CAC_HUF || cac->race == CAC_SYM || cac->race == CAC_SYF)
                {
                    cac->hair_color = (uint16_t)colors[0];
                }
                else
                {
                    cac->hair_color = 0xFFFF;
                }

                hair_set = true;
            }
        }
        else
        {
            if (costumes)
            {
                accesory_set = true;
                set->accesory = my_hair;
            }
        }
    }

    if (costumes)
    {
        if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_BUST, &set->top, colors, &accesory))
        {
            DPRINTF("Failed to find bust.\n");
            return false;
        }

        set->top_color1 = (uint16_t)colors[0];
        set->top_color2 = (uint16_t)colors[1];
        set->top_color3 = (uint16_t)colors[2];
        set->top_color4 = (uint16_t)colors[3];

        if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_PANTS, &set->bottom, colors, &accesory))
        {
            DPRINTF("Failed to find pants.\n");
            return false;
        }

        set->bottom_color1 = (uint16_t)colors[0];
        set->bottom_color2 = (uint16_t)colors[1];
        set->bottom_color3 = (uint16_t)colors[2];
        set->bottom_color4 = (uint16_t)colors[3];

        if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_RIST, &set->gloves, colors, &accesory))
        {
            DPRINTF("Failed to find rist.\n");
            return false;
        }

        set->gloves_color1 = (uint16_t)colors[0];
        set->gloves_color2 = (uint16_t)colors[1];
        set->gloves_color3 = (uint16_t)colors[2];
        set->gloves_color4 = (uint16_t)colors[3];

        if (!X2m2Cac::GetPart(x2m, *bcs, x2m_slot, PART_BOOTS, &set->shoes, colors, &accesory))
        {
            DPRINTF("Failed to find boots.\n");
            return false;
        }

        set->shoes_color1 = (uint16_t)colors[0];
        set->shoes_color2 = (uint16_t)colors[1];
        set->shoes_color3 = (uint16_t)colors[2];
        set->shoes_color4 = (uint16_t)colors[3];

        if (!accesory_set)
        {
            if (!X2m2Cac::FindAccessory(x2m, *bcs, x2m_slot, &set->accesory))
            {
                set->accesory = 0xFFFFFFFF;
            }
        }

        // Talisman is part of equipment
        set->talisman = X2m2Cac::GetTalisman(x2m, x2m_slot);
    }

    if (skills)
    {
        set->super_skills[0] = X2m2Cac::GetSkill(x2m, x2m_slot, 0, false);
        set->super_skills[1] = X2m2Cac::GetSkill(x2m, x2m_slot, 1, false);
        set->super_skills[2] = X2m2Cac::GetSkill(x2m, x2m_slot, 2, false);
        set->super_skills[3] = X2m2Cac::GetSkill(x2m, x2m_slot, 3, false);
        set->ult_skills[0] = X2m2Cac::GetSkill(x2m, x2m_slot, 4, false);
        set->ult_skills[1] = X2m2Cac::GetSkill(x2m, x2m_slot, 5, false);
        set->evasive_skill= X2m2Cac::GetSkill(x2m, x2m_slot, 6, false);
        // Cac get blast skill from talisman
        //set->blast_skill = X2m2Cac::GetSkill(x2m, x2m_slot, 7, false);
        set->awaken_skill = X2m2Cac::GetSkill(x2m, x2m_slot, 8, false);
    }

    if (stats)
    {
        uint16_t hea, ki, stm, atk, str, bla;

        if (!X2m2Cac::GetStats(x2m, x2m_slot, &hea, &ki, &stm, &atk, &str, &bla))
        {
            DPRINTF("Cannot get stats. Maybe not a Cac2X2m/Cycyt x2m.\n");
            return false;
        }

        cac->hea = hea;
        cac->ki = ki;
        cac->stm = stm;
        cac->atk = atk;
        cac->str = str;
        cac->bla = bla;
    }

    return true;
}

void Dialog::on_x2mButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open file", config.lf_installer_open, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_installer_open = file;
    config.Save();

    ui->x2mEdit->setText(file);
}

void Dialog::on_savButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(nullptr, "Select save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");

    if (file.isNull())
        return;

    config.lf_external_sav = file;
    config.Save();

    ui->savEdit->setText(file);
}

void Dialog::on_buttonBox_rejected()
{
    qApp->exit(0);
}

void Dialog::on_buttonBox_accepted()
{
    QString x2m_path = ui->x2mEdit->text().trimmed();

    if (x2m_path.isEmpty())
    {
        DPRINTF("The x2m text field is empty!");
        return;
    }

    QString sav_path = ui->savEdit->text().trimmed();

    if (sav_path.isEmpty())
    {
        DPRINTF("The sav text field is empty!\n");
        return;
    }

    X2mFile x2m;
    Xv2SavFile sav;

    if (!x2m.LoadFromFile(Utils::QStringToStdString(x2m_path)))
    {
        DPRINTF("Failed to load x2m file.\n");
        return;
    }

    if (x2m.GetType() != X2mType::NEW_CHARACTER)
    {
        DPRINTF("That is not a character x2m.\n");
        return;
    }

    uint32_t x2m_race = X2m2Cac::GetRace(&x2m);
    if (x2m_race == (uint32_t)-1)
    {
        DPRINTF("This doesn't seem to be a Cac2X2m/Cycit x2m.\n");
        return;
    }

    if (!sav.LoadFromFile(Utils::QStringToStdString(sav_path)))
    {
        DPRINTF("Failed to load save file.\n");
        return;
    }

    bool single_costume = (x2m.GetNumSlotEntries() == 1);
    uint32_t cac_idx;
    std::map<uint32_t, uint32_t> costume_maps;
    XV2Cac *cac;

    if (single_costume)
    {
        ListDialog dialog(ListMode::CAC_FULL, this, (void *)&sav);

        if (!dialog.exec())
            return;

        uint32_t result = dialog.GetResultData();

        if ((int)result < 0)
        {
            DPRINTF("You didn't select any cac.\n");
            return;
        }

        cac_idx = result / XV2_NUM_PRESETS;
        costume_maps[0] = result % XV2_NUM_PRESETS;
    }
    else
    {
        ListDialog dialog(ListMode::CAC_PARTIAL, this, (void *)&sav);

        if (!dialog.exec())
            return;

        cac_idx = dialog.GetResultData();
    }

    if ((int)cac_idx < 0)
    {
        DPRINTF("You didn't select any cac.\n");
        return;
    }

    cac = sav.GetCac(cac_idx);
    if (!cac || cac->race >= CAC_NUM_RACES || !sav.IsValidCac(cac_idx))
    {
        DPRINTF("Error obtaining cac data from save.\n");
        return;
    }

    if (!single_costume)
    {
        X2mCacSlotMapDialog dialog(&x2m, &costume_maps, this);

        if (!dialog.exec())
            return;
    }

    X2mCacComponentsDialog dialog(cac->race, x2m_race, this);

    if (!dialog.exec())
        return;

    bool use_basic = dialog.useBasic();
    bool use_body_components = dialog.useBodyComponents();
    bool keep_race = dialog.keepRace();
    bool use_costumes = dialog.useCostumes();
    bool use_skills = dialog.useSkills();
    bool use_stats = dialog.useStats();

    if (cac->race != x2m_race)
    {
        if (use_body_components && !keep_race)
        {
            QString text = "Current settings will change the race in the save file.\n"
                           "This may cause problems with specific race quests.\n\n"
                           "Do you want to contnue?";

            if (QMessageBox::question(this, "Change race?", text,
                                      QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                      QMessageBox::No) != QMessageBox::Yes)
            {
                return;
            }
        }
    }

    bool face_base_set = false;
    bool face_forehead_set = false;
    bool hair_set = false;
    bool eyes_set = false;
    bool nose_set = false;
    bool ears_set = false;

    WaitDialog wait;
    wait.show();
    qApp->processEvents();

    for (auto &it : costume_maps)
    {
        if (!X2mSlotToCacSlot(cac, &x2m, x2m_race, it.second, it.first, use_basic, use_body_components,
                              keep_race, use_costumes, use_skills, use_stats, face_base_set, face_forehead_set,
                              hair_set, nose_set, ears_set, eyes_set))
        {
            DPRINTF("Failed on x2m slot: %d, cac slot: %d\n"
                    "Nothing was written to the save file.\n", it.first, it.second);
            wait.close();
            return;
        }

        use_basic = false;
        use_stats = false;
    }

    if (use_body_components && !hair_set)
    {
        if (cac->race == CAC_HUM || cac->race == CAC_HUF || cac->race == CAC_SYM || cac->race == CAC_SYF)
        {
            cac->hair = 50;
        }
    }

    wait.close();

    QString text = "The operations in RAM have finished with success.\n"
                   "Upon clicking Yes button, the data will be written to the save file.\n\n"
                   "Do you want to modify the save file?\n\n"
                   "DISCLAIMER: IF YOU HAVEN'T DONE A BACKUP OF YOUR SAVE, THIS IS THE MOMENT TO DO IT.";

    if (QMessageBox::question(this, "Modify save?", text,
                              QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                              QMessageBox::Yes) != QMessageBox::Yes)
    {
        return;
    }

    if (!sav.SaveToFile(Utils::QStringToStdString(sav_path)))
    {
        DPRINTF("Failed to save sav file.\n");
        return;
    }

    UPRINTF("Data succesfully commited to the save!\n");
}
