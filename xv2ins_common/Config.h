#ifndef ____CONFIG_H_
#define ____CONFIG_H_

#include <QString>
#include "Utils.h"

//#define CURRENT_MAX_SLOTS   1000

struct Config
{
    QString game_directory;
    QString flex_path;

    float patcher_version_new;
    QString lf_installer_open;

    QString lf_editor_open;
    QString lf_editor_save;

    QString ld_chara_dir;
    QString ld_skill_dir;
    QString lf_sel_port;
    QString lf_btl_port;
    QString ld_add_data;
    QString ld_css_audio;
    QString ld_import_skill;
    QString ld_stage_dir;
    QString lf_stage_lighting;
    QString lf_sel_port_stage;
    QString lf_qst_port_stage;
    QString lf_sel_back_stage;

    QString ld_cost_hum;
    QString ld_cost_huf;
    QString ld_cost_nmc;
    QString ld_cost_fri;
    QString ld_cost_mam;
    QString ld_cost_maf;

    QString lf_external_cms;
    QString lf_external_cus;
    QString lf_external_cso;
    QString lf_external_psc;
    QString lf_external_aur;
    QString lf_external_sev;
    QString lf_external_cml;
    QString lf_external_hci;
    QString lf_external_pal;
    QString lf_external_sav;
    QString lf_external_pup;
    QString lf_depends_skill;
    QString lf_external_bcs;
    QString lf_depends_cost;
    QString lf_external_ttb;
    QString lf_external_ttc;
    QString lf_external_cnc;

    QString ld_quest_export;
    QString ld_quest_import;
    QString ld_quest_attachments;

    int position;

    bool listen_css_language_english;
    bool listen_sev_language_english;

    bool prb_fix_applied;

    void Load();
    void Save();

    bool ConfigureRequirements();

    static QString GetDirectory(const QString &file);

private:

    void FindRequirements(bool &configure_game, bool &configure_flex);
};

extern Config config;

namespace XV2Patcher
{
    void Init();
    bool IsInstalled();
    float GetVersion();

    bool ConfigureDefaults();
    bool SetConfigIfNeeded();
}

#endif // ____CONFIG_H_
