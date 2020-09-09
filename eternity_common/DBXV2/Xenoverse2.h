#ifndef __XENOVERSE2_H__
#define __XENOVERSE2_H__

#include "Xv2Fs.h"
#include "CharaListFile.h"
#include "Xv2PatcherSlotsFile.h"
#include "MsgFile.h"
#include "CmsFile.h"
#include "CusFile.h"
#include "CsoFile.h"
#include "PscFile.h"
#include "AurFile.h"
#include "SevFile.h"
#include "CmlFile.h"
#include "HciFile.h"
#include "Xv2PreBakedFile.h"
#include "IdbFile.h"
#include "PalFile.h"
#include "TnlFile.h"
#include "Xv2SavFile.h"

#include "PsaFile.h"
#include "PupFile.h"
#include "BcsFile.h"
#include "Xv2StageDefFile.h"
#include "Xv2PatcherSlotsFileStage.h"
#include "TtbFile.h"
#include "TtcFile.h"
#include "CncFile.h"
#include "CnsFile.h"
#include "ErsFile.h"

#include "Criware/AcbFile.h"
#include "Criware/Afs2File.h"
#include "Criware/HcaFile.h"

#include "Dimps/EmbFile.h"

#include "Misc/IggyFile.h"

// Order is not the one the game uses
enum
{
    XV2_LANG_ENGLISH,
    XV2_LANG_SPANISH1,
    XV2_LANG_SPANISH2,
    XV2_LANG_FRENCH,
    XV2_LANG_GERMAN,
    XV2_LANG_ITALIAN,
    XV2_LANG_PORTUGUESE,
    XV2_LANG_POLISH,
    XV2_LANG_RUSSIAN,
    XV2_LANG_CHINESE1,
    XV2_LANG_CHINESE2,
    XV2_LANG_KOREAN,

    XV2_LANG_NUM
};

// Internal codes of the DLC
// There are the lowest level dlc codes, used by the function that check the dlc ownership
// Some parts of the game may have different codes that ultimately translate to these
enum
{
    XV2_DLC_GKB = 0,
    XV2_DLC1 = 5,
    XV2_DLC2 = 6,
    XV2_DLC3 = 7,
    XV2_DLC4 = 8,
    XV2_DLC_STEVE_AOKI = 0x12,
    XV2_DLC_BGM1 = 0x13, /* this and next... are they released at any region/plattform? */
    XV2_DLC_BGM2 = 0x14,
    XV2_DLC_XV1_STORY = 0x15, /* NX... exclusive? */
    XV2_DLC_EXTRA1 = 0x16,
    XV2_DLC_EXTRA2 = 0x17,
};

#include "X2mCostumeFile.h"

extern const std::vector<std::string> xv2_lang_codes;
extern Xv2PatcherSlotsFile *chara_list;
extern IggyFile *charasele;

extern CmsFile *game_cms;
extern CusFile *game_cus;
extern CsoFile *game_cso;
extern PscFile *game_psc;
extern AurFile *game_aur;
extern SevFile *game_sev;
extern CmlFile *game_cml;
extern HciFile *game_hci;
extern PsaFile *game_psa;
extern PupFile *game_pup;

extern std::vector<MsgFile *> game_chara_names;
extern std::vector<MsgFile *> game_chara_costume_names;

extern std::vector<MsgFile *> sup_sk_names;
extern std::vector<MsgFile *> ult_sk_names;
extern std::vector<MsgFile *> eva_sk_names;
extern std::vector<MsgFile *> awa_sk_names;

extern std::vector<MsgFile *> sup_sk_descs;
extern std::vector<MsgFile *> ult_sk_descs;
extern std::vector<MsgFile *> eva_sk_descs;
extern std::vector<MsgFile *> awa_sk_descs;

extern std::vector<MsgFile *> quest_btlhud_texts;

extern EmbFile *game_sel_port;
extern Xv2PreBakedFile *game_prebaked;

extern std::vector<MsgFile *> game_lobby_texts;
extern std::vector<MsgFile *> game_cac_costume_names;
extern std::vector<MsgFile *> game_accesory_names;
extern std::vector<MsgFile *> game_talisman_names;
extern std::vector<MsgFile *> game_material_names;
extern std::vector<MsgFile *> game_battle_names;

extern std::vector<MsgFile *> game_cac_costume_descs;
extern std::vector<MsgFile *> game_accesory_descs;

extern IdbFile *game_bottom_idb;
extern IdbFile *game_gloves_idb;
extern IdbFile *game_shoes_idb;
extern IdbFile *game_top_idb;
extern IdbFile *game_accesory_idb;
extern IdbFile *game_talisman_idb;
extern IdbFile *game_skill_idb;
extern IdbFile *game_material_idb;
extern IdbFile *game_battle_idb;

extern PalFile *game_pal;
extern TnlFile *game_tnl;

extern AcbFile *game_css_acb_jp;
extern AcbFile *game_css_acb_en;
extern Afs2File *game_css_awb_jp;
extern Afs2File *game_css_awb_en;

extern AcbFile *game_sev_cmn_acb_jp;
extern AcbFile *game_sev_cmn_acb_en;
extern Afs2File *game_sev_cmn_awb_jp;
extern Afs2File *game_sev_cmn_awb_en;

extern AcbFile *game_sev_cmn2_acb_jp;
extern AcbFile *game_sev_cmn2_acb_en;
extern Afs2File *game_sev_cmn2_awb_jp;
extern Afs2File *game_sev_cmn2_awb_en;

extern BcsFile *game_hum_bcs;
extern BcsFile *game_huf_bcs;
extern BcsFile *game_nmc_bcs;
extern BcsFile *game_fri_bcs;
extern BcsFile *game_mam_bcs;
extern BcsFile *game_maf_bcs;

extern X2mCostumeFile *game_costume_file;

extern Xv2PatcherSlotsFileStage *game_stage_slots_file, *game_stage_slots_file_local;

extern Xv2StageDefFile *game_stage_def;
extern std::vector<MsgFile *> game_stage_names;

extern EmbFile *game_stage01_emb, *game_stage02_emb;

extern TtbFile *game_ttb;
extern TtcFile *game_ttc;

extern MsgFile *qc_dialogue_voice;
extern std::vector<MsgFile *> qc_dialogue_subs;

extern CncFile *game_cnc;
extern CnsFile *game_cns;

extern ErsFile *game_ers;

namespace Xenoverse2
{
    void InitFs(const std::string &game_path);
    //bool InitCharaList(const std::string &chasel_path, const std::string &list_path);
    bool InitCharaList();
    bool InitSystemFiles(bool only_cms=false, bool multiple_hci=false);
    bool InitCharaNames(int only_this_lang=-1);
    bool InitCharaCostumeNames(int only_this_lang=-1);
    bool InitSkillNames(int only_this_lang=-1);
    bool InitSkillDescs(int only_this_lang=-1);
    bool InitBtlHudText(int only_this_lang=-1);
    bool InitSelPort();
    bool InitPreBaked();
    bool InitLobbyText(int only_this_lang=-1);
    bool InitCacCostumeNames(int only_this_lang=-1);
    bool InitCacCostumeDescs(int only_this_lang=-1);
    bool InitTalismanNames(int only_this_lang=-1);
    bool InitMaterialNames(int only_this_lang=-1);
    bool InitBattleNames(int only_this_lang=-1);
    bool InitIdb(bool costumes=true, bool accesories=true, bool talisman=true, bool skills=true, bool material=false, bool battle=false);
    bool InitLobby(bool tnl=false);
    bool InitSound(bool load_sev_cmn=false);
    bool InitCac();
    bool InitCostumeFile();
    bool InitStageSlots();
    bool InitStageNames(int only_this_lang=-1);
    bool InitStageEmb();
    bool InitCommonDialogue();
    bool InitDualSkill(bool init_cnc, bool init_cns);
    bool InitVfx();

    bool CommitCharaList(bool commit_slots, bool commit_iggy);
    bool CommitSystemFiles(bool pup);
    bool CommitSelPort();
    bool CommitCharaNames();
    bool CommitCharaCostumeNames();
    bool CommitSkillNames();
    bool CommitSkillDescs();
    bool CommitBtlHudText();
    bool CommitPreBaked();
    bool CommitLobbyText();
    bool CommitCacCostumeNames();
    bool CommitCacCostumeDescs();
    bool CommitLobby(bool tnl=false);
    bool CommitIdb(bool costumes, bool accesories, bool talisman, bool skills);
    bool CommitSound(bool css, bool sev, bool sev1, bool sev2);
    bool CommitCac();
    bool CommitCostumeFile();
    bool CommitStageSlots();
    bool CommitStageNames();
    bool CommitStageEmb();
    bool CommitCommonDialogue();
    bool CommitDualSkill(bool commit_cnc, bool commit_cns);
    bool CommitVfx();

    bool GetCharaCodeFromId(uint32_t id, std::string &code);

    bool IsOriginalChara(const std::string &short_name);
    bool IsForbiddenNewStageName(const std::string &name);
    bool CompileCharaSel(const std::string &chasel_path, const std::string &compiler);

    bool GetCharaName(const std::string &code, std::string &name, int lang=XV2_LANG_ENGLISH, int index=0);
    bool GetCharaCostumeName(const std::string &code, int var, int model_preset, std::string &name, int lang=XV2_LANG_ENGLISH);

    bool SetCharaName(const std::string &code, const std::string &name, int lang, int index=0);
    bool SetCharaCostumeName(const std::string &code, int var, int model_preset, const std::string &name, int lang);

    bool RemoveCharaName(const std::string &code, int lang, int index=0);
    bool RemoveAllCharaName(const std::string &code, int lang);
    bool RemoveCharaCostumeName(const std::string &code, int var, int model_preset, int lang);

    bool GetSuperSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetUltimateSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetEvasiveSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetAwakenSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);

    bool SetSuperSkillName(uint16_t name_id, const std::string &name, int lang);
    bool SetUltimateSkillName(uint16_t name_id, const std::string &name, int lang);
    bool SetEvasiveSkillName(uint16_t name_id, const std::string &name, int lang);
    bool SetAwakenSkillName(uint16_t name_id, const std::string &name, int lang);

    bool RemoveSuperSkillName(uint16_t name_id, int lang);
    bool RemoveUltimateSkillName(uint16_t name_id, int lang);
    bool RemoveEvasiveSkillName(uint16_t name_id, int lang);
    bool RemoveAwakenSkillName(uint16_t name_id, int lang);

    bool GetSuperSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool GetUltimateSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool GetEvasiveSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool GetAwakenSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);

    bool SetSuperSkillDesc(uint16_t name_id, const std::string &desc, int lang);
    bool SetUltimateSkillDesc(uint16_t name_id, const std::string &desc, int lang);
    bool SetEvasiveSkillDesc(uint16_t name_id, const std::string &desc, int lang);
    bool SetAwakenSkillDesc(uint16_t name_id, const std::string &desc, int lang);

    bool RemoveSuperSkillDesc(uint16_t name_id, int lang);
    bool RemoveUltimateSkillDesc(uint16_t name_id, int lang);
    bool RemoveEvasiveSkillDesc(uint16_t name_id, int lang);
    bool RemoveAwakenSkillDesc(uint16_t name_id, int lang);

    bool GetBtlHudAwakenName(uint16_t name_id, uint16_t trans_stage, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetBtlHudAwakenName(uint16_t name_id, uint16_t trans_stage, const std::string &name, int lang);
    bool RemoveBtlHudAwakenName(uint16_t name_id, uint16_t trans_stage, int lang);

    uint8_t *GetSelPortrait(const std::string &name, size_t *psize);
    bool SetSelPortrait(const std::string &name, const uint8_t *buf, size_t size);
    bool RemoveSelPortrait(const std::string &name);

    bool GetLobbyName(uint32_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetLobbyName(uint32_t name_id, const std::string &name, int lang=XV2_LANG_ENGLISH);
    bool RemoveLobbyName(uint32_t name_id, int lang);

    bool GetCacCostumeName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetCacCostumeName(uint32_t name_idx, const std::string &name, int lang);
    bool AddCacCostumeName(const std::string &entry_name, const std::string &name, int lang, uint16_t *ret_idx);
    bool RemoveCacCostumeName(uint32_t name_idx, int lang, bool update_idb);

    bool GetAccesoryName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetAccesoryName(uint32_t desc_idx, const std::string &name, int lang);
    bool AddAccesoryName(const std::string &name, int lang, uint16_t *ret_idx);
    bool RemoveAccesoryName(uint32_t name_idx, int lang, bool update_idb);

    bool GetCacCostumeDesc(uint32_t desc_idx, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool SetCacCostumeDesc(uint32_t desc_idx, const std::string &desc, int lang);
    bool AddCacCostumeDesc(const std::string &entry_name, const std::string &desc, int lang, uint16_t *ret_idx);
    bool RemoveCacCostumeDesc(uint32_t desc_idx, int lang, bool update_idb);

    bool GetAccesoryDesc(uint32_t desc_idx, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool SetAccesoryDesc(uint32_t desc_idx, const std::string &desc, int lang);
    bool AddAccesoryDesc(const std::string &desc, int lang, uint16_t *ret_idx);
    bool RemoveAccesoryDesc(uint32_t desc_idx, int lang, bool update_idb);

    bool GetTalismanName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    uint32_t GetBlastFromTalisman(uint32_t talisman_id, bool id2);

    bool GetMaterialName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetBattleName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);

    /* Generic audio functions */
    bool SetAcbAwbData(AcbFile *acb, AwbFile *awb);
    bool SetSound(AcbFile *acb, Afs2File *awb, uint32_t cue_id, HcaFile &hca, bool is_external_awb=true);
    uint32_t SetSound(AcbFile *acb, Afs2File *awb, const std::string &name, HcaFile &hca, bool is_external_awb=true);
    bool FreeSound(AcbFile *acb,  const std::string &name);
    /***************************/

    HcaFile *GetCssSound(uint32_t cue_id, bool english);
    HcaFile *GetCssSound(const std::string &name, bool english);
    bool SetCssSound(uint32_t cue_id, bool english, HcaFile &hca);
    uint32_t SetCssSound(const std::string &name, bool english, HcaFile &hca);
    bool FreeCssSound(const std::string &name, bool english);

    bool SetSevCmnSound(uint32_t cue_id, bool english, HcaFile &hca, bool is_global_cue_id);
    uint32_t SetSevCmnSound(const std::string &name, bool english, HcaFile &hca, bool return_global_cue_id);
    uint32_t GetSevCmnCueId(const std::string &name, bool english, bool *is_sev2, bool return_global_cue_id);
    bool FreeSevCmnSound(const std::string &name, bool english);

    uint32_t CusAuraToAurAura(uint32_t id);

    bool GetStageName(const std::string &id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetStageName(const std::string &id, const std::string &name, int lang);
    bool RemoveStageName(const std::string &id, int lang);

    bool GetTtbSubtitle(const std::string &event_name, std::string &subtitle, int lang=XV2_LANG_ENGLISH);
    bool SetTtbSubtitle(const std::string &event_name, const std::string &subtitle, int lang);
    bool RemoveTtbSubtitle(const std::string &event_name, int lang, bool *existed);

    bool GetTtbVoice(const std::string &event_name, std::string &voice);
    bool SetTtbVoice(const std::string &event_name, const std::string &voice);
    bool RemoveTtbVoice(const std::string &event_name, bool *existed);

    // These may be moved to utils...
    std::string UnescapeHtml(const std::string &str);
    std::string EscapeHtml(const std::string &str);

    // Higher level api
    std::string GetCharaAndCostumeName(const std::string &code, uint32_t costume_id, uint32_t model_preset, int lang=XV2_LANG_ENGLISH);
    std::string GetCharaAndCostumeName(uint32_t char_id, uint32_t costume_id, uint32_t model_preset, bool ignore_unknown, int lang=XV2_LANG_ENGLISH);

    std::string GetCacTopName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacBottomName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacGlovesName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacShoesName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacAccesoryName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetTalismanNameEx(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetMaterialNameEx(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetBattleNameEx(uint16_t item_id, int lang=XV2_LANG_ENGLISH);

    std::string GetSkillDirectory(const CusSkill &skill, std::string *last_part=nullptr);

    std::vector<std::string> GetCostumeNamesForPartSet(uint32_t partset, uint8_t race_lock=0xFF);
    std::string GetCostumeNamesForPartSet2(uint32_t partset, uint8_t race_lock=0xFF);
}

extern Xv2Fs *xv2fs;

#endif // __XENOVERSE2_H__
