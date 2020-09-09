#include <QString>
#include <QFile>

#include "Xenoverse2.h"
#include "Config.h"
#include "xv2ins_common.h"
#include "CharaSeleIggyBlob.h"
#include "stageseleiggyblob.h"

//#define MY_CHARALIST_OLD        "XV2INS\\CharaListV6.as"
//#define MY_CHARALIST_NEW        "data/XV2P_SLOTS.x2s"
#define CHARASELE_IGGY_PATH     "data/ui/iggy/CHARASELE.iggy"
#define STAGESELE_IGGY_PATH     "data/ui/iggy/STAGESELE.iggy"
//#define CHASEL_PATH "Internal/CharaSele"

#define GAME_CST_FILE   "data/system/chara_select_table.cst"

static bool IsOldStylePsc()
{
    size_t size;

    uint8_t *buf = xv2fs->ReadFile("data/system/parameter_spec_char.psc", &size);
    if (!buf)
        return false;

    bool ret = false;

    if (size >= sizeof(PSCHeader))
    {
        PSCHeader *hdr = (PSCHeader *)buf;
        if (hdr->header_size < sizeof(PSCHeader))
            ret = true;
    }

    delete[] buf;
    return ret;
}

static bool NeedsUpdate()
{
    return IsOldStylePsc();
}

static uint8_t *ReadResourceFile(const char *path, size_t *size)
{
    QFile file(path);

     if (!file.open(QFile::ReadOnly))
         return nullptr;

     uint8_t *buf = new uint8_t[file.size()];
     if (!buf)
     {
         DPRINTF("%s: Memory allocation error: %x\n", FUNCNAME, (unsigned int)file.size());
         file.close();
         return nullptr;
     }

     if (file.read((char *)buf, file.size()) != file.size())
     {
         delete[] buf;
         file.close();
         return nullptr;
     }

     *size = file.size();
     file.close();

     return buf;
}

#define DIALOGUE_PATH "data/sound/VOX/Quest/Dialogue/"

bool InstallCraftedAcb()
{
    if (!xv2fs->FileExists(std::string(DIALOGUE_PATH) + "CAQD_ADD_VOX.acb", false) || !xv2fs->FileExists(std::string(DIALOGUE_PATH) + "CAQD_ADD_VOX.awb", false))
    {
        size_t size;
        uint8_t *buf = ReadResourceFile(":/crafted_acb/CAQD_ADD_VOX.acb", &size);

        if (!buf)
        {
            DPRINTF("%s: Failed to read internal resource, shouldn't happen.\n", FUNCNAME);
            return false;
        }

        bool ret = xv2fs->WriteFile(std::string(DIALOGUE_PATH) + "CAQD_ADD_VOX.acb", buf, size);
        delete[] buf;
        if (!ret)
        {
            DPRINTF("%s: Failed to write acb file (jp)\n", FUNCNAME);
            return false;
        }

        buf = ReadResourceFile(":/crafted_acb/CAQD_ADD_VOX.awb", &size);

        if (!buf)
        {
            DPRINTF("%s: Failed to read internal resource, shouldn't happen.\n", FUNCNAME);
            return false;
        }

        ret = xv2fs->WriteFile(std::string(DIALOGUE_PATH) + "CAQD_ADD_VOX.awb", buf, size);
        delete[] buf;
        if (!ret)
        {
            DPRINTF("%s: Failed to write awb file (jp)\n", FUNCNAME);
            return false;
        }
    }

    if (!xv2fs->FileExists(std::string(DIALOGUE_PATH) + "en/CAQD_ADD_VOX.acb", false) || !xv2fs->FileExists(std::string(DIALOGUE_PATH) + "en/CAQD_ADD_VOX.awb", false))
    {
        size_t size;
        uint8_t *buf = ReadResourceFile(":/crafted_acb/en/CAQD_ADD_VOX.acb", &size);

        if (!buf)
        {
            DPRINTF("%s: Failed to read internal resource, shouldn't happen.\n", FUNCNAME);
            return false;
        }

        bool ret = xv2fs->WriteFile(std::string(DIALOGUE_PATH) + "en/CAQD_ADD_VOX.acb", buf, size);
        delete[] buf;
        if (!ret)
        {
            DPRINTF("%s: Failed to write acb file (en)\n", FUNCNAME);
            return false;
        }

        buf = ReadResourceFile(":/crafted_acb/en/CAQD_ADD_VOX.awb", &size);

        if (!buf)
        {
            DPRINTF("%s: Failed to read internal resource, shouldn't happen.\n", FUNCNAME);
            return false;
        }

        ret = xv2fs->WriteFile(std::string(DIALOGUE_PATH) + "en/CAQD_ADD_VOX.awb", buf, size);
        delete[] buf;
        if (!ret)
        {
            DPRINTF("%s: Failed to write awb file (en)\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

bool Bootstrap(bool multiple_hci, bool installer_mode, bool *needs_update)
{
    if (needs_update)
        *needs_update = false;

    config.Load();

    if (!config.ConfigureRequirements())
    {
        DPRINTF("ConfigureRequirements failed.\n");
        exit(-1);
    }

    Xenoverse2::InitFs(Utils::QStringToStdString(config.game_directory));    

    float version = Utils::GetExeVersion(Utils::MakePathString(Utils::QStringToStdString(config.game_directory), "bin/DBXV2.exe"));
    if (version < MINIMUM_EXE_VERSION_REQUIRED)
    {
        DPRINTF("Version %.3f or greater of the game is required. You have version %.3f.\n", MINIMUM_EXE_VERSION_REQUIRED, version);
        return false;
    }

    if (needs_update)
    {
        *needs_update = NeedsUpdate();

        if (*needs_update)
            return false;
    }

    if (installer_mode && !InstallCraftedAcb())
        return false;

    bool install_iggy = true;

    if (xv2fs->FileExists(CHARASELE_IGGY_PATH, false))
    {
        size_t size;
        uint8_t *buf = xv2fs->ReadFile(CHARASELE_IGGY_PATH, &size);

        if (!buf)
        {
            DPRINTF("%s: Failed to load CHARASELE.iggy.\n", FUNCNAME);
            exit(-1);
        }

        for (size_t i = 0; i < size-strlen(XV2_PATCHER_AS3_TAG_CHARA)-1; i++)
        {
            if (memcmp(buf+i, XV2_PATCHER_AS3_TAG_CHARA, strlen(XV2_PATCHER_AS3_TAG_CHARA)) == 0)
            {
                install_iggy = false;
                break;
            }
        }

        delete[] buf;
    }

    if (install_iggy)
    {
        if (!xv2fs->WriteFile(CHARASELE_IGGY_PATH, charasele_iggy_blob, sizeof(charasele_iggy_blob)))
        {
            DPRINTF("%s: Failed to save CHARASELE.iggy.\n", FUNCNAME);
            exit(-1);
        }
    }

    install_iggy = true;

    if (xv2fs->FileExists(STAGESELE_IGGY_PATH, false))
    {
        size_t size;
        uint8_t *buf = xv2fs->ReadFile(STAGESELE_IGGY_PATH, &size);

        if (!buf)
        {
            DPRINTF("%s: Failed to load STAGESELE.iggy.\n", FUNCNAME);
            exit(-1);
        }

        for (size_t i = 0; i < size-strlen(XV2_PATCHER_AS3_TAG_STAGE)-1; i++)
        {
            if (memcmp(buf+i, XV2_PATCHER_AS3_TAG_STAGE, strlen(XV2_PATCHER_AS3_TAG_STAGE)) == 0)
            {
                install_iggy = false;
                break;
            }
        }

        delete[] buf;
    }

    if (install_iggy)
    {
        if (!xv2fs->WriteFile(STAGESELE_IGGY_PATH, stagesele_iggy_blob, sizeof(stagesele_iggy_blob)))
        {
            DPRINTF("%s: Failed to save STAGESELE.iggy.\n", FUNCNAME);
            exit(-1);
        }
    }

    //if (!Xenoverse2::InitCharaList(CHASEL_PATH, my_charalist))
    if (!Xenoverse2::InitCharaList())
    {
        DPRINTF("InitCharaList failed.\n");
        exit(-1);
    }

    /*if (!config.prb_fix_applied)
    {
        config.prb_fix_applied = true;
        config.Save();

        size_t num_slots = chara_list->GetNumSlots();
        if ((*chara_list)[num_slots-1].entries[0].code != "MRN")
        {
            // Zamasu damaged
            {
                CharaListSlot c_slot;
                CharaListSlotEntry s_entry;

                s_entry.costume_index = 0;
                s_entry.code = "ZMD";
                s_entry.code = "\"" + s_entry.code + "\"";
                s_entry.model_preset = 1;
                s_entry.flag_gk2 = false;
                s_entry.unlock_index = 0;
                s_entry.voices_id_list[0] = 178;
                s_entry.voices_id_list[1] = 179;
                s_entry.dlc = "PRB";

                c_slot.entries.push_back(s_entry);
                chara_list->GetSlots().push_back(c_slot);
            }

            // Demigra
            {
                CharaListSlot c_slot;
                CharaListSlotEntry s_entry;

                s_entry.costume_index = 0;
                s_entry.code = "DMG";
                s_entry.code = "\"" + s_entry.code + "\"";
                s_entry.model_preset = 1;
                s_entry.flag_gk2 = false;
                s_entry.unlock_index = 0;
                s_entry.voices_id_list[0] = 209;
                s_entry.voices_id_list[1] = 210;
                s_entry.dlc = "PRB";

                c_slot.entries.push_back(s_entry);
                chara_list->GetSlots().push_back(c_slot);
            }

            // Demigra
            {
                CharaListSlot c_slot;
                CharaListSlotEntry s_entry;

                s_entry.costume_index = 0;
                s_entry.code = "MRN";
                s_entry.code = "\"" + s_entry.code + "\"";
                s_entry.model_preset = 1;
                s_entry.flag_gk2 = false;
                s_entry.unlock_index = 0;
                s_entry.voices_id_list[0] = 142;
                s_entry.voices_id_list[1] = 143;
                s_entry.dlc = "PRB";

                c_slot.entries.push_back(s_entry);
                chara_list->GetSlots().push_back(c_slot);
            }

            Xenoverse2::CommitCharaList(true, false);
        }
    }*/

    if (!Xenoverse2::InitSystemFiles(false, multiple_hci))
    {
        DPRINTF("InitSystemFiles failed.\n");

        if (!installer_mode)
            exit(-1);

        return false;
    }

    if (!Xenoverse2::InitCharaNames())
    {
        DPRINTF("InitCharaNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCharaCostumeNames())
    {
        DPRINTF("InitCharaCostumeNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitSkillNames())
    {
        DPRINTF("InitSkillNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitSkillDescs())
    {
        DPRINTF("InitSkillDescs failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitBtlHudText())
    {
        DPRINTF("InitBtlHudText failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitSelPort())
    {
        DPRINTF("InitSelPort failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitPreBaked())
    {
        DPRINTF("InitPreBaked failed.\n");
        exit(-1);
    }

    // This is now loaded on demand
    /*if (!Xenoverse2::InitSound(true))
    {
        DPRINTF("InitSound failed.\n");
        exit(-1);
    }*/

    if (!Xenoverse2::InitIdb(true, true, true, true))
    {
        DPRINTF("InitIdb failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitTalismanNames())
    {
        DPRINTF("InitTalismanNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCac())
    {
        DPRINTF("InitCac failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCacCostumeNames())
    {
        DPRINTF("InitCacCostumeNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCacCostumeDescs())
    {
        DPRINTF("InitCacCostumeDescs failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCostumeFile())
    {
        DPRINTF("InitCostumeFile failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitStageSlots())
    {
        DPRINTF("InitStageSlots failed.\n");
        exit(-1);
    }    

    if (!installer_mode)
        return true;    

    return true;
}


