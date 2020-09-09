#include <algorithm>

#include "FileStream.h"
#include "IniFile.h"
#include "half.hpp"
#include "debug.h"

#include "Criware/HcaFile.h"

#include "Dimps/SkeletonFile.h"
#include "Dimps/EmaFile.h"
#include "Dimps/EmbFile.h"
#include "Dimps/EmmFile.h"
#include "Dimps/EmoFile.h"

#include "SSSS/2ryFile.h"
#include "SSSS/BgrFile.h"
#include "SSSS/CscFile.h"
#include "SSSS/CspFile.h"
#include "SSSS/SpmFile.h"
#include "SSSS/TdbFile.h"
#include "SSSS/VpmFile.h"
#include "SSSS/VstFile.h"

#include "DBXV2/Xenoverse2.h"
#include "DBXV2/CmsFile.h"
#include "DBXV2/CusFile.h"
#include "DBXV2/BcsFile.h"
#include "DBXV2/CsoFile.h"
#include "DBXV2/AurFile.h"
#include "DBXV2/CmlFile.h"
#include "DBXV2/BacFile.h"
#include "DBXV2/BsaFile.h"
#include "DBXV2/AitFile.h"
#include "DBXV2/BcmFile.h"
#include "DBXV2/BaiFile.h"
#include "DBXV2/CncFile.h"
#include "DBXV2/QxdFile.h"
#include "DBXV2/PscFile.h"
#include "DBXV2/HciFile.h"
#include "DBXV2/SevFile.h"
#include "DBXV2/PalFile.h"
#include "DBXV2/TnlFile.h"
#include "DBXV2/TsdFile.h"
#include "DBXV2/TtbFile.h"
#include "DBXV2/TtcFile.h"
#include "DBXV2/QmlFile.h"
#include "DBXV2/MsgFile.h"
#include "DBXV2/EskFile.h"
#include "DBXV2/EanFile.h"
#include "DBXV2/ErsFile.h"
#include "DBXV2/Xv2SavFile.h"
#include "DBXV2/PupFile.h"
#include "DBXV2/TsrFile.h"
#include "DBXV2/QslFile.h"
#include "DBXV2/QbtFile.h"
#include "DBXV2/QedFile.h"
#include "DBXV2/QsfFile.h"

#include "Misc/IggyTexFile.h"

enum class FileType
{
    EMO,
    EMB,
    EMM,
    EMA,
    _2RY,
    SKL,
    DDS,
    VPM,
    VST,
    SPM,
    BGR,
    CMS,
    CSC,
    CSP,
    CUS,
    CSO,
    MSG,
    TDB,
    BCS,
    PSC,
    HCI,
    SEV,
    CML,
    PAL,
    IDB,
    PUP,
    BAC,
    AIT,
    BCM,
    AUR,
    BAI,
    TNL,
    TSR,
    TSD,
    TTB,
    TTC,
    CNC,
    UNKNOWN,
};

struct FileTypeAssociation
{
    std::string extension;
    FileType file_type;
    bool is_xml;
};

static const std::vector<FileTypeAssociation> file_associations =
{
    { ".emo", FileType::EMO, false },
    { ".emb", FileType::EMB, false },
    { ".emm", FileType::EMM, false },
    { ".ema", FileType::EMA, false },
    { ".2ry", FileType::_2RY, false },
    { ".skl", FileType::SKL, false },
    { ".vpm", FileType::VPM, false },
    { ".vst", FileType::VST, false },
    { ".spm", FileType::SPM, false },
    { ".bgr", FileType::BGR, false },
    { ".cms", FileType::CMS, false },
    { ".csc", FileType::CSC, false },
    { ".csp", FileType::CSP, false },
    { ".cus", FileType::CUS, false },
    { ".cso", FileType::CSO, false },
    { ".msg", FileType::MSG, false },
    { ".spm", FileType::SPM, false },
    { ".tdb", FileType::TDB, false },
    { ".bcs", FileType::BCS, false },
    { ".psc", FileType::PSC, false },
    { ".hci", FileType::HCI, false },
    { ".sev", FileType::SEV, false },
    { ".cml", FileType::CML, false },
    { ".pal", FileType::PAL, false },
    { ".idb", FileType::IDB, false },
    { ".pup", FileType::PUP, false },
    { ".bac", FileType::BAC, false },
    { ".ait", FileType::AIT, false },
    { ".bcm", FileType::BCM, false },
    { ".aur", FileType::AUR, false },
    { ".bai", FileType::BAI, false },
    { ".tnl", FileType::TNL, false },
    { ".tsr", FileType::TSR, false },
    { ".tsd", FileType::TSD, false },
    { ".ttb", FileType::TTB, false },
    { ".ttc", FileType::TTC, false },
    { ".cnc", FileType::CNC, false },

    { ".emo.xml", FileType::EMO, true },
    { ".emb.xml", FileType::EMB, true },
    { ".emm.xml", FileType::EMM, true },
    { ".ema.xml", FileType::EMA, true },
    { ".2ry.xml", FileType::_2RY, true },
    { ".skl.xml", FileType::SKL, true },
    { ".vpm.xml", FileType::VPM, true },
    { ".vst.xml", FileType::VST, true },
    { ".spm.xml", FileType::SPM, true },
    { ".bgr.xml", FileType::BGR, true },
    { ".cms.xml", FileType::CMS, true },
    { ".csc.xml", FileType::CSC, true },
    { ".csp.xml", FileType::CSP, true },
    { ".cus.xml", FileType::CUS, true },
    { ".cso.xml", FileType::CSO, true },
    { ".msg.xml", FileType::MSG, true },
    { ".spm.xml", FileType::SPM, true },
    { ".tdb.xml", FileType::TDB, true },
    { ".bcs.xml", FileType::BCS, true },
    { ".psc.xml", FileType::PSC, true },
    { ".hci.xml", FileType::HCI, true },
    { ".sev.xml", FileType::SEV, true },
    { ".cml.xml", FileType::CML, true },
    { ".pal.xml", FileType::PAL, true },
    { ".idb.xml", FileType::IDB, true },
    { ".pup.xml", FileType::PUP, true },
    { ".bac.xml", FileType::BAC, true },
    { ".ait.xml", FileType::AIT, true },
    { ".bcm.xml", FileType::BCM, true },
    { ".aur.xml", FileType::AUR, true },
    { ".bai.xml", FileType::BAI, true },
    { ".tnl.xml", FileType::TNL, true },
    { ".tsr.xml", FileType::TSR, true },
    { ".tsd.xml", FileType::TSD, true },
    { ".ttb.xml", FileType::TTB, true },
    { ".ttc.xml", FileType::TTC, true },
    { ".cnc.xml", FileType::CNC, true },
};

FileType get_file_type(const std::string &file_path, bool *is_xml=nullptr)
{
    for (const FileTypeAssociation &fta : file_associations)
    {
        if (Utils::EndsWith(file_path, fta.extension, false))
        {
            if (is_xml)
                *is_xml = fta.is_xml;

            return fta.file_type;
        }
    }

    return FileType::UNKNOWN;
}

void Xenoverse2Setup(bool idb, bool init_lobby, bool init_common_dialogue)
{
    IniFile ini;
    std::string xv2_path;

    if (!ini.LoadFromFile(Utils::GetAppDataPath("XV2INS/xv2ins.ini"), false))
        return;

    if (!ini.GetStringValue("General", "game_directory", xv2_path))
        return;

    if (xv2_path.length() == 0)
        return;

    Xenoverse2::InitFs(xv2_path);

    if (!Xenoverse2::InitSystemFiles(true))
        return;

    if (!Xenoverse2::InitCharaNames(XV2_LANG_ENGLISH))
        return;

    if (!Xenoverse2::InitCharaCostumeNames(XV2_LANG_ENGLISH))
        return;

    if (!Xenoverse2::InitSkillNames(XV2_LANG_ENGLISH))
        return;

    if (idb)
    {
        if (!Xenoverse2::InitIdb())
            return;

        if (!Xenoverse2::InitLobbyText())
            return;

        if (!Xenoverse2::InitCacCostumeNames())
            return;

        if (!Xenoverse2::InitTalismanNames())
            return;

        if (!Xenoverse2::InitMaterialNames())
            return;

        if (!Xenoverse2::InitBattleNames())
            return;
    }

    if (init_lobby)
    {
        if (!Xenoverse2::InitLobby(true))
            return;
    }

    if (init_common_dialogue)
    {
        if (!Xenoverse2::InitCommonDialogue())
            return;
    }
}

int xmlize(const std::string &file_path)
{
    BaseFile *file;

    bool is_xml;
    FileType type = get_file_type(file_path, &is_xml);

    if (type == FileType::_2RY)
    {
        file = new _2ryFile();
    }
    else if (type == FileType::BGR)
    {
        file = new BgrFile();
    }
    else if (type == FileType::CMS)
    {
        file = new CmsFile();
    }
    else if (type == FileType::CSC)
    {
        file = new CscFile();
    }
    else if (type == FileType::CSP)
    {
        file = new CspFile();
    }
    else if (type == FileType::CUS)
    {
        file = new CusFile();
    }
    else if (type == FileType::CSO)
    {
        file = new CsoFile();
    }
    else if (type == FileType::EMA)
    {
        file = new EmaFile();
    }
    else if (type == FileType::EMB)
    {
        file = new EmbFile();
    }
    else if (type == FileType::EMM)
    {
        file = new EmmFile();
    }
    else if (type == FileType::EMO)
    {
        file = new EmoFile();
    }
    else if (type == FileType::MSG)
    {
        file = new MsgFile();
    }
    else if (type == FileType::SKL)
    {
        file = new SkeletonFile();
    }
    else if (type == FileType::SPM)
    {
        file = new SpmFile();
    }
    else if (type == FileType::TDB)
    {
        file = new TdbFile();
    }
    else if (type == FileType::VPM)
    {
        file = new VpmFile();
    }
    else if (type == FileType::VST)
    {
        file = new VstFile();
    }
    else if (type == FileType::BCS)
    {
        file = new BcsFile();
    }
    else if (type == FileType::PSC)
    {
        file = new PscFile();
    }
    else if (type == FileType::HCI)
    {
        file = new HciFile();
    }
    else if (type == FileType::SEV)
    {
        file = new SevFile();
    }
    else if (type == FileType::CML)
    {
        file = new CmlFile();
    }
    else if (type == FileType::PAL)
    {
        file = new PalFile();
    }
    else if (type == FileType::IDB)
    {
        file = new IdbFile();
    }
    else if (type == FileType::PUP)
    {
        file = new PupFile();
    }
    else if (type == FileType::BAC)
    {
        file = new BacFile();
    }
    else if (type == FileType::AIT)
    {
        file = new AitFile();
    }
    else if (type == FileType::BCM)
    {
        file = new BcmFile();
    }
    else if (type == FileType::AUR)
    {
        file = new AurFile();
    }
    else if (type == FileType::BAI)
    {
        file = new BaiFile();
    }
    else if (type == FileType::TNL)
    {
        file = new TnlFile();
    }
    else if (type == FileType::TSR)
    {
        file = new TsrFile();
    }
    else if (type == FileType::TSD)
    {
        file = new TsdFile();
    }
    else if (type == FileType::TTB)
    {
        file = new TtbFile();
    }
    else if (type == FileType::TTC)
    {
        file = new TtcFile();
    }
    else if (type == FileType::CNC)
    {
        file = new CncFile();
    }
    else
    {
        DPRINTF("Filetype of \"%s\" not supported.\n", file_path.c_str());
        return -1;
    }

    if (!file->SmartLoad(file_path))
    {
        DPRINTF("Load of file failed.\n");
        delete file;
        return -1;
    }

    std::string output;

    if (is_xml)
    {
        output = file_path.substr(0, file_path.length()-4);
    }
    else
    {
        output = file_path + ".xml";
    }

    if (!is_xml)
    {
        if (type == FileType::CMS || type == FileType::CUS || type == FileType::CSO ||
            type == FileType::PSC || type == FileType::HCI || type == FileType::SEV ||
            type == FileType::CML || type == FileType::PAL || type == FileType::TNL ||
            type == FileType::IDB || type == FileType::TSD || type == FileType::TTB ||
            type == FileType::TTC || type == FileType::CNC)
        {
            bool load_idb;
            bool load_lobby;
            bool common_dialogue;

            load_idb = (type== FileType::IDB || type == FileType::PAL || type == FileType::TNL || type == FileType::TSD);
            load_lobby = (type == FileType::TSD);
            common_dialogue = (type == FileType::TTB);

            Xenoverse2Setup(load_idb, load_lobby, common_dialogue);

            IdbFile *idb_file = dynamic_cast<IdbFile *>(file);
            if (idb_file)
            {
                IdbCommentType comm_type = IdbCommentType::NONE;
                std::string filename = Utils::ToLowerCase(Utils::GetFileNameString(file_path));

                if (filename == "skill_item.idb")
                {
                    comm_type = IdbCommentType::SKILL;
                }
                else if (filename == "costume_top_item.idb" || filename == "costume_bottom_item.idb"  ||
                         filename == "costume_gloves_item.idb" || filename == "costume_shoes_item.idb")
                {
                    comm_type = IdbCommentType::COSTUME;
                }
                else if (filename == "accessory_item.idb")
                {
                    comm_type = IdbCommentType::ACCESORY;
                }
                else if (filename == "talisman_item.idb")
                {
                    comm_type = IdbCommentType::TALISMAN;
                }
                else if (filename == "material_item.idb")
                {
                    comm_type = IdbCommentType::MATERIAL;
                }
                else if (filename == "battle_item.idb")
                {
                    comm_type = IdbCommentType::BATTLE;
                }

                idb_file->SetCommentType(comm_type);
            }

            TtbFile *ttb_file = dynamic_cast<TtbFile *>(file);
            if (ttb_file)
                ttb_file->SetSubtitleComments(true);

            TtcFile *ttc_file = dynamic_cast<TtcFile *>(file);
            if (ttc_file)
                ttc_file->SetSubtitleComments(true);
        }
    }

    if (!file->SmartSave(output))
    {
        DPRINTF("File save failed.\n");
        delete file;
        return -1;
    }

    UPRINTF("Done.\n");
    delete file;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        DPRINTF("Bad usage. Usage: %s file\n", argv[0]);
        return -1;
    }

    int ret = xmlize(argv[1]);

    if (ret != 0)
    {
        UPRINTF("Press enter to exit.");
        getchar();
    }

    return ret;
}
