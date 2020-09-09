#include <QDir>
#include <QFile>

#include "Config.h"
#include "IniFile.h"
#include "requerimentsdialog.h"

#include "debug.h"

#define XV2INS_CONFIG  "XV2INS\\xv2ins.ini"

Config config;

static std::string GetIniPath()
{
    return Utils::GetAppData() + "\\" + XV2INS_CONFIG;
}

void Config::Load()
{
    IniFile ini;

    std::string game_directory_std;
    std::string flex_path_std;

    std::string lf_installer_open_std;

    std::string lf_editor_open_std;
    std::string lf_editor_save_std;

    std::string ld_chara_dir_std;
    std::string ld_skill_dir_std;
    std::string lf_sel_port_std;
    std::string lf_btl_port_std;
    std::string ld_add_data_std;
    std::string ld_css_audio_std;
    std::string ld_import_skill_std;
    std::string ld_stage_dir_std;
    std::string lf_stage_lighting_std;
    std::string lf_sel_port_stage_std;
    std::string lf_qst_port_stage_std;
    std::string lf_sel_back_stage_std;

    std::string ld_cost_hum_std;
    std::string ld_cost_huf_std;
    std::string ld_cost_nmc_std;
    std::string ld_cost_fri_std;
    std::string ld_cost_mam_std;
    std::string ld_cost_maf_std;

    std::string lf_external_cms_std;
    std::string lf_external_cus_std;
    std::string lf_external_cso_std;
    std::string lf_external_psc_std;
    std::string lf_external_aur_std;
    std::string lf_external_sev_std;
    std::string lf_external_cml_std;
    std::string lf_external_hci_std;
    std::string lf_external_pal_std;
    std::string lf_external_sav_std;
    std::string lf_external_pup_std;
    std::string lf_depends_skill_std;
    std::string lf_external_bcs_std;
    std::string lf_depends_cost_std;
    std::string lf_external_ttb_std;
    std::string lf_external_ttc_std;
    std::string lf_external_cnc_std;

    std::string ld_quest_export_std;
    std::string ld_quest_import_std;
    std::string ld_quest_attachments_std;

    // Ignore errors
    ini.LoadFromFile(GetIniPath(), false);

    ini.GetStringValue("General", "game_directory", game_directory_std);
    ini.GetStringValue("General", "flex_path", flex_path_std);

    ini.GetFloatValue("Installer", "patcher_version_new", &patcher_version_new);
    ini.GetStringValue("Installer", "lf_installer_open", lf_installer_open_std);

    ini.GetStringValue("Editor", "lf_editor_open", lf_editor_open_std);
    ini.GetStringValue("Editor", "lf_editor_save", lf_editor_save_std);

    ini.GetStringValue("Editor", "ld_chara_dir", ld_chara_dir_std);
    ini.GetStringValue("Editor", "ld_skill_dir", ld_skill_dir_std);
    ini.GetStringValue("Editor", "lf_sel_port", lf_sel_port_std);
    ini.GetStringValue("Editor", "lf_btl_port", lf_btl_port_std);
    ini.GetStringValue("Editor", "ld_add_data", ld_add_data_std);
    ini.GetStringValue("Editor", "ld_css_audio", ld_css_audio_std);
    ini.GetStringValue("Editor", "ld_import_skill", ld_import_skill_std);
    ini.GetStringValue("Editor", "ld_stage_dir", ld_stage_dir_std);
    ini.GetStringValue("Editor", "lf_stage_lighting", lf_stage_lighting_std);
    ini.GetStringValue("Editor", "lf_sel_port_stage", lf_sel_port_stage_std);
    ini.GetStringValue("Editor", "lf_qst_port_stage", lf_qst_port_stage_std);
    ini.GetStringValue("Editor", "lf_sel_back_stage", lf_sel_back_stage_std);

    ini.GetStringValue("Editor", "ld_cost_hum", ld_cost_hum_std);
    ini.GetStringValue("Editor", "ld_cost_huf", ld_cost_huf_std);
    ini.GetStringValue("Editor", "ld_cost_nmc", ld_cost_nmc_std);
    ini.GetStringValue("Editor", "ld_cost_fri", ld_cost_fri_std);
    ini.GetStringValue("Editor", "ld_cost_mam", ld_cost_mam_std);
    ini.GetStringValue("Editor", "ld_cost_maf", ld_cost_maf_std);

    ini.GetStringValue("Editor", "lf_external_cms", lf_external_cms_std);
    ini.GetStringValue("Editor", "lf_external_cus", lf_external_cus_std);
    ini.GetStringValue("Editor", "lf_external_cso", lf_external_cso_std);
    ini.GetStringValue("Editor", "lf_external_psc", lf_external_psc_std);
    ini.GetStringValue("Editor", "lf_external_aur", lf_external_aur_std);
    ini.GetStringValue("Editor", "lf_external_sev", lf_external_sev_std);
    ini.GetStringValue("Editor", "lf_external_cml", lf_external_cml_std);
    ini.GetStringValue("Editor", "lf_external_hci", lf_external_hci_std);
    ini.GetStringValue("Editor", "lf_external_pal", lf_external_pal_std);
    ini.GetStringValue("Editor", "lf_external_sav", lf_external_sav_std);
    ini.GetStringValue("Editor", "lf_external_pup", lf_external_pup_std);
    ini.GetStringValue("Editor", "lf_depends_skill", lf_depends_skill_std);
    ini.GetStringValue("Editor", "lf_external_bcs", lf_external_bcs_std);
    ini.GetStringValue("Editor", "lf_depends_cost", lf_depends_cost_std);
    ini.GetStringValue("Editor", "lf_external_ttb", lf_external_ttb_std);
    ini.GetStringValue("Editor", "lf_external_ttc", lf_external_ttc_std);
    ini.GetStringValue("Editor", "lf_external_cnc", lf_external_cnc_std);

    ini.GetIntegerValue("Editor", "position", &position, 0);
    ini.GetBooleanValue("Editor", "listen_css_language_english", &listen_css_language_english);
    ini.GetBooleanValue("Editor", "listen_sev_language_english", &listen_sev_language_english);

    ini.GetBooleanValue("Installer", "prb_fix_applied", &prb_fix_applied, false);

    ini.GetStringValue("Editor", "ld_quest_export", ld_quest_export_std);
    ini.GetStringValue("Editor", "ld_quest_import", ld_quest_import_std);
    ini.GetStringValue("Editor", "ld_quest_attachments", ld_quest_attachments_std);

    game_directory = Utils::StdStringToQString(game_directory_std);
    flex_path = Utils::StdStringToQString(flex_path_std);

    lf_installer_open = Utils::StdStringToQString(lf_installer_open_std);

    lf_editor_open = Utils::StdStringToQString(lf_editor_open_std);
    lf_editor_save = Utils::StdStringToQString(lf_editor_save_std);

    ld_chara_dir = Utils::StdStringToQString(ld_chara_dir_std);
    ld_skill_dir = Utils::StdStringToQString(ld_skill_dir_std);
    lf_sel_port = Utils::StdStringToQString(lf_sel_port_std);
    lf_btl_port = Utils::StdStringToQString(lf_btl_port_std);
    ld_add_data = Utils::StdStringToQString(ld_add_data_std);
    ld_css_audio = Utils::StdStringToQString(ld_css_audio_std);
    ld_import_skill = Utils::StdStringToQString(ld_import_skill_std);
    ld_stage_dir = Utils::StdStringToQString(ld_stage_dir_std);
    lf_stage_lighting = Utils::StdStringToQString(lf_stage_lighting_std);
    lf_sel_port_stage = Utils::StdStringToQString(lf_sel_port_stage_std);
    lf_qst_port_stage = Utils::StdStringToQString(lf_qst_port_stage_std);
    lf_sel_back_stage = Utils::StdStringToQString(lf_sel_back_stage_std);

    ld_cost_hum = Utils::StdStringToQString(ld_cost_hum_std);
    ld_cost_huf = Utils::StdStringToQString(ld_cost_huf_std);
    ld_cost_nmc = Utils::StdStringToQString(ld_cost_nmc_std);
    ld_cost_fri = Utils::StdStringToQString(ld_cost_fri_std);
    ld_cost_mam = Utils::StdStringToQString(ld_cost_mam_std);
    ld_cost_maf = Utils::StdStringToQString(ld_cost_maf_std);

    lf_external_cms = Utils::StdStringToQString(lf_external_cms_std);
    lf_external_cus = Utils::StdStringToQString(lf_external_cus_std);
    lf_external_cso = Utils::StdStringToQString(lf_external_cso_std);
    lf_external_psc = Utils::StdStringToQString(lf_external_psc_std);
    lf_external_aur = Utils::StdStringToQString(lf_external_aur_std);
    lf_external_sev = Utils::StdStringToQString(lf_external_sev_std);
    lf_external_cml = Utils::StdStringToQString(lf_external_cml_std);
    lf_external_hci = Utils::StdStringToQString(lf_external_hci_std);
    lf_external_pal = Utils::StdStringToQString(lf_external_pal_std);
    lf_external_sav = Utils::StdStringToQString(lf_external_sav_std);
    lf_external_pup = Utils::StdStringToQString(lf_external_pup_std);
    lf_depends_skill = Utils::StdStringToQString(lf_depends_skill_std);
    lf_external_bcs = Utils::StdStringToQString(lf_external_bcs_std);
    lf_depends_cost = Utils::StdStringToQString(lf_depends_cost_std);
    lf_external_ttb = Utils::StdStringToQString(lf_external_ttb_std);
    lf_external_ttc = Utils::StdStringToQString(lf_external_ttc_std);
    lf_external_cnc = Utils::StdStringToQString(lf_external_cnc_std);

    ld_quest_export = Utils::StdStringToQString(ld_quest_export_std);
    ld_quest_import = Utils::StdStringToQString(ld_quest_import_std);
    ld_quest_attachments = Utils::StdStringToQString(ld_quest_attachments_std);
}

void Config::Save()
{
    IniFile ini;
    std::string ini_path = GetIniPath();

    std::string game_directory_std;
    std::string flex_path_std;

    std::string lf_installer_open_std;

    std::string lf_editor_open_std;
    std::string lf_editor_save_std;

    std::string ld_chara_dir_std;
    std::string ld_skill_dir_std;
    std::string lf_sel_port_std;
    std::string lf_btl_port_std;
    std::string ld_add_data_std;
    std::string ld_css_audio_std;
    std::string ld_import_skill_std;
    std::string ld_stage_dir_std;
    std::string lf_stage_lighting_std;
    std::string lf_sel_port_stage_std;
    std::string lf_qst_port_stage_std;
    std::string lf_sel_back_stage_std;

    std::string ld_cost_hum_std;
    std::string ld_cost_huf_std;
    std::string ld_cost_nmc_std;
    std::string ld_cost_fri_std;
    std::string ld_cost_mam_std;
    std::string ld_cost_maf_std;

    std::string lf_external_cms_std;
    std::string lf_external_cus_std;
    std::string lf_external_cso_std;
    std::string lf_external_psc_std;
    std::string lf_external_aur_std;
    std::string lf_external_sev_std;
    std::string lf_external_cml_std;
    std::string lf_external_hci_std;
    std::string lf_external_pal_std;
    std::string lf_external_sav_std;
    std::string lf_external_pup_std;
    std::string lf_depends_skill_std;
    std::string lf_external_bcs_std;
    std::string lf_depends_cost_std;
    std::string lf_external_ttb_std;
    std::string lf_external_ttc_std;
    std::string lf_external_cnc_std;

    std::string ld_quest_export_std;
    std::string ld_quest_import_std;
    std::string ld_quest_attachments_std;

    game_directory_std = Utils::QStringToStdString(game_directory);
    flex_path_std = Utils::QStringToStdString(flex_path);

    lf_installer_open_std = Utils::QStringToStdString(lf_installer_open);

    lf_editor_open_std = Utils::QStringToStdString(lf_editor_open);
    lf_editor_save_std = Utils::QStringToStdString(lf_editor_save);

    ld_chara_dir_std = Utils::QStringToStdString(ld_chara_dir);
    ld_skill_dir_std = Utils::QStringToStdString(ld_skill_dir);
    lf_sel_port_std = Utils::QStringToStdString(lf_sel_port);
    lf_btl_port_std = Utils::QStringToStdString(lf_btl_port);
    ld_add_data_std = Utils::QStringToStdString(ld_add_data);
    ld_css_audio_std = Utils::QStringToStdString(ld_css_audio);
    ld_import_skill_std = Utils::QStringToStdString(ld_import_skill);
    ld_stage_dir_std = Utils::QStringToStdString(ld_stage_dir);
    lf_stage_lighting_std = Utils::QStringToStdString(lf_stage_lighting);
    lf_sel_port_stage_std = Utils::QStringToStdString(lf_sel_port_stage);
    lf_qst_port_stage_std = Utils::QStringToStdString(lf_qst_port_stage);
    lf_sel_back_stage_std = Utils::QStringToStdString(lf_sel_back_stage);

    ld_cost_hum_std = Utils::QStringToStdString(ld_cost_hum);
    ld_cost_huf_std = Utils::QStringToStdString(ld_cost_huf);
    ld_cost_nmc_std = Utils::QStringToStdString(ld_cost_nmc);
    ld_cost_fri_std = Utils::QStringToStdString(ld_cost_fri);
    ld_cost_mam_std = Utils::QStringToStdString(ld_cost_mam);
    ld_cost_maf_std = Utils::QStringToStdString(ld_cost_maf);

    lf_external_cms_std = Utils::QStringToStdString(lf_external_cms);
    lf_external_cus_std = Utils::QStringToStdString(lf_external_cus);
    lf_external_cso_std = Utils::QStringToStdString(lf_external_cso);
    lf_external_psc_std = Utils::QStringToStdString(lf_external_psc);
    lf_external_aur_std = Utils::QStringToStdString(lf_external_aur);
    lf_external_sev_std = Utils::QStringToStdString(lf_external_sev);
    lf_external_cml_std = Utils::QStringToStdString(lf_external_cml);
    lf_external_hci_std = Utils::QStringToStdString(lf_external_hci);
    lf_external_pal_std = Utils::QStringToStdString(lf_external_pal);
    lf_external_sav_std = Utils::QStringToStdString(lf_external_sav);
    lf_external_pup_std = Utils::QStringToStdString(lf_external_pup);
    lf_depends_skill_std = Utils::QStringToStdString(lf_depends_skill);
    lf_external_bcs_std = Utils::QStringToStdString(lf_external_bcs);
    lf_depends_cost_std = Utils::QStringToStdString(lf_depends_cost);
    lf_external_ttb_std = Utils::QStringToStdString(lf_external_ttb);
    lf_external_ttc_std = Utils::QStringToStdString(lf_external_ttc);
    lf_external_cnc_std = Utils::QStringToStdString(lf_external_cnc);

    ld_quest_export_std = Utils::QStringToStdString(ld_quest_export);
    ld_quest_import_std = Utils::QStringToStdString(ld_quest_import);
    ld_quest_attachments_std = Utils::QStringToStdString(ld_quest_attachments);

    ini.LoadFromFile(ini_path, false);

    ini.SetStringValue("General", "game_directory", game_directory_std);
    ini.SetStringValue("General", "flex_path", flex_path_std);

    ini.SetFloatValue("Installer", "patcher_version_new", patcher_version_new);
    ini.SetStringValue("Installer", "lf_installer_open", lf_installer_open_std);    

    ini.SetStringValue("Editor", "lf_editor_open", lf_editor_open_std);
    ini.SetStringValue("Editor", "lf_editor_save", lf_editor_save_std);

    ini.SetStringValue("Editor", "ld_chara_dir", ld_chara_dir_std);
    ini.SetStringValue("Editor", "ld_skill_dir", ld_skill_dir_std);
    ini.SetStringValue("Editor", "lf_sel_port", lf_sel_port_std);
    ini.SetStringValue("Editor", "lf_btl_port", lf_btl_port_std);
    ini.SetStringValue("Editor", "ld_add_data", ld_add_data_std);
    ini.SetStringValue("Editor", "ld_css_audio", ld_css_audio_std);
    ini.SetStringValue("Editor", "ld_import_skill", ld_import_skill_std);
    ini.SetStringValue("Editor", "ld_stage_dir", ld_stage_dir_std);
    ini.SetStringValue("Editor", "lf_stage_lighting", lf_stage_lighting_std);
    ini.SetStringValue("Editor", "lf_sel_port_stage", lf_sel_port_stage_std);
    ini.SetStringValue("Editor", "lf_qst_port_stage", lf_qst_port_stage_std);

    ini.SetStringValue("Editor", "ld_cost_hum", ld_cost_hum_std);
    ini.SetStringValue("Editor", "ld_cost_huf", ld_cost_huf_std);
    ini.SetStringValue("Editor", "ld_cost_nmc", ld_cost_nmc_std);
    ini.SetStringValue("Editor", "ld_cost_fri", ld_cost_fri_std);
    ini.SetStringValue("Editor", "ld_cost_mam", ld_cost_mam_std);
    ini.SetStringValue("Editor", "ld_cost_maf", ld_cost_maf_std);

    ini.SetStringValue("Editor", "lf_external_cms", lf_external_cms_std);
    ini.SetStringValue("Editor", "lf_external_cus", lf_external_cus_std);
    ini.SetStringValue("Editor", "lf_external_cso", lf_external_cso_std);
    ini.SetStringValue("Editor", "lf_external_psc", lf_external_psc_std);
    ini.SetStringValue("Editor", "lf_external_aur", lf_external_aur_std);
    ini.SetStringValue("Editor", "lf_external_sev", lf_external_sev_std);
    ini.SetStringValue("Editor", "lf_external_cml", lf_external_cml_std);
    ini.SetStringValue("Editor", "lf_external_hci", lf_external_hci_std);
    ini.SetStringValue("Editor", "lf_external_pal", lf_external_pal_std);
    ini.SetStringValue("Editor", "lf_external_sav", lf_external_sav_std);
    ini.SetStringValue("Editor", "lf_external_pup", lf_external_pup_std);
    ini.SetStringValue("Editor", "lf_depends_skill", lf_depends_skill_std);
    ini.SetStringValue("Editor", "lf_external_bcs", lf_external_bcs_std);
    ini.SetStringValue("Editor", "lf_depends_cost", lf_depends_cost_std);
    ini.SetStringValue("Editor", "lf_sel_back_stage", lf_sel_back_stage_std);
    ini.SetStringValue("Editor", "lf_external_ttb", lf_external_ttb_std);
    ini.SetStringValue("Editor", "lf_external_ttc", lf_external_ttc_std);
    ini.SetStringValue("Editor", "lf_external_cnc", lf_external_cnc_std);

    ini.SetIntegerValue("Editor", "position", position);
    ini.SetBooleanValue("Editor", "listen_css_language_english", listen_css_language_english);
    ini.SetBooleanValue("Editor", "listen_sev_language_english", listen_sev_language_english);

    ini.SetBooleanValue("Installer", "prb_fix_applied", &prb_fix_applied);

    ini.SetStringValue("Editor", "ld_quest_export", ld_quest_export_std);
    ini.SetStringValue("Editor", "ld_quest_import", ld_quest_import_std);
    ini.SetStringValue("Editor", "ld_quest_attachments", ld_quest_attachments_std);

    ini.SaveToFile(ini_path, true, true);
}

void Config::FindRequirements(bool &configure_game, bool &configure_flex)
{
    static bool already_done = false;

    if (already_done) // Avoid doing this again if we go recursive in ConfigureRequirements
        return;

    already_done = true;

    if (configure_game)
    {
        const std::string path1 = "C:/Program Files (x86)/Steam/steamapps/common/DB Xenoverse 2/bin/DBXV2.exe";
        std::string path2 = "@:/SteamLibrary/steamapps/common/DB Xenoverse 2/bin/DBXV2.exe";

        if (Utils::FileExists(path1))
        {
            configure_game = false;
            game_directory = Utils::StdStringToQString(path1, false);
        }
        else
        {
            for (char c = 'C'; c <= 'Z'; c++)
            {
                path2.front() = c;

                if (Utils::FileExists(path2))
                {
                    configure_game = false;
                    game_directory = Utils::StdStringToQString(path2, false);
                }

                break;
            }
        }

        if (!configure_game)
            game_directory = game_directory.mid(0, game_directory.length()-13);
    }

    if (configure_flex)
    {
        std::string path1 = "@:/flex_sdk_4.6/bin/mxmlc.exe";
        std::string path2 = "@:/flex_sdk/bin/mxmlc.exe";

        for (char c = 'C'; c <= 'Z'; c++)
        {
            path1.front() = c; path2.front() = c;

            if (Utils::FileExists(path1))
            {
                configure_flex = false;
                flex_path = Utils::StdStringToQString(path1);
                break;
            }
            else if (Utils::FileExists(path2))
            {
                configure_flex = false;
                flex_path = Utils::StdStringToQString(path2);
                break;
            }
        }
    }
}

bool Config::ConfigureRequirements()
{
    bool configure_game = false;
    bool configure_flex = false;

    QDir game(game_directory);

    if (game_directory.length() > 0 && game.exists())
    {
        QFile game_exe(game.filePath("bin/DBXV2.exe"));

        if (!game_exe.exists())
        {
            configure_game = true;
        }
    }
    else
    {
        configure_game = true;
    }

    QFile flex(flex_path);
    configure_flex = (flex_path.length() == 0 || !flex.exists());

    if (!configure_game && !configure_flex)
        return true;

    FindRequirements(configure_game, configure_flex);

    //if (!configure_game && !configure_flex)
    if (!configure_game)
        return true;

    QDir game2(game_directory);

    RequerimentsDialog dialog(configure_game ? "" : game2.absoluteFilePath("bin/DBXV2.exe"));

    if (!dialog.exec())
        return false;

    game_directory = dialog.GetGameExe();

    if (!game_directory.endsWith("/bin/DBXV2.exe", Qt::CaseInsensitive))
    {
        if (!game_directory.endsWith("\\bin\\DBXV2.exe", Qt::CaseInsensitive))
        {
            game_directory.clear();
            return false;
        }

        game_directory = game_directory.mid(0, game_directory.length()-13);
    }
    else
    {
       game_directory = game_directory.mid(0, game_directory.length()-13);
    }

    // ehem, let's recurse until user selects proper directory, clicks cancel, or error
    return ConfigureRequirements();
}

QString Config::GetDirectory(const QString &file)
{
    if (file.isEmpty())
        return QString();

    int i = file.lastIndexOf(QRegExp("[/\\\\]+"));
    if (i == -1)
        return file;

    return file.left(i);
}


#include "Xenoverse2.h"

#define XV2PATCHER_DLL  "bin/xinput1_3.dll"
#define XV2PATCHER_ALT_DLL  "bin/dinput8.dll"
#define XV2PATCHER_INI  "XV2PATCHER/xv2patcher.ini"

static uint8_t v01_md5[16] = { 0xA8, 0x94, 0x0B, 0x2D, 0xEE, 0x80, 0x90, 0xE8, 0x09, 0xB0, 0xDF, 0xEB, 0x4E, 0xE2, 0x01, 0x3B };
static uint8_t v02_md5[16] = { 0x0A, 0x10, 0x48, 0x41, 0xB8, 0xCC, 0x52, 0x80, 0x0A, 0x89, 0xC7, 0x74, 0x2F, 0xCB, 0x10, 0xA8 };
static uint8_t v03_md5[16] = { 0xC9, 0xCF, 0xD9, 0x5E, 0x64, 0x76, 0x2E, 0x78, 0x6D, 0xC2, 0x91, 0xB6, 0x10, 0xB6, 0x76, 0xCE };
static uint8_t v04_md5[16] = { 0x74, 0xB4, 0x58, 0x2F, 0x3F, 0x8C, 0x82, 0xD9, 0x71, 0xC5, 0x18, 0xE6, 0xC8, 0xCF, 0xD2, 0x97 };
static uint8_t v041_md5[16] = {	0xDA, 0x16, 0x84, 0x52, 0xAB, 0x79, 0x32, 0x1D, 0xC9, 0x04, 0x07, 0x92, 0x08, 0x67, 0x4D, 0x11 };
static uint8_t v05_md5[16] = { 0xE8, 0xFA, 0x44, 0x50, 0xBC, 0x93, 0x59, 0xC7, 0x7C, 0x65, 0xF9, 0xB6, 0x59, 0x8C, 0x39, 0x66 };
static uint8_t v05b_md5[16] = {	0xD5, 0xFA, 0x95, 0x58, 0xAD, 0x91, 0x72, 0xAB, 0x46, 0xFC, 0x53, 0x57, 0x78, 0xFF, 0xC1, 0x60 };
static uint8_t v05c_md5[16] = {	0x7F, 0xEA, 0x57, 0x6D, 0x44, 0x70, 0x79, 0xDB, 0x86, 0xA7, 0x52, 0x25, 0x00, 0x42, 0x26, 0xA6 };
static uint8_t v051_md5[16] = {	0x1C, 0x9A, 0x1D, 0x9E, 0x99, 0x61, 0x8B, 0x72, 0xCC, 0xCF, 0xA2, 0x21, 0x2A, 0x1C, 0xE7, 0x72 };
static uint8_t v051_pre_md5[16] = { 0x70, 0x04, 0x7F, 0x10, 0x76, 0x02, 0xED, 0xB0, 0x61, 0x10, 0xC1, 0xCE, 0x2E, 0x2B, 0xB4, 0x4C };
static uint8_t v06_md5[16] = { 0xF8, 0x1D, 0x0B, 0xEC, 0x18, 0x91, 0xA0, 0x6D, 0x75, 0xCE, 0xE6, 0x98, 0xE0, 0x98, 0x41, 0x22 };
static uint8_t v07_md5[16] = { 0x3B, 0x61, 0xE5, 0xA3, 0x1B, 0xC8, 0x3D, 0xD9, 0x90, 0x3F, 0x89, 0x50, 0xFE, 0x79, 0xCC, 0xBE };

static float version = -1.0f;

static bool GetPatcherVersion(const std::string &file, float *version, bool warn)
{
    size_t size;
    uint8_t *buf = xv2fs->ReadFile(file, &size);
    uint8_t md5[16];

    if (!buf)
    {
        *version = -1.0f;

        if (warn)
        {
            DPRINTF("Cannot open \"%s\" in game directory.\n", file.c_str());
        }

        return false;
    }

    const char *search_str = "XV2PATCHER VERSION ";

    for (size_t i = 0; i < size-50; i++)
    {
        if (memcmp(buf+i, search_str, strlen(search_str)) == 0)
        {
            float temp;

            if (sscanf((const char *)buf+i+strlen(search_str), "%f", &temp) == 1)
            {
                *version = temp;

                /* Begin debug */
                /*char debug_str[4];
                debug_str[3] = 0;
                memcpy(debug_str, buf+i+strlen(search_str), 3);

                DPRINTF("Version obtained from [GAME_DIRECTORY]/\"%s\".\nFloat version = %f. String version = %s\n"
                        "Game directory = %s\n", file.c_str(), temp, debug_str, Utils::QStringToStdString(config.game_directory).c_str());*/
                /* End debug */

                delete[] buf;                
                return true;
            }

            break;
        }
    }

    Utils::Md5(buf, (uint32_t)size, md5);
    delete[] buf;

    if (memcmp(md5, v01_md5, sizeof(md5)) == 0)
    {
        *version = 0.1f;
    }
    else if (memcmp(md5, v02_md5, sizeof(md5)) == 0)
    {
        *version = 0.2f;
    }
    else if (memcmp(md5, v03_md5, sizeof(md5)) == 0)
    {
        *version = 0.3f;
    }
    else if (memcmp(md5, v04_md5, sizeof(md5)) == 0)
    {
        *version = 0.4f;
    }
    else if (memcmp(md5, v041_md5, sizeof(md5)) == 0)
    {
        *version = 0.41f;
    }
    else if (memcmp(md5, v05_md5, sizeof(md5)) == 0 || memcmp(md5, v05b_md5, sizeof(md5)) == 0 || memcmp(md5, v05c_md5, sizeof(md5)) == 0)
    {
        *version = 0.5f;
    }
    else if (memcmp(md5, v051_md5, sizeof(md5)) == 0 || memcmp(md5, v051_pre_md5, sizeof(md5)) == 0)
    {
        *version = 0.51f;
    }
    else if (memcmp(md5, v06_md5, sizeof(md5)) == 0)
    {
        *version = 0.6f;
    }
    else if (memcmp(md5, v07_md5, sizeof(md5)) == 0)
    {
        *version = 0.7f;
    }
    else
    {
        *version = 0.0f;
    }

    return true;
}

void XV2Patcher::Init()
{
    if (!GetPatcherVersion(XV2PATCHER_ALT_DLL, &version, false))
    {
        GetPatcherVersion(XV2PATCHER_DLL, &version, true);
    }
}

bool XV2Patcher::IsInstalled()
{
    return (version >= 0.0f);
}

float XV2Patcher::GetVersion()
{
    return version;
}

bool XV2Patcher::ConfigureDefaults()
{
    IniFile ini;

    if (!xv2fs->LoadFile(&ini, XV2PATCHER_INI))
    {
        DPRINTF("Failed to load xv2patcher.ini. Fix your xv2patcher installation.\n");
        return false;
    }

    ini.SetBooleanValue("Patches", "loose_files", true);
    ini.SetBooleanValue("Patches", "unlock_chara_mods", true);

    if (!xv2fs->SaveFile(&ini, XV2PATCHER_INI))
    {
        DPRINTF("Failed to save xv2patcher.ini.\n"
                "The game directory may not be writtable.\n\n"
                "THIS PROGRAM NEEDS A WRITABLE GAME DIRECTORY, I WON'T SAY IT AGAIN");
        return false;
    }

    return true;
}

bool XV2Patcher::SetConfigIfNeeded()
{
    IniFile ini;

    if (!xv2fs->LoadFile(&ini, XV2PATCHER_INI))
    {
        DPRINTF("Failed to load xv2patcher.ini. Fix your xv2patcher installation.\n");
        return false;
    }

    ini.AddIntegerConstant("auto", 0);

    bool new_chara, new_stages;
    int CharacterMax;

    ini.GetBooleanValue("Patches", "new_chara", &new_chara, false);
    ini.GetIntegerValue("NewChara", "CharacterMax", &CharacterMax, 99);
    ini.GetBooleanValue("Patches", "new_stages", &new_stages, false);

    if (new_chara && CharacterMax == 0 && new_stages)
        return true;

    ini.SetBooleanValue("Patches", "new_chara", true);
    ini.SetIntegerValue("NewChara", "CharacterMax", 0);
    ini.SetBooleanValue("Patches", "new_stages", true);

    return xv2fs->SaveFile(&ini, XV2PATCHER_INI);
}
