#ifndef XV2INS_COMMON_H
#define XV2INS_COMMON_H

#define PROGRAM_NAME_INSTALLER  "XV2 Mods Installer"
#define PROGRAM_NAME_CHARACREAT "XV2 Chara Creator"
#define PROGRAM_NAME_SKILLCREAT "XV2 Skill Creator"
#define PROGRAM_NAME_COSTCREAT  "XV2 Costume Creator"
#define PROGRAM_NAME_STAGECREAT "XV2 Stage Creator"
#define PROGRAM_NAME_QUESTCREAT "XV2 Quest Creator"
#define PROGRAM_VERSION "3.4"

#define INSTALLED_MODS_PATH "XV2INS/Installed"

#define MINIMUM_EXE_VERSION_REQUIRED  1.14f
#define MINIMUM_PATCHER_REQUIRED    3.4f

#define XV2_PATCHER_AS3_TAG         "XV2_PATCHER_AS3_V06"
#define XV2_PATCHER_AS3_TAG_CHARA   "XV2_PATCHER_AS3_V05"
#define XV2_PATCHER_AS3_TAG_STAGE   "XV2_PATCHER_AS3_V06"

bool Bootstrap(bool multiple_hci, bool installer_mode, bool *needs_update=nullptr);



#endif // XV2INS_COMMON_H
