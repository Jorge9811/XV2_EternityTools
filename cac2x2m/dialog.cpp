#include "dialog.h"
#include "ui_dialog.h"

#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>

#include "Config.h"
#include "Xenoverse2.h"
#include "Cac2X2m.h"
#include "listdialog.h"
#include "waitdialog.h"
#include "debug.h"

#pragma warning(disable:4258)

#define PROGRAM_NAME    "Cac2X2m"
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

    QString file = QFileDialog::getOpenFileName(nullptr, "Select save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");
    if (file.isNull())
        return false;

    if (!sav.LoadFromFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Failed to load save file.\n");
        return false;
    }

    ListDialog dialog(ListMode::CAC_PARTIAL, nullptr, &sav);
    if (!dialog.exec())
        return false;

    cac = sav.GetCac(dialog.GetResultData());
    if (!cac || cac->race >= CAC_NUM_RACES)
        return false;

    costume_checks.push_back(ui->c0Edit);
    costume_checks.push_back(ui->c1Edit);
    costume_checks.push_back(ui->c2Edit);
    costume_checks.push_back(ui->c3Edit);
    costume_checks.push_back(ui->c4Edit);
    costume_checks.push_back(ui->c5Edit);
    costume_checks.push_back(ui->c6Edit);
    costume_checks.push_back(ui->c7Edit);

    for (int i = 0 ; i < XV2_NUM_PRESETS; i++)
    {
        uint32_t top = cac->sets[i].top;
        std::string name;

        if (top < 0xFFFF)
            name = Xenoverse2::GetCacTopName(top);

        if (name.length() == 0)
        {
            if (i == 0)
                name = "Current Clothing";
            else
                name = "Preset " + Utils::ToString(i);

            costume_checks[i]->setChecked(false);
        }
        else
        {
            costume_checks[i]->setChecked(true);
        }

        costume_checks[i]->setText(Utils::StdStringToQString(name, false));;
    }

    return true;
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

    if (!Xenoverse2::InitSkillNames())
    {
        DPRINTF("InitSkillNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCacCostumeNames())
    {
        DPRINTF("InitCacCostumeNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitTalismanNames())
    {
        DPRINTF("InitTalismanNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitIdb())
    {
        DPRINTF("InitIdb failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitSelPort())
    {
        DPRINTF("InitSelPort failed.\n");
        exit(-1);
    }
}

bool Dialog::ProcessShutdown()
{
    config.Save();
    return true;
}

bool Dialog::ConvertToX2m(const QString &file, const QString &code, const uint8_t *guid, const std::vector<XV2CacSet *> &sets, bool css_voice)
{
    X2mFile x2m;
    std::string race;

    if (cac->race == CAC_HUM)
        race = "HUM";
    else if (cac->race == CAC_HUF)
        race = "HUF";
    else if (cac->race == CAC_SYM)
        race = "SYM";
    else if (cac->race == CAC_SYF)
        race = "SYF";
    else if (cac->race == CAC_NMC)
        race = "NMC";
    else if (cac->race == CAC_FRI)
        race = "FRI";
    else if (cac->race == CAC_MAM)
        race = "MAM";
    else if (cac->race == CAC_MAF)
        race = "MAF";
    else
        return false;

    const std::vector<CusSkill> &blast_skills = game_cus->GetBlastSkills();
    if (blast_skills.size() == 0)
        return false;

    if (guid)
    {
        x2m.SetModGuid(Utils::GUID2String(guid));
    }

    x2m.SetType(X2mType::NEW_CHARACTER);
    x2m.SetModName(cac->name);
    x2m.SetModVersion(1.0f);
    x2m.SetModAuthor(Utils::QStringToStdString(QString("%1 v%2").arg(PROGRAM_NAME).arg(PROGRAM_VERSION)));
    x2m.SetEntryName(Utils::QStringToStdString(code));

    x2m.SetCharaName(cac->name, XV2_LANG_ENGLISH);

    // Cms / Files
    BcsFile orig_bcs, new_bcs;
    EskFile new_esk;
    EanFile new_cam_ean;
    std::unordered_set<std::string> files;
    std::string base_dir;
    std::vector<uint32_t> extra_trans;
    std::vector<uint32_t> et_def_set;

    x2m.SetBodyShape(cac->body_shape);
    x2m.SetCanUseAnyDualSkill(true);

    if (!Cac2X2m::SetCms(&x2m, race, files, base_dir, orig_bcs, new_esk, new_cam_ean))
    {
        DPRINTF("SetCms failed.\n");
        return false;
    }

    new_bcs = orig_bcs;

    for (size_t i = 0; i < sets.size(); i++)
    {
        const XV2CacSet *set = sets[i];

        uint32_t bust = Cac2X2m::BustItem(set->top);
        uint32_t pants = Cac2X2m::PantsItem(set->bottom);
        uint32_t rist = Cac2X2m::RistItem(set->gloves);
        uint32_t boots = Cac2X2m::BootsItem(set->shoes);
        uint32_t accesory = Cac2X2m::AccesoryItem(set->accesory);

        BcsPartSet &output_set = new_bcs.GetPartSets()[i];

        if (!Cac2X2m::BuildPartSet(&x2m, output_set, orig_bcs, files, base_dir,
                                   cac->face_base, cac->face_forehead, cac->eyes, cac->nose, cac->ears,
                                   cac->hair, bust, pants, rist, boots, accesory))
        {
            DPRINTF("BuildPartSet failed.\n");
            return false;
        }

        if (!Cac2X2m::SetColors(output_set, orig_bcs,
                                cac->skin_color1, cac->skin_color2, cac->skin_color3, cac->skin_color4,
                                cac->hair_color, cac->eye_color,
                                set->top_color1, set->top_color2, set->top_color3, set->top_color4,
                                set->bottom_color1, set->bottom_color2, set->bottom_color3, set->bottom_color4,
                                set->gloves_color1, set->gloves_color2, set->gloves_color3, set->gloves_color4,
                                set->shoes_color1, set->shoes_color2, set->shoes_color3, set->shoes_color4,
                                cac->makeup_color1, cac->makeup_color2, cac->makeup_color3))
        {
            DPRINTF("SetColors failed.\n");
            return false;
        }

        // Cso
        CsoEntry cso_entry;

        if (!Cac2X2m::SetCso(race, cso_entry, files, cac->voice))
        {
            DPRINTF("SetCso failed.\n");
            return false;
        }

        x2m.AddCsoEntry(cso_entry);

        // Slots
        X2mSlotEntry slot;

        for (int j = 0; j < XV2_LANG_NUM; j++)
        {
            std::string name;

            if (set->top < 0xFFFF)
                name = Xenoverse2::GetCacTopName(set->top, j);

            if (name.length() > 0)
            {
                slot.costume_name[j] = name;
            }
            else
            {
                if (j == XV2_LANG_ENGLISH)
                {
                    // Urgh hack
                    uint32_t x = Utils::DifPointer(set, cac->sets) / sizeof(XV2CacSet);

                    if (x == 0)
                        slot.costume_name[j] = "Current Clothing";
                    else
                    {
                        slot.costume_name[j] = "Preset " + Utils::ToString(x);
                    }
                }
            }
        }

        if (css_voice)
        {
            slot.voices_id_list[0] = slot.voices_id_list[1] = X2M_DUMMY_ID;
            slot.audio_files[0] = slot.audio_files[1] = "VOICE";
        }

        slot.costume_index = (int)i;
        x2m.AddSlotEntry(slot);

        // Cus
        CusSkillSet skill_set;

        for (int i = 0; i < 4; i++)
            skill_set.char_skills[i] = set->super_skills[i];

        for (int i = 0; i < 2; i++)
            skill_set.char_skills[4+i] = set->ult_skills[i];

        skill_set.char_skills[6] = set->evasive_skill;
        skill_set.char_skills[7] = set->blast_skill;
        skill_set.char_skills[8] = set->awaken_skill;

        if (set->talisman < 0xFFFF)
        {
            uint32_t blast = Xenoverse2::GetBlastFromTalisman(set->talisman, false);
            if (blast != (uint32_t)-1)
                skill_set.char_skills[7] = blast;
        }

        if (skill_set.char_skills[7] >= 0xFFFF)
            skill_set.char_skills[7] = 21081;

        Cac2X2m::ResolveCustomSkills(&x2m, skill_set);
        x2m.AddSkillSet(skill_set);

        if (set->awaken_skill < 0x8000)
        {
            CusSkill *awa_skill = game_cus->FindAwakenSkillByID(set->awaken_skill);
            if (awa_skill)
            {
                if (awa_skill->partset != 0xFFFF)
                {
                    bool ssj = (awa_skill->partset >= 100 && awa_skill->partset <= 102) || (awa_skill->partset >= 297 && awa_skill->partset <= 299);

                    if (!ssj && std::find(extra_trans.begin(), extra_trans.end(), awa_skill->partset) == extra_trans.end())
                    {
                        uint32_t num_trans = awa_skill->num_transforms;
                        if (num_trans == 0xFFFFFFFF)
                            num_trans = 1;

                        for (uint32_t j = 0; j < num_trans; j++)
                        {
                            extra_trans.push_back(awa_skill->partset+j);
                            et_def_set.push_back((uint32_t)i);
                        }
                    }
                }
            }
        }

        // Psc
        PscSpecEntry psc_entry;

        if (!Cac2X2m::SetPsc(race, psc_entry, cac->body_shape, cac->hea, cac->ki, cac->stm,
                             cac->atk, cac->str, cac->bla, set->talisman))
        {
            DPRINTF("SetPsc failed.\n");
            return false;
        }

        x2m.AddPscEntry(psc_entry);

        // aur
        AurCharaLink aur_entry;

        if (!Cac2X2m::SetAura(race, aur_entry))
        {
            DPRINTF("SetAura failed.\n");
            return false;
        }

        x2m.AddAurEntry(aur_entry);

        // Cml
        CmlEntry cml_entry;

        if (!Cac2X2m::SetCml(race, cml_entry, cac->body_shape))
        {
            DPRINTF("SetCml failed.\n");
            return false;
        }

        x2m.AddCmlEntry(cml_entry);
    }

    if (!Cac2X2m::SetBody(new_esk, new_cam_ean, orig_bcs, cac->body_shape))
    {
        DPRINTF("SetBody failed.\n");
        return false;
    }

    if (race == "HUM" || race == "HUF" || race == "SYM" || race == "SYF")
    {
        if (!Cac2X2m::CopySSJ(new_bcs, orig_bcs, files, base_dir, x2m.GetEntryName()))
        {
            DPRINTF("CopySuperSaiyan failed.\n");
            return false;
        }
    }

    if (extra_trans.size() > 0)
    {
        if (!Cac2X2m::CopyTransforms(new_bcs, orig_bcs, extra_trans, et_def_set, (uint32_t)sets.size(), files, base_dir, x2m.GetEntryName()))
        {
            DPRINTF("CopyTransforms failed.\n");
            return false;
        }
    }

    // Write files
    if (!Cac2X2m::WriteFiles(&x2m, files))
    {
        DPRINTF("WriteFiles failed.\n");
        return false;
    }

    if (!Cac2X2m::WriteCustomFiles(&x2m, new_bcs, new_esk, new_cam_ean))
    {
        DPRINTF("WriteBcs failed.\n");
        return false;
    }

    if (!Cac2X2m::WriteSelPort(&x2m, race))
    {
        DPRINTF("WriteSelPort failed.\n");
        return false;
    }

    // 0.3: no battle portrait = auto battle portrait
    /*if (!Cac2X2m::WriteBtlPort(&x2m, race))
    {
        DPRINTF("WriteBtlPort failed.\n");
        return false;
    }*/

    if (race == "HUM" || race == "HUF" || race == "SYM" || race == "SYF")
    {
        if (!Cac2X2m::FixSSJ3(&x2m))
        {
            DPRINTF("FixSSJ3 failed.\n");
            return false;
        }
    }

    // Audio tracks
    if (css_voice)
    {
        if (!Cac2X2m::WriteCssVoice(&x2m, race, cac->voice))
        {
            DPRINTF("WriteCssVoice failed.\n");
            return false;
        }
    }

    if (!x2m.SaveToFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Failed to write x2m.\n");
        return false;
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



void Dialog::on_codeButton_clicked()
{
    std::string code;

    do
    {
        code.clear();

        while (code.length() != 3)
        {
            char c;

            Utils::GetRandomData(&c, 1);

            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
                code += c;
        }

    } while (Xenoverse2::IsOriginalChara(code) || game_cms->FindEntryByName(code));

    ui->codeEdit->setText(Utils::StdStringToQString(code));
}

void Dialog::on_buttonBox_rejected()
{
    reject();
}

void Dialog::on_buttonBox_accepted()
{
    QString code = ui->codeEdit->text();

    if (code.isEmpty())
    {
        DPRINTF("Code field is empty!\n");
        return;
    }

    if (code.length() != 3)
    {
        DPRINTF("Code must be a 3-letter code.\n");
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        char c = code[i].toLatin1();

        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        {
            // Ok
        }
        else
        {
            DPRINTF("Code can only use capital letters or digits.\n");
            return;
        }

    }

    if (Xenoverse2::IsOriginalChara(Utils::QStringToStdString(code)))
    {
        DPRINTF("This char code can't be used because it is in use by the game.\n");
        return;
    }

    std::vector<XV2CacSet *> sets;

    for (int i = 0; i < XV2_NUM_PRESETS; i++)
    {
        if (costume_checks[i]->isChecked())
        {
            sets.push_back(&cac->sets[i]);
        }
    }

    if (sets.size() == 0)
    {
        DPRINTF("How about you check at least one costume, huh?\n");
        return;
    }

    QString file = QFileDialog::getSaveFileName(this, "Save", config.GetDirectory(config.lf_editor_save), "X2M Files (*.x2m)");
    if (file.isNull())
        return;

    config.lf_editor_save = file;
    config.Save();

    uint8_t guid[16];
    int guid_mode = ui->guidComboBox->currentIndex();
    bool use_guid = false;

    if (guid_mode >= 0 && guid_mode <= 2)
    {
        use_guid = true;

        Utils::Md5(cac->name, (uint32_t)strlen(cac->name), guid);

        if (guid_mode != 2)
        {
            std::string race;
            uint8_t md5_race[16];

            if (cac->race == CAC_HUM)
                race = "HUM";
            else if (cac->race == CAC_HUF)
                race = "HUF";
            else if (cac->race == CAC_SYM)
                race = "SYM";
            else if (cac->race == CAC_SYF)
                race = "SYF";
            else if (cac->race == CAC_NMC)
                race = "NMC";
            else if (cac->race == CAC_FRI)
                race = "FRI";
            else if (cac->race == CAC_MAM)
                race = "MAM";
            else if (cac->race == CAC_MAF)
                race = "MAF";
            else
                return;

            Utils::Md5(race.c_str(), (uint32_t)race.length(), md5_race);
            Utils::XorBuf(guid, md5_race, sizeof(guid));

            if (guid_mode == 1)
            {
                uint8_t md5_steam[16];
                uint64_t steam_id = sav.GetSteamID64();

                Utils::Md5(&steam_id, sizeof(steam_id), md5_steam);
                Utils::XorBuf(guid, md5_steam, sizeof(guid));
            }
        }
    }

    WaitDialog wait;

    wait.show();
    qApp->processEvents();

    bool ret= ConvertToX2m(file, code, (use_guid) ? guid : nullptr, sets, ui->voiceCheck->isChecked()); //EntryToX2m(entry, file, dialog.GetCode(), (random_guid) ? nullptr : guid, dialog.UseCssVoice());
    wait.close();

    if (ret)
    {
        UPRINTF("Conversion done succesfully.\n");
    }
    else
    {
        DPRINTF("Conversion failed.\n");
    }

    accept();
}
