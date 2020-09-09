#ifndef CAC2X2M_H
#define CAC2X2M_H

#include <unordered_set>
#include "Xenoverse2.h"
#include "BcsFile.h"
#include "EskFile.h"
#include "X2mFile.h"
#include "EanFile.h"
#include "Xv2SavFile.h"

enum PartType
{
    PART_FACE_BASE,
    PART_FACE_FOREHEAD,
    PART_FACE_EYE,
    PART_FACE_NOSE,
    PART_FACE_EAR,
    PART_HAIR,
    PART_BUST,
    PART_PANTS,
    PART_RIST,
    PART_BOOTS
};

namespace Cac2X2m
{
    bool BuildPart(BcsPart &output, const BcsPart &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name, const std::string &part_name);

    bool BuildFaceBase(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildFaceForehead(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildFaceEye(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildFaceNose(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildFaceEar(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildHair(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildBust(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildPants(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildRist(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool BuildBoots(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);

    bool MergePart(X2mFile *x2m, BcsPart &output, const BcsPart &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &part_name);
    bool MergeAccesory(X2mFile *x2m, BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, bool wig);

    bool BuildPartSet(X2mFile *x2m, BcsPartSet &output, const BcsFile &input, std::unordered_set<std::string> &files, const std::string &base_dir,
                      uint32_t face_base, uint32_t face_forehead, uint32_t face_eye, uint32_t face_nose, uint32_t face_ear,
                      uint32_t hair, uint32_t bust, uint32_t pants, uint32_t rist, uint32_t boots, uint32_t accesory);

    bool AddPartColor(BcsPart &part, const BcsFile &input, const std::string &part_name, uint32_t color);

    bool SetHairColor(BcsPartSet &output, const BcsFile &input, uint32_t color);
    bool SetSkinColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    bool SetEyesColor(BcsPartSet &output, const BcsFile &input, uint32_t color);
    bool SetBustColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    bool SetPantsColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    bool SetRistColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    bool SetBootsColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
    bool SetPaintColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3);

    bool SetColors(BcsPartSet &output, const BcsFile &input,
                   uint32_t skin_color1, uint32_t skin_color2, uint32_t skin_color3, uint32_t skin_color4,
                   uint32_t hair_color, uint32_t eye_color,
                   uint32_t bust_color1, uint32_t bust_color2, uint32_t bust_color3, uint32_t bust_color4,
                   uint32_t pants_color1, uint32_t pants_color2, uint32_t pants_color3, uint32_t pants_color4,
                   uint32_t rist_color1, uint32_t rist_color2, uint32_t rist_color3, uint32_t rist_color4,
                   uint32_t boots_color1, uint32_t boots_color2, uint32_t boots_color3, uint32_t boots_color4,
                   uint32_t paint_color1, uint32_t paint_color2, uint32_t paint_color3);

    bool SetBody(EskFile &output, EanFile &cam_output, const BcsFile &input, uint32_t body_shape);

    bool CopyPartSet(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool CopySSJ(BcsFile &output, const BcsFile &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);
    bool CopyTransforms(BcsFile &output, const BcsFile &input, const std::vector<uint32_t> &trans_sets, const std::vector<uint32_t> &def_sets, uint32_t first_valid, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name);

    bool FixSSJ3(X2mFile *x2m);

    uint32_t BustItem(uint32_t item_id);
    uint32_t PantsItem(uint32_t item_id);
    uint32_t RistItem(uint32_t item_id);
    uint32_t BootsItem(uint32_t item_id);
    uint32_t AccesoryItem(uint32_t item_id);

    bool SetCms(X2mFile *x2m, const std::string &cms_entry, std::unordered_set<std::string> &files, std::string &base_dir, BcsFile &bcs, EskFile &esk, EanFile &cam_ean);
    bool SetCms(X2mFile *x2m, uint32_t cms_entry, std::unordered_set<std::string> &files, std::string &base_dir, BcsFile &bcs, EskFile &esk, EanFile &cam_ean);

    void ResolveCustomSkills(X2mFile *x2m, CusSkillSet &set);

    bool SetCso(uint32_t race, CsoEntry &new_cso, std::unordered_set<std::string> &files, uint32_t voice);
    bool SetCso(const std::string &race, CsoEntry &new_cso, std::unordered_set<std::string> &files, uint32_t voice);

    bool SetPsc(uint32_t race, PscSpecEntry &new_psc, uint32_t body_shape, uint16_t hea, uint16_t ki, uint16_t stm, uint16_t atk, uint16_t str, uint16_t bla, uint32_t talisman);
    bool SetPsc(const std::string &race, PscSpecEntry &new_psc, uint32_t body_shape, uint16_t hea, uint16_t ki, uint16_t stm, uint16_t atk, uint16_t str, uint16_t bla, uint32_t talisman);

    bool SetAura(uint32_t race, AurCharaLink &new_aur);
    bool SetAura(const std::string &race, AurCharaLink &new_aur);

    bool SetCml(uint32_t race, CmlEntry &new_cml, uint32_t body_shape);
    bool SetCml(const std::string &race, CmlEntry &new_cml, uint32_t body_shape);

    bool WriteFiles(X2mFile *x2m, const std::unordered_set<std::string> &files);
    bool WriteCustomFiles(X2mFile *x2m, BcsFile &bcs, EskFile &esk, EanFile &cam_ean);

    bool WriteSelPort(X2mFile *x2m, const std::string &cms_entry);
    bool WriteSelPort(X2mFile *x2m, uint32_t cms_entry);

    bool WriteBtlPort(X2mFile *x2m, const std::string &cms_entry);
    bool WriteBtlPort(X2mFile *x2m, uint32_t cms_entry);

    uint16_t SuperSkillFromId2(uint16_t id2);
    uint16_t UltSkillFromId2(uint16_t id2);
    uint16_t EvaSkillFromId2(uint16_t id2);
    uint16_t BlastSkillFromId2(uint16_t id2);
    uint16_t AwakenSkillFromId2(uint16_t id2);

    bool WriteCssVoice(X2mFile *x2m, uint32_t race, uint32_t voice);
    bool WriteCssVoice(X2mFile *x2m, const std::string &race, uint32_t voice);
}

namespace X2m2Cac
{
    bool GetRace(X2mFile *x2m, std::string &race);
    uint32_t GetRace(X2mFile *x2m);
    int GetVoice(X2mFile *x2m);
    bool GetName(X2mFile *x2m, std::string &name, int lang);

    uint32_t BustItem(uint32_t model, const std::string &race);
    uint32_t PantsItem(uint32_t model, const std::string &race);
    uint32_t RistItem(uint32_t model, const std::string &race);
    uint32_t BootsItem(uint32_t model, const std::string &race);
    uint32_t AccesoryItem(uint32_t model, const std::string &race);

    bool GetColor(const BcsFile &bcs, const BcsPart &part, const std::string &part_name, uint32_t *color);

    bool GetPart(X2mFile *x2m, const BcsFile &race_bcs, uint32_t slot_idx, uint32_t part_idx, uint32_t *part_out, uint32_t *colors_out, bool *is_accessory_out);
    bool FindAccessory(X2mFile *x2m, const BcsFile &race_bcs, uint32_t slot_idx, uint32_t *accessory);

    uint32_t GetTalisman(X2mFile *x2m, uint32_t slot_idx);

    uint32_t GetSkill(X2mFile *x2m, uint32_t slot_idx, uint32_t skill_idx, bool format2);
    bool GetStats(X2mFile *x2m, uint32_t slot_idx, uint16_t *hea, uint16_t *ki, uint16_t *stm, uint16_t *atk, uint16_t *str, uint16_t *bla);
}

#endif // CAC2X2M_H
