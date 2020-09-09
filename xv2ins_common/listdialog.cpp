#include "listdialog.h"
#include "ui_listdialog.h"

#include "Xenoverse2.h"
#include "X2mFile.h"
#include "EmbFile.h"

#define BEHAVIOUR_MAX	25

ListDialog::ListDialog(ListMode mode, QWidget *parent, void *extra, int flags) :
    QDialog(parent),  ui(new Ui::ListDialog),  mode(mode), extra(extra), flags(flags)

{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    PopulateList();
}

ListDialog::~ListDialog()
{
    delete ui;
}

bool ListDialog::BattlePortraitVisitor(const std::string &path, bool, void *param)
{
    ListDialog *pthis = (ListDialog *)param;

    if (Utils::EndsWith(path, ".emb", false))
    {
        size_t pos = path.rfind('/');

        if (pos != std::string::npos)
        {
            std::string name = path.substr(pos+1);

            if (name.length() == 7)
            {
                pthis->ui->listWidget->addItem(Utils::StdStringToQString(name));
            }
        }
    }

    return true;
}

bool ListDialog::StageLightingVisitor(const std::string &path, bool, void *param)
{
    ListDialog *pthis = (ListDialog *)param;

    if (Utils::EndsWith(path, ".emb", false))
    {
        size_t pos = path.rfind('/');

        if (pos != std::string::npos)
        {
            std::string name = path.substr(pos+1);
            pthis->ui->listWidget->addItem(Utils::StdStringToQString(name));
        }
    }

    return true;
}

bool ListDialog::TtcLlFilesVisitor(const std::string &path, bool, void *param)
{
    ListDialog *pthis = (ListDialog *)param;
    const std::string fn = Utils::GetFileNameString(path);

    if (fn.length() == 16 && Utils::BeginsWith(fn, "qc_", false) && Utils::EndsWith(fn, "_voice.msg", false))
    {
        std::string char_code = fn.substr(3, 3);
        pthis->ui->listWidget->addItem(Utils::StdStringToQString(char_code));
    }

    return true;
}

void ListDialog::PopulateList()
{
    if (mode == ListMode::SELECT_PORTRAIT)
    {
        for (const EmbContainedFile &file : *game_sel_port)
        {
            ui->listWidget->addItem(Utils::StdStringToQString(file.GetName()));
        }
    }
    else if (mode == ListMode::BATTLE_PORTRAIT)
    {
        xv2fs->VisitDirectory("data/ui/texture/", true, false, false, BattlePortraitVisitor, this);
    }
    else if (mode == ListMode::CHARASEL_PARTIAL)
    {
        if (!chara_list)
        {
            DPRINTF("Internal error, chara_list not initialized.\n");
            return;
        }

        ui->listWidget->setSortingEnabled(false);

        for (const CharaListSlot &slot : *chara_list)
        {
            std::string code;
            std::string name;

            if (slot.entries.size() > 0)
            {
                const CharaListSlotEntry &entry = slot.entries[0];

                code = entry.code;

                if (code.length() == 5 && code[0] == '"' && code[4] == '"')
                    code = code.substr(1, 3);

                if (code == "AvatarCode" || code == "HUM")
                {
                    name = "Avatar";
                }
                else if (code.length() == 3)
                {
                    if (!Xenoverse2::GetCharaName(code, name, XV2_LANG_ENGLISH, entry.costume_index))
                        name = "????";
                }
                else
                {
                    name = "????";
                }
            }
            else
            {
                name = "";
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::CHARASEL_FULL)
    {
        if (!chara_list)
        {
            DPRINTF("Internal error, chara_list not initialized.\n");
            return;
        }

        ui->listWidget->setSortingEnabled(false);

        for (const CharaListSlot &slot : *chara_list)
        {
            std::string code;

            for (const CharaListSlotEntry& entry : slot.entries)
            {
                std::string name, costume;

                if (1/*code.length() == 0*/)
                {
                    code = entry.code;

                    if (code.length() == 5 && code[0] == '"' && code[4] == '"')
                        code = code.substr(1, 3);
                }

                if (code == "AvatarCode" || code == "HUM")
                {
                    name = "Avatar";
                }
                else if (code.length() == 3)
                {
                    if (!Xenoverse2::GetCharaName(code, name, XV2_LANG_ENGLISH, entry.costume_index))
                        name = "????";
                }
                else
                {
                    name = "????";
                }

                if (entry.code == "AvatarCode" || entry.code == "HUM")
                {
                    costume = Utils::ToString(entry.costume_index);
                }
                else if (entry.code.length() == 5 && entry.code[0] == '"' && entry.code[4] == '"')
                {
                    std::string code = entry.code.substr(1, 3);

                    if (!Xenoverse2::GetCharaCostumeName(code, entry.costume_index, entry.model_preset, costume))
                        costume = "????";
                }
                else
                {
                    costume = "????";
                }

                ui->listWidget->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(costume, false));
            }
        }
    }
    else if (mode == ListMode::CMS)
    {
        CmsFile *cms = (extra) ? (CmsFile *)extra : game_cms;
        int n = 0;

        ui->listWidget->setSortingEnabled(false);

        for (CmsEntry *entry : *cms)
        {
            CmsEntryXV2 *xv2_entry = dynamic_cast<CmsEntryXV2 *>(entry);
            if (!xv2_entry)
            {
                DPRINTF("%s: Internal error (CmsEntryXV2 cast).\n", FUNCNAME);
                exit(-1);
            }

            ui->listWidget->addItem(Utils::StdStringToQString(xv2_entry->name));
            ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(xv2_entry->id));
            n++;
        }

        ui->listWidget->setSortingEnabled(true);
        ui->listWidget->sortItems();
    }
    else if (mode == ListMode::CUS)
    {
        CusFile *cus = (extra) ? (CusFile *)extra : game_cus;

        ui->listWidget->setSortingEnabled(false);

        for (const CusSkillSet &entry : cus->GetSkillSets())
        {
            std::string code, chara_name, costume_name;

            chara_name = Utils::ToString(entry.char_id);
            costume_name = Utils::ToString(entry.costume_id);

            if (Xenoverse2::GetCharaCodeFromId(entry.char_id, code))
            {
                if (Xenoverse2::GetCharaName(code, chara_name, XV2_LANG_ENGLISH, entry.costume_id))
                {
                    Xenoverse2::GetCharaCostumeName(code, entry.costume_id, entry.model_preset, costume_name);
                }
                else
                {
                    chara_name = code;
                }
            }

            ui->listWidget->addItem(Utils::StdStringToQString(chara_name + " - " + costume_name, false));
        }
    }
    else if (mode == ListMode::CSO)
    {
        CsoFile *cso = (extra) ? (CsoFile *)extra : game_cso;

        ui->listWidget->setSortingEnabled(false);

        for (const CsoEntry &entry : *cso)
        {
            std::string code, chara_name, costume_name;

            chara_name = Utils::ToString(entry.char_id);
            costume_name = Utils::ToString(entry.costume_id);

            if (Xenoverse2::GetCharaCodeFromId(entry.char_id, code))
            {
                if (Xenoverse2::GetCharaName(code, chara_name, XV2_LANG_ENGLISH, entry.costume_id))
                {
                    Xenoverse2::GetCharaCostumeName(code, entry.costume_id, 0, costume_name);
                }
                else
                {
                    chara_name = code;
                }
            }

            ui->listWidget->addItem(Utils::StdStringToQString(chara_name + " - " + costume_name, false));
        }
    }
    else if (mode == ListMode::PSC)
    {
        PscFile *psc = (extra) ? (PscFile *)extra : game_psc;
        const std::vector<PscEntry> &psc_entries = psc->GetEntries((size_t)flags);

        ui->listWidget->setSortingEnabled(false);

        for (const PscEntry &entry : psc_entries)
        {
            for (const PscSpecEntry &spec : entry.specs)
            {
                std::string code, chara_name, costume_name;

                chara_name = Utils::ToString(entry.char_id);
                costume_name = Utils::ToString(spec.costume_id);

                if (Xenoverse2::GetCharaCodeFromId(entry.char_id, code))
                {
                    if (Xenoverse2::GetCharaName(code, chara_name, XV2_LANG_ENGLISH, spec.costume_id))
                    {
                        Xenoverse2::GetCharaCostumeName(code, spec.costume_id, 0, costume_name);
                    }
                    else
                    {
                        chara_name = code;
                    }
                }

                ui->listWidget->addItem(Utils::StdStringToQString(chara_name + " - " + costume_name, false));
            }
        }
    }
    else if (mode == ListMode::AUR)
    {
        AurFile *aur = (extra) ? (AurFile *)extra : game_aur;
        const std::vector<AurCharaLink> &links = aur->GetCharaLinks();

        ui->listWidget->setSortingEnabled(false);

        for (const AurCharaLink &entry : links)
        {
            std::string code, chara_name, costume_name;

            chara_name = Utils::ToString(entry.char_id);
            costume_name = Utils::ToString(entry.costume_id);

            if (Xenoverse2::GetCharaCodeFromId(entry.char_id, code))
            {
                if (Xenoverse2::GetCharaName(code, chara_name, XV2_LANG_ENGLISH, entry.costume_id))
                {
                    Xenoverse2::GetCharaCostumeName(code, entry.costume_id, 0, costume_name);
                }
                else
                {
                    chara_name = code;
                }
            }

            ui->listWidget->addItem(Utils::StdStringToQString(chara_name + " - " + costume_name, false));
        }
    }
    else if (mode == ListMode::SEV)
    {
        SevFile *sev;
        bool include_mods;

        if (extra)
        {
           sev = (SevFile *)extra;
           include_mods = true;
        }
        else
        {
            sev = game_sev;
            include_mods = false;
        }

        ui->listWidget->setSortingEnabled(false);

        for (const SevEntry &entry : *sev)
        {
            if (!include_mods && entry.char_id >= XV2_FREE_ID_SEARCH_START)
                continue;

            CmsEntry *cms_entry = game_cms->FindEntryByID(entry.char_id);

            if (!cms_entry || cms_entry->name.length() != 3)
                continue;

            std::string name, costume_name;

            if (!Xenoverse2::GetCharaName(cms_entry->name, name, XV2_LANG_ENGLISH, (entry.costume_id == 0xFFFFFFFF) ? 0 : entry.costume_id))
                name = cms_entry->name;
            else
                name = cms_entry->name + " - " + name;

            if (entry.costume_id == 0xFFFFFFFF)
            {
                costume_name = "Any";
            }
            else
            {
               if (!Xenoverse2::GetCharaCostumeName(cms_entry->name, entry.costume_id, 0, costume_name))
                   costume_name = Utils::ToString(entry.costume_id);
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name + " - " + costume_name, false));
        }
    }
    else if (mode == ListMode::CML)
    {
        CmlFile *cml = (extra) ? (CmlFile *)extra : game_cml;

        ui->listWidget->setSortingEnabled(false);

        for (const CmlEntry &entry : *cml)
        {
            if (entry.char_id == 0xFFFF)
            {
                ui->listWidget->addItem("NONE");

            }
            else
            {
                std::string code, chara_name, costume_name;

                chara_name = Utils::ToString(entry.char_id);
                costume_name = Utils::ToString(entry.costume_id);

                if (Xenoverse2::GetCharaCodeFromId(entry.char_id, code))
                {
                    if (Xenoverse2::GetCharaName(code, chara_name, XV2_LANG_ENGLISH, entry.costume_id))
                    {
                        Xenoverse2::GetCharaCostumeName(code, entry.costume_id, 0, costume_name);
                    }
                    else
                    {
                        chara_name = code;
                    }
                }

                ui->listWidget->addItem(Utils::StdStringToQString(chara_name + " - " + costume_name, false));
            }
        }
    }
    else if (mode == ListMode::HCI)
    {
        HciFile *hci = (extra) ? (HciFile *)extra : game_hci;
        std::unordered_set<uint32_t> added;

        for (const HciEntry &entry : *hci)
        {
            if (added.find(entry.char_id) == added.end())
            {
                added.insert(entry.char_id);

                CmsEntry *cms_entry = game_cms->FindEntryByID(entry.char_id);

                if (!cms_entry || cms_entry->name.length() != 3)
                    continue;

                std::string name;

                if (!Xenoverse2::GetCharaName(cms_entry->name, name, XV2_LANG_ENGLISH, 0))
                    name = cms_entry->name;
                else
                    name = cms_entry->name + " - " + name;

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
            }
        }
    }
    else if (mode == ListMode::PAL)
    {
        PalFile *pal = (extra) ? (PalFile *)extra : game_pal;

        ui->listWidget->setSortingEnabled(false);

        for (const PalEntry &entry : *pal)
        {
            std::string name;
            Xenoverse2::GetLobbyName(entry.name_id, name);

            if (name.length() > 0)
            {
                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
            }
            else
            {
                ui->listWidget->addItem(QString("%1").arg((int16_t)entry.id));
            }
        }
    }
    else if (mode == ListMode::CAC_FULL)
    {
        Xv2SavFile *sav = (Xv2SavFile *)extra;

        ui->listWidget->setSortingEnabled(false);

        for (uint32_t i = 0, n = 0; i < XV2_NUM_CAC; i++)
        {
            if (!sav->IsValidCac(i))
                continue;

            for (uint32_t j = 0; j < XV2_NUM_PRESETS; j++)
            {
                const char *name = sav->GetCac(i)->name;

                if (j == 0)
                    ui->listWidget->addItem(QString("%1 - Current Clothing").arg(name));
                else
                    ui->listWidget->addItem(QString("%1 - Preset %2").arg(name).arg(j));

                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant((i*XV2_NUM_PRESETS)+j));
                n++;
            }
        }
    }
    else if (mode == ListMode::CAC_PARTIAL)
    {
        Xv2SavFile *sav = (Xv2SavFile *)extra;

        ui->listWidget->setSortingEnabled(false);

        for (uint32_t i = 0, n= 0; i < XV2_NUM_CAC; i++)
        {
            if (!sav->IsValidCac(i))
                continue;

            ui->listWidget->addItem(sav->GetCac(i)->name);
            ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(i));
            n++;
        }
    }
    else if (mode == ListMode::SKILL)
    {
        CusFile *cus = (extra) ? (CusFile *)extra : game_cus;

        ui->listWidget->setSortingEnabled(false);

        const std::vector<CusSkill> &super_skills = cus->GetSuperSkills();
        const std::vector<CusSkill> &ult_skills = cus->GetUltimateSkills();
        const std::vector<CusSkill> &eva_skills = cus->GetEvasiveSkills();
        const std::vector<CusSkill> &blast_skills = cus->GetBlastSkills();
        const std::vector<CusSkill> &awaken_skills = cus->GetAwakenSkills();

        int n = 0;
        std::string name;

        if (flags & SKILL_FLAG_SUPER)
        {
            for (const CusSkill &skill : super_skills)
            {
                name = "???";

                Xenoverse2::GetSuperSkillName(skill.id2, name);                
                name += " [SUPER]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.id));
                n++;
            }
        }

        if (flags & SKILL_FLAG_ULTIMATE)
        {
            for (const CusSkill &skill : ult_skills)
            {
                name = "???";

                Xenoverse2::GetUltimateSkillName(skill.id2, name);
                name += " [ULTIMATE]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.id));
                n++;
            }
        }

        if (flags & SKILL_FLAG_EVASIVE)
        {
            for (const CusSkill &skill : eva_skills)
            {
                name = "???";

                Xenoverse2::GetEvasiveSkillName(skill.id2, name);
                name += " [EVASIVE]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.id));
                n++;
            }
        }

        if (flags & SKILL_FLAG_BLAST)
        {
            for (const CusSkill &skill : blast_skills)
            {
                name = skill.name + " - " + Utils::ToString(skill.id);
                name += " [BLAST]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.id));
                n++;
            }
        }

        if (flags & SKILL_FLAG_AWAKEN)
        {
            for (const CusSkill &skill : awaken_skills)
            {
                name = "???";

                Xenoverse2::GetAwakenSkillName(skill.id2, name);

                name += " - " + skill.name;
                name += " - " + Utils::ToString(skill.id);
                name += " [AWAKEN]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.id));
                n++;
            }
        }
    }
    else if (mode == ListMode::SKILL_IDB)
    {
        IdbFile *idb = (extra) ? (IdbFile *)extra : game_skill_idb;
        ui->listWidget->setSortingEnabled(false);

        for (const IdbEntry &entry : *idb)
        {
            std::string name;

            if (entry.type == CUS_SKILL_TYPE_SUPER)
            {
                if (!Xenoverse2::GetSuperSkillName(entry.id, name))
                    name = Utils::ToString(entry.id);

                name += " [SUPER]";
            }
            else if (entry.type == CUS_SKILL_TYPE_ULTIMATE)
            {
                if (!Xenoverse2::GetUltimateSkillName(entry.id, name))
                    name = Utils::ToString(entry.id);

                name += " [ULTIMATE]";
            }
            else if (entry.type == CUS_SKILL_TYPE_EVASIVE)
            {
                if (!Xenoverse2::GetEvasiveSkillName(entry.id, name))
                    name = Utils::ToString(entry.id);

                name += " [EVASIVE]";
            }
            else if (entry.type == CUS_SKILL_TYPE_BLAST)
            {
                const CusSkill *skill = game_cus->FindBlastSkillByID(CUS_BLAST_START+entry.id);
                if (skill)
                {
                    name = skill->name;
                }
                else
                {
                    name = Utils::ToString(entry.id);
                }

                name += " [BLAST]";
            }
            else if (entry.type == CUS_SKILL_TYPE_AWAKEN)
            {
                if (!Xenoverse2::GetAwakenSkillName(entry.id, name))
                    name = Utils::ToString(entry.id);

                const CusSkill *skill = game_cus->FindBlastSkillByID(CUS_AWAKEN_START+entry.id);
                if (skill)
                {
                    name += " - " + skill->name;
                }

                name += " [AWAKEN]";
            }
            else
            {
                name = Utils::ToString(entry.id) + " [Type: " + Utils::ToString(entry.type) + "]";
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::PUP)
    {
        PupFile *pup = (extra) ? (PupFile *)extra : game_pup;

        ui->listWidget->setSortingEnabled(false);

        for (const PupEntry &entry : *pup)
        {
            QString text = QString("Entry 0x%1 (%2)").arg(entry.id, 0, 16).arg(entry.id);
            ui->listWidget->addItem(text);
        }
    }
    else if (mode == ListMode::PUP_SKILL)
    {
        CusFile *cus = (extra) ? (CusFile *)extra : game_cus;
        int n = 0;

        ui->listWidget->setSortingEnabled(false);

        const std::vector<CusSkill> &super_skills = cus->GetSuperSkills();
        for (const CusSkill &skill : super_skills)
        {
            if (skill.pup_id != 0xFFFF && game_pup->FindEntryByID(skill.pup_id))
            {
                std::string name;

                if (!Xenoverse2::GetSuperSkillName(skill.id2, name))
                    name = Utils::ToString(skill.id);

                name += " [SUPER]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.pup_id));
                n++;
            }
        }

        const std::vector<CusSkill> &ultimate_skills = cus->GetUltimateSkills();
        for (const CusSkill &skill : ultimate_skills)
        {
            if (skill.pup_id != 0xFFFF && game_pup->FindEntryByID(skill.pup_id))
            {
                std::string name;

                if (!Xenoverse2::GetUltimateSkillName(skill.id2, name))
                    name = Utils::ToString(skill.id);

                name += " [ULTIMATE]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.pup_id));
                n++;
            }
        }

        const std::vector<CusSkill> &evasive_skills = cus->GetEvasiveSkills();
        for (const CusSkill &skill : evasive_skills)
        {
            if (skill.pup_id != 0xFFFF && game_pup->FindEntryByID(skill.pup_id))
            {
                std::string name;

                if (!Xenoverse2::GetEvasiveSkillName(skill.id2, name))
                    name = Utils::ToString(skill.id);

                name += " [EVASIVE]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.pup_id));
                n++;
            }
        }

        const std::vector<CusSkill> &blast_skills = cus->GetBlastSkills();
        for (const CusSkill &skill : blast_skills)
        {
            if (skill.pup_id != 0xFFFF && game_pup->FindEntryByID(skill.pup_id))
            {
                std::string name = skill.name + " - " + Utils::ToString(skill.id);
                name += " [BLAST]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(skill.pup_id));
                n++;
            }
        }

        const std::vector<CusSkill> &awaken_skills = cus->GetAwakenSkills();
        for (const CusSkill &skill : awaken_skills)
        {
            if (skill.pup_id != 0xFFFF && game_pup->FindEntryByID(skill.pup_id))
            {
                uint16_t num_trans = skill.num_transforms;
                if (num_trans == 0xFFFF)
                    num_trans = 1;

                for (uint16_t i = 0; i < num_trans; i++)
                {
                    uint16_t pup_id = skill.pup_id + i;

                    if (!game_pup->FindEntryByID(pup_id))
                        break;

                    std::string name;

                    if (Xenoverse2::GetAwakenSkillName(skill.id2, name))
                        name += " - ";

                    name += skill.name + " - " + Utils::ToString(skill.id);
                    name += " [AWAKEN]";

                    if (num_trans > 1)
                    {
                        name += " - Stage " + Utils::ToString(i+1);
                    }

                    ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                    ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(pup_id));
                    n++;
                }
            }
        }
    }
    else if (mode == ListMode::SKILL_TRANS)
    {
        CusFile *cus = (extra) ? (CusFile *)extra : game_cus;
        int n = 0;

        ui->listWidget->setSortingEnabled(false);

        const std::vector<CusSkill> &awaken_skills = cus->GetAwakenSkills();
        for (const CusSkill &skill : awaken_skills)
        {
            for (unsigned int i = 0; ; i++)
            {
                std::string name;

                if (!Xenoverse2::GetBtlHudAwakenName(skill.id2, i, name))
                    break;

                name += " - " + skill.name + " - " + Utils::ToString(skill.id) + " - " + "[Trans " + Utils::ToString(i) + "]";

                uint32_t data = ((uint32_t)skill.id2 << 16) | i;

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(data));
                n++;
            }
        }
    }
    else if (mode == ListMode::AURA)
    {
        AurFile *aur = (extra) ? (AurFile *)extra : game_aur;

        ui->listWidget->setSortingEnabled(false);

        for (const AurAura &aura : (*aur).GetAuras())
        {
            QString text = QString("Entry 0x%1 (%2)").arg(aura.id, 0, 16).arg(aura.id);
            ui->listWidget->addItem(text);
        }
    }
    else if (mode == ListMode::AURA_SKILL)
    {
        CusFile *cus = (extra) ? (CusFile *)extra : game_cus;
        int n = 0;
        bool use_cus_aura = false;

        if (flags & FLAG_USE_CUS_AURA)
            use_cus_aura = true;

        ui->listWidget->setSortingEnabled(false);

        const std::vector<CusSkill> &super_skills = cus->GetSuperSkills();
        for (const CusSkill &skill : super_skills)
        {
            uint32_t aura = (use_cus_aura) ? skill.aura : Xenoverse2::CusAuraToAurAura(skill.aura);
            bool cond = (use_cus_aura) ? (aura <= BEHAVIOUR_MAX || game_prebaked->FindAuraData(aura)) : (aura != 0xFFFFFFFF && game_aur->FindAuraByID(aura));

            if (cond)
            {
                std::string name;

                if (!Xenoverse2::GetSuperSkillName(skill.id2, name))
                    name = Utils::ToString(skill.id);

                name += " [SUPER]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(aura));
                n++;
            }
        }

        const std::vector<CusSkill> &ultimate_skills = cus->GetUltimateSkills();
        for (const CusSkill &skill : ultimate_skills)
        {
            uint32_t aura = (use_cus_aura) ? skill.aura : Xenoverse2::CusAuraToAurAura(skill.aura);
            bool cond = (use_cus_aura) ? (aura <= BEHAVIOUR_MAX || game_prebaked->FindAuraData(aura)) : (aura != 0xFFFFFFFF && game_aur->FindAuraByID(aura));

            if (cond)
            {
                std::string name;

                if (!Xenoverse2::GetUltimateSkillName(skill.id2, name))
                    name = Utils::ToString(skill.id);

                name += " [ULTIMATE]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(aura));
                n++;
            }
        }

        const std::vector<CusSkill> &evasive_skills = cus->GetEvasiveSkills();
        for (const CusSkill &skill : evasive_skills)
        {
            uint32_t aura = (use_cus_aura) ? skill.aura : Xenoverse2::CusAuraToAurAura(skill.aura);
            bool cond = (use_cus_aura) ? (aura <= BEHAVIOUR_MAX || game_prebaked->FindAuraData(aura)) : (aura != 0xFFFFFFFF && game_aur->FindAuraByID(aura));

            if (cond)
            {
                std::string name;

                if (!Xenoverse2::GetEvasiveSkillName(skill.id2, name))
                    name = Utils::ToString(skill.id);

                name += " [EVASIVE]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(aura));
                n++;
            }
        }

        const std::vector<CusSkill> &blast_skills = cus->GetBlastSkills();
        for (const CusSkill &skill : blast_skills)
        {
            uint32_t aura = (use_cus_aura) ? skill.aura : Xenoverse2::CusAuraToAurAura(skill.aura);
            bool cond = (use_cus_aura) ? (aura <= BEHAVIOUR_MAX || game_prebaked->FindAuraData(aura)) : (aura != 0xFFFFFFFF && game_aur->FindAuraByID(aura));

            if (cond)
            {
                std::string name = skill.name + " - " + Utils::ToString(skill.id);
                name += " [BLAST]";

                ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(aura));
                n++;
            }
        }

        const std::vector<CusSkill> &awaken_skills = cus->GetAwakenSkills();
        for (const CusSkill &skill : awaken_skills)
        {
            uint32_t aura = (use_cus_aura) ? skill.aura : Xenoverse2::CusAuraToAurAura(skill.aura);
            bool cond = (use_cus_aura) ? (aura <= BEHAVIOUR_MAX || game_prebaked->FindAuraData(aura)) : (aura != 0xFFFFFFFF && game_aur->FindAuraByID(aura));

            if (cond)
            {
                uint16_t num_trans = skill.num_transforms;
                if (num_trans == 0xFFFF)
                    num_trans = 1;

                for (uint16_t i = 0; i < num_trans; i++)
                {
                    uint32_t this_aura = (use_cus_aura) ? skill.aura+i : Xenoverse2::CusAuraToAurAura(skill.aura+i);
                    bool cond = (use_cus_aura) ? (this_aura <= BEHAVIOUR_MAX || game_prebaked->FindAuraData(this_aura)) : (this_aura != 0xFFFFFFFF && game_aur->FindAuraByID(this_aura));

                    if (!cond)
                        break;

                    std::string name;

                    if (Xenoverse2::GetAwakenSkillName(skill.id2, name))
                        name += " - ";

                    name += skill.name + " - " + Utils::ToString(skill.id);
                    name += " [AWAKEN]";

                    if (num_trans > 1)
                    {
                        name += " - Stage " + Utils::ToString(i+1);
                    }

                    ui->listWidget->addItem(Utils::StdStringToQString(name, false));
                    ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(this_aura));
                    n++;
                }
            }
        }
    }
    else if (mode == ListMode::TALISMAN)
    {
        IdbFile *idb = (extra) ? (IdbFile *)extra : game_talisman_idb;
        ui->listWidget->setSortingEnabled(false);

        for (const IdbEntry &entry : *idb)
        {
            std::string name = Xenoverse2::GetTalismanNameEx(entry.id);

            if (name.length() == 0)
                name = Utils::ToString(entry.id);

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::BCS)
    {
        BcsFile *bcs = (BcsFile *)extra;
        int race_lock = flags;

        ui->listWidget->setSortingEnabled(false);

        const std::vector<BcsPartSet> &sets = bcs->GetPartSets();
        for (size_t i = 0; i < sets.size(); i++)
        {
            QString entry_text = QString("%1").arg(i);

            if (!sets[i].valid)
            {
                entry_text += " [EMPTY]";
            }

            if (race_lock != 0)
            {
                std::string name = Xenoverse2::GetCostumeNamesForPartSet2((uint32_t)i, race_lock);

                if (name.length() != 0)
                    entry_text += " - " + Utils::StdStringToQString(name, false);
            }

            ui->listWidget->addItem(entry_text);
        }
    }
    else if (mode == ListMode::COSTUME_IDB)
    {
        IdbFile *idb = (IdbFile *)extra;
        ui->listWidget->setSortingEnabled(false);

        for (const IdbEntry &entry : *idb)
        {
            std::string name;

            if (!Xenoverse2::GetCacCostumeName(entry.name_id, name))
                name = Utils::ToString(entry.id);

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::ACCESSORY_IDB)
    {
        IdbFile *idb = (extra) ? (IdbFile *)extra : game_accesory_idb;
        ui->listWidget->setSortingEnabled(false);

        for (const IdbEntry &entry : *idb)
        {
            std::string name;

            if (!Xenoverse2::GetAccesoryName(entry.name_id, name))
                name = Utils::ToString(entry.id);

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::STAGE)
    {
        ui->listWidget->setSortingEnabled(false);

        for (size_t i = 0; i < game_stage_def->GetNumStages(); i++)
        {
            std::string name = (*game_stage_def)[i].code;
            std::string st_name;

            if (Xenoverse2::GetStageName(name, st_name))
            {
                name += " - " + st_name;
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::STAGE_LIGHTING)
    {
        xv2fs->VisitDirectory("data/lighting/environment/", true, false, false, StageLightingVisitor, this);
    }
    else if (mode == ListMode::STAGE_SS)
    {
        ui->listWidget->setSortingEnabled(false);

        for (size_t i = 0; i < game_stage_def->GetNumSsStages(); i++)
        {
            Xv2Stage *stage = game_stage_def->GetStageBySsid((uint8_t)i);
            std::string name = stage->code;
            std::string st_name;

            if (Xenoverse2::GetStageName(name, st_name))
            {
                name += " - " + st_name;
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
    else if (mode == ListMode::X2M_SLOT)
    {
        ui->listWidget->setSortingEnabled(false);

        X2mFile *x2m = (X2mFile *)extra;

        if (x2m->GetType() != X2mType::NEW_CHARACTER)
        {
            ui->listWidget->addItem("Dummy");
            return;
        }

        for (size_t i = 0; i < x2m->GetNumSlotEntries(); i++)
        {
            const X2mSlotEntry &slot_entry = x2m->GetSlotEntry(i);
            ui->listWidget->addItem(Utils::StdStringToQString(slot_entry.costume_name[XV2_LANG_ENGLISH], false));
        }
    }
    else if (mode == ListMode::TTB)
    {
        TtbFile *ttb;
        bool include_mods;

        if (extra)
        {
           ttb = (TtbFile *)extra;
           include_mods = true;
        }
        else
        {
            ttb = game_ttb;
            include_mods = false;
        }

        struct CharCostumeDef
        {
            std::string code;
            int costume;

            inline bool operator < (const CharCostumeDef &other) const
            {
                if (code == other.code)
                    return (costume < other.costume);

                return (code.compare(other.code) < 0);
            }
        };

        std::set<CharCostumeDef> char_set;

        for (const TtbEntry &entry : *ttb)
        {
            CharCostumeDef cd;

            if (!include_mods && entry.cms_id >= XV2_FREE_ID_SEARCH_START)
                continue;

            CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(entry.cms_id));

            if (cms)
            {
                cd.code = cms->name;

                for (const TtbEvent &event : entry.events)
                {
                    cd.costume = (int)event.costume;
                    char_set.insert(cd);
                }
            }

            for (const TtbEvent &event : entry.events)
            {
                if (include_mods || event.cms2_id < XV2_FREE_ID_SEARCH_START)
                {
                    cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms2_id));

                    if (cms)
                    {
                        cd.code = cms->name;
                        cd.costume = (int)event.costume2;
                        char_set.insert(cd);
                    }
                }

                if (include_mods || event.cms3_id < XV2_FREE_ID_SEARCH_START)
                {
                    cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms3_id));

                    if (cms)
                    {
                        cd.code = cms->name;
                        cd.costume = (int)event.costume3;
                        char_set.insert(cd);
                    }
                }

                if (include_mods || event.cms4_id < XV2_FREE_ID_SEARCH_START)
                {
                    cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms4_id));

                    if (cms)
                    {
                        cd.code = cms->name;
                        cd.costume = (int)event.costume4;
                        char_set.insert(cd);
                    }
                }

                if (include_mods || event.cms5_id < XV2_FREE_ID_SEARCH_START)
                {
                    cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms5_id));

                    if (cms)
                    {
                        cd.code = cms->name;
                        cd.costume = (int)event.costume5;
                        char_set.insert(cd);
                    }
                }
            }
        }

        int n = 0;
        ui->listWidget->setSortingEnabled(false);

        for (const CharCostumeDef &def : char_set)
        {
            std::string name, costume_name;

            if (!Xenoverse2::GetCharaName(def.code, name, XV2_LANG_ENGLISH, (def.costume == -1) ? 0 : def.costume))
                name = def.code;
            else
                name = def.code + " - " + name;

            if (def.costume == -1)
            {
                costume_name = "Any";
            }
            else
            {
               if (!Xenoverse2::GetCharaCostumeName(def.code, def.costume, 0, costume_name))
                   costume_name = Utils::ToString(def.costume);
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(costume_name, false));
            ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(Utils::StdStringToQString(def.code) + QString("%1").arg((uint32_t)def.costume)));
            n++;
        }
    }

    else if (mode == ListMode::TTC)
    {
        TtcFile *ttc;
        bool include_mods;

        if (extra)
        {
           ttc = (TtcFile *)extra;
           include_mods = true;
        }
        else
        {
            ttc = game_ttc;
            include_mods = false;
        }

        struct CharCostumeDef
        {
            std::string code;
            int costume;

            inline bool operator < (const CharCostumeDef &other) const
            {
                if (code == other.code)
                    return (costume < other.costume);

                return (code.compare(other.code) < 0);
            }
        };

        std::set<CharCostumeDef> char_set;

        for (const TtcEntry &entry : *ttc)
        {
            CharCostumeDef cd;

            if (!include_mods && entry.cms_id >= XV2_FREE_ID_SEARCH_START)
                continue;

            CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(entry.cms_id));

            if (cms)
            {
                cd.code = cms->name;

                for (const TtcEventList &list : entry.lists)
                {
                    for (const TtcEvent &event : list.events)
                    {
                        cd.costume = (int)event.costume;
                        char_set.insert(cd);
                    }
                }
            }
        }

        int n = 0;
        ui->listWidget->setSortingEnabled(false);

        for (const CharCostumeDef &def : char_set)
        {
            std::string name, costume_name;

            if (!Xenoverse2::GetCharaName(def.code, name, XV2_LANG_ENGLISH, (def.costume == -1) ? 0 : def.costume))
                name = def.code;
            else
                name = def.code + " - " + name;

            if (def.costume == -1)
            {
                costume_name = "Any";
            }
            else
            {
               if (!Xenoverse2::GetCharaCostumeName(def.code, def.costume, 0, costume_name))
                   costume_name = Utils::ToString(def.costume);
            }

            ui->listWidget->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(costume_name, false));
            ui->listWidget->item(n)->setData(Qt::UserRole, QVariant(Utils::StdStringToQString(def.code) + QString("%1").arg((uint32_t)def.costume)));
            n++;
        }
    }
    else if (mode == ListMode::TTC_FILES)
    {
        xv2fs->VisitDirectory("data/msg/", true, false, false, TtcLlFilesVisitor, this);
    }
    else if (mode == ListMode::CNC)
    {
        CncFile *cnc = (extra) ? (CncFile *)extra : game_cnc;
        ui->listWidget->setSortingEnabled(false);

        for (const CncEntry &entry : *cnc)
        {
            std::string name = Xenoverse2::GetCharaAndCostumeName(entry.cms_id, entry.costume_index, entry.model_preset, false);
            ui->listWidget->addItem(Utils::StdStringToQString(name, false));
        }
    }
}

void ListDialog::on_listWidget_itemActivated(QListWidgetItem *item)
{
    result = item->text();
    result_integer = ui->listWidget->currentIndex().row();
    result_data = ui->listWidget->item(result_integer)->data(Qt::UserRole).toInt();
    result_data_str = ui->listWidget->item(result_integer)->data(Qt::UserRole).toString();
    accept();
}

void ListDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> list = ui->listWidget->selectedItems();

    if (list.size() != 1)
        return;

    result = list[0]->text();
    result_integer = ui->listWidget->currentIndex().row();
    result_data = ui->listWidget->item(result_integer)->data(Qt::UserRole).toInt();
    result_data_str = ui->listWidget->item(result_integer)->data(Qt::UserRole).toString();
}


