#include <windows.h>
#include <math.h>
#include <cmath>
#include <algorithm>
#include "PatchUtils.h"
#include "PscFile.h"
#include "Xv2PreBakedFile.h"
#include "Xv2PatcherSlotsFile.h"
#include "xv2patcher.h"
#include "chara_patch.h"
#include "debug.h"

#include "Misc/IggyFile.h"

#include <unordered_set>
#include <unordered_map>

#define BEHAVIOUR_MAX	25

typedef void (* SendToAS3Type)(void *, int32_t, uint64_t);
typedef void (*Func1Type)(void *, uint32_t, uint32_t, void *, void *);
typedef void (*Func2Type)(void *, uint32_t, uint32_t, void *);
typedef int (* CheckUnlockType)(void *, int32_t, int32_t);
typedef uint64_t (* SetBodyShapeType)(void *, int32_t, uint32_t, float);

/*typedef uint64_t (* ResultPortraitsType)(uint64_t arg_rcx, uint64_t arg_rdx, uint32_t is_cac, uint32_t cms_entry, 
									uint64_t arg_20, uint64_t arg_28, uint64_t arg_30, uint64_t arg_38, uint64_t arg_40, uint64_t arg_48, uint64_t arg_50, uint64_t arg_58, uint64_t arg_60, uint64_t arg_68,
									uint64_t arg_70, uint64_t arg_78, uint64_t arg_80, uint64_t arg_88, uint64_t arg_90, uint64_t arg_98, uint64_t arg_A0, uint64_t arg_A8, uint64_t arg_B0, uint64_t arg_B8,
									uint64_t arg_C0, uint64_t arg_C8, uint64_t arg_D0, uint64_t arg_D8, uint64_t arg_E0, uint64_t arg_E8, uint64_t arg_F0, uint64_t arg_F8, uint64_t arg_100, uint64_t arg_108,
									uint64_t arg_110, uint64_t arg_118, uint64_t arg_120, uint64_t arg_128, uint64_t arg_130, uint64_t arg_138, uint64_t arg_140, uint64_t arg_148, uint64_t arg_150, uint64_t arg_158,
									uint64_t arg_160, uint64_t arg_168, uint64_t arg_170, uint64_t arg_178, uint64_t arg_180, uint64_t arg_188, uint64_t arg_190, uint64_t arg_198, uint64_t arg_1A0, uint64_t arg_1A8);*/
									
//typedef uint64_t (* ResultPortraitsType)(uint64_t arg_rcx, uint64_t arg_rdx, uint32_t is_cac, uint32_t cms_entry);
typedef uint64_t (* ResultPortraits2Type)(Battle_HudCockpit *pthis, int idx, void *r8, void *r9, void *arg5, void *arg6, void *arg7, void *arg8);
typedef uint32_t (* Behaviour10FuncType)(void *, uint32_t, void *);
typedef void (* ApplyCacMatsType)(uint64_t *, uint32_t, const char *);


// These constants are the ones in CharaSele.as
// I have kept their names despite in native code, receives are actually sends

static constexpr const int PlayerNumFri = 2;
static constexpr const int SkillMax = 8;
static constexpr const int CharaVarIndexNum = 32;
static constexpr const int CharacterMax = 128;
static constexpr const int CustomListMax = 256;
static constexpr const int CharacterTableData = 12;  // 1.14: 11->12
static constexpr const int CharacterTableMax = 512; // 1.10: 350->512
static constexpr const int ReceiveType_FlagUseCancel = 0;
static constexpr const int ReceiveType_PlayerFriNum = 1;
static constexpr const int ReceiveType_PlayerEnmNum = 2;
static constexpr const int ReceiveType_PartyNpcNum = 3;
static constexpr const int ReceiveType_FlagSelectAvatar = 4;
static constexpr const int ReceiveType_FlagLocalBattle = 5;
static constexpr const int ReceiveType_Flag2pController = 6;
static constexpr const int ReceiveType_Str2pController = 7;
static constexpr const int ReceiveType_Time = 8;
static constexpr const int ReceiveType_CharaNameStr = 9;
static constexpr const int ReceiveType_NameOption_GK2 = 10;
static constexpr const int ReceiveType_NameOption_LvText = 11; // 1.10: new
static constexpr const int ReceiveType_NameOption_LvNum = 12; // 1.10: new
static constexpr const int ReceiveType_VariationNameStr = 13; // 1.10: 11->13
static constexpr const int ReceiveType_TarismanHeaderStr = 14; // 1.10: 12->14
static constexpr const int ReceiveType_TarismanNameStr = 15; // 1.10: 13->15
static constexpr const int ReceiveType_ImageStrStart = 16; ; // 1.10: 14->16
static constexpr const int ReceiveType_ImageStrEnd = ReceiveType_ImageStrStart + CharacterMax - 1; // 0x8F (pre 1.10: 0x8D)
static constexpr const int ReceiveType_UnlockVarStart = ReceiveType_ImageStrEnd + 1; // 0x90 (pre 1.10: 0x8E)
static constexpr const int ReceiveType_UnlockVarEnd = ReceiveType_UnlockVarStart + CharaVarIndexNum * CharacterMax - 1; // 0x108F (pre 1.10: 0x108D)
static constexpr const int ReceiveType_KeyStrL1 = ReceiveType_UnlockVarEnd + 1; // 0x1090 (pre 1.10: 0x108E)
static constexpr const int ReceiveType_KeyStrR1 = ReceiveType_KeyStrL1 + 1; // 0x1091 (pre 1.10: 0x108F)
static constexpr const int ReceiveType_KeyStrL2 = ReceiveType_KeyStrR1 + 1; // 0x1092 (pre 1.10: 0x1090)
static constexpr const int ReceiveType_KeyStrR2 = ReceiveType_KeyStrL2 + 1; // 0x1093 (pre 1.10: 0x1091)
static constexpr const int ReceiveType_KeyStrRU = ReceiveType_KeyStrR2 + 1; // 0x1094 (pre 1.10: 0x1092)
static constexpr const int ReceiveType_KeyStrRD = ReceiveType_KeyStrRU + 1; // 0x1095 (pre 1.10: 0x1093)
static constexpr const int ReceiveType_KeyStrRL = ReceiveType_KeyStrRD + 1; // 0x1096 (pre 1.10: 0x1094)
static constexpr const int ReceiveType_KeyStrRR = ReceiveType_KeyStrRL + 1; // 0x1097 (pre 1.10: 0x1095)
static constexpr const int ReceiveType_KeyStrSingleLS = ReceiveType_KeyStrRR + 1; // 0x1098 (pre 1.10: 0x1096)
static constexpr const int ReceiveType_KeyStrSingleRS = ReceiveType_KeyStrSingleLS + 1; // 0x1099 (pre 1.10: 0x1097)
static constexpr const int ReceiveType_KeyStrSingleU = ReceiveType_KeyStrSingleRS + 1; // 0x109A (pre 1.10: 0x1098)
static constexpr const int ReceiveType_KeyStrSingleD = ReceiveType_KeyStrSingleU + 1; // 0x109B (ptr 1.10: 0x1099)
static constexpr const int ReceiveType_KeyStrSingleL = ReceiveType_KeyStrSingleD + 1; // 0x109C (pre 1.10: 0x109A)
static constexpr const int ReceiveType_KeyStrSingleR = ReceiveType_KeyStrSingleL + 1; // 0x109D (pre 1.10: 0x109B)
static constexpr const int ReceiveType_SkillNameStrStart = ReceiveType_KeyStrSingleR + 1; // 0x109E (pre 1.10: 0x109C)
static constexpr const int ReceiveType_SkillNameStrEnd = ReceiveType_SkillNameStrStart + SkillMax - 1; // 0x10A5 (pre 1.10: 0x10A3)
static constexpr const int ReceiveType_ImageStrNpcStart = ReceiveType_SkillNameStrEnd + 1; // 0x10A6 (pre 1.10: 0x10A4)
static constexpr const int ReceiveType_ImageStrNpcEnd = ReceiveType_ImageStrNpcStart + PlayerNumFri - 1; // 0x10A7 (pre 1.10: 0x10A5)
static constexpr const int ReceiveType_CharaSelectedStart = ReceiveType_ImageStrNpcEnd + 1; // 0x10A8 (pre 1.10: 0x10A6) 
static constexpr const int ReceiveType_CharaSelectedEnd = ReceiveType_CharaSelectedStart + CharacterMax - 1; // 0x1127 (pre 1.10: 0x1125)
static constexpr const int ReceiveType_CharaVariationStart = ReceiveType_CharaSelectedEnd + 1; // 0x1128 (pre 1.10: 0x1126)
static constexpr const int ReceiveType_CharaVariationEnd = ReceiveType_CharaVariationStart + CharaVarIndexNum - 1; // 0x1147 (pre 1.10: 0x1145)
static constexpr const int ReceiveType_DLCUnlockFlag = ReceiveType_CharaVariationEnd + 1; // 0x1148 (pre 1.10: 0x1146) 
static constexpr const int ReceiveType_JoyConSingleFlag = ReceiveType_DLCUnlockFlag + 1; // 0x1149 (pre 1.10: 0x1147)
static constexpr const int ReceiveType_WaitLoadNum = ReceiveType_JoyConSingleFlag; // 0x1149 (pre 1.10: 0x1147)

static constexpr const int ReceiveType_CostumeNum = ReceiveType_JoyConSingleFlag + 1; // 0x114A (pre 1.10: 0x1148)
static constexpr const int ReceiveType_CharacterTableStart = ReceiveType_CostumeNum + 1; // 0x114B (pre 1.10: 0x1149)
static constexpr const int ReceiveType_CostumeID = ReceiveType_CharacterTableStart; // 0x114B (pre 1.10: 0x1149)
static constexpr const int ReceiveType_CID = ReceiveType_CostumeID + 1; // 0x114C (pre 1.10: 0x114A)
static constexpr const int ReceiveType_MID = ReceiveType_CID + 1; // 0x114D (pre 1.10: 0x114B)
static constexpr const int ReceiveType_PID = ReceiveType_MID + 1; // 0x114E (pre 1.10: 0x114C)
static constexpr const int ReceiveType_UnlockNum = ReceiveType_PID + 1; // 0x114F (pre 1.10: 0x114D)
static constexpr const int ReceiveType_Gokuaku = ReceiveType_UnlockNum + 1; // 0x1150 (pre 1.10: 0x114E)
static constexpr const int ReceiveType_SelectVoice1 = ReceiveType_Gokuaku + 1; // 0x1151 (pre 1.10: 0x114F)
static constexpr const int ReceiveType_SelectVoice2 = ReceiveType_SelectVoice1 + 1; // 0x1152 (pre 1.10: 0x1150)
static constexpr const int ReceiveType_DlcKey = ReceiveType_SelectVoice2 + 1; // 0x1153 (pre 1.10: 0x1151)
static constexpr const int ReceiveType_CustomCostume = ReceiveType_DlcKey + 1; // 0x1154 (pre 1.10: 0x1152)
static constexpr const int ReceiveType_AvatarSlotID = ReceiveType_CustomCostume + 1; // 0x1155 (new in 1.10)
static constexpr const int ReceiveType_AfterTU9Order = ReceiveType_AvatarSlotID + 1; // 0x1156 (new in 1.14)
static constexpr const int ReceiveType_CharacterTableEnd = ReceiveType_CharacterTableStart + CharacterTableMax * CharacterTableData; // 0x294B (pre 1.14: 0x274B)

static constexpr const int ReceiveType_UseCustomList = ReceiveType_CharacterTableEnd + 1; // 0x294C (pre 1.14: 0x274C)
static constexpr const int ReceiveType_CustomListNum = ReceiveType_UseCustomList + 1; // 0x294D (pre 1.14: 0x274D)
static constexpr const int ReceiveType_CustomList_CID_Start = ReceiveType_CustomListNum + 1; // 0x294E (pre 1.14: 0x274E)
static constexpr const int ReceiveType_CustomList_CID_End = ReceiveType_CustomList_CID_Start + CustomListMax - 1; // 0x2A4D (pre 1.14: 0x284D)
static constexpr const int ReceiveType_CustomList_MID_Start = ReceiveType_CustomList_CID_End + 1; // 0x2A4E (pre 1.14: 0x284E)
static constexpr const int ReceiveType_CustomList_MID_End = ReceiveType_CustomList_MID_Start + CustomListMax - 1; // 0x2B4D (pre 1.14: 0x294D)
static constexpr const int ReceiveType_CustomList_PID_Start = ReceiveType_CustomList_MID_End + 1; // 0x2B4E (pre 1.14: 0x294E)
static constexpr const int ReceiveType_CustomList_PID_End = ReceiveType_CustomList_PID_Start + CustomListMax - 1; // 0x2C4D (pre 1.14: 0x2A4D)
static constexpr const int ReceiveType_CustomList_PartnerJudge_Start = ReceiveType_CustomList_PID_End + 1; // 0x2C4E (pre 1.14: 0x2A4E)
static constexpr const int ReceiveType_CustomList_PartnerJudge_End = ReceiveType_CustomList_PartnerJudge_Start + CustomListMax - 1; // 0x2D4D (pre 1.14: 0x2B4D)
static constexpr const int ReceiveType_Num = ReceiveType_CustomList_PartnerJudge_End + 1; // 0x2D4E (pre 1.14: 0x2B4E)

static_assert(ReceiveType_Num == 0x2D4E, "Error");

extern "C" 
{
	
static int n_CharacterMax;
static int n_ReceiveType_ImageStrEnd; 
static int n_ReceiveType_UnlockVarStart;
static int n_ReceiveType_UnlockVarEnd;
static int n_ReceiveType_KeyStrL1;
static int n_ReceiveType_KeyStrR1;
static int n_ReceiveType_KeyStrL2;
static int n_ReceiveType_KeyStrR2;
static int n_ReceiveType_KeyStrRU;
static int n_ReceiveType_KeyStrRD;
static int n_ReceiveType_KeyStrRL;
static int n_ReceiveType_KeyStrRR;
static int n_ReceiveType_KeyStrSingleLS;
static int n_ReceiveType_KeyStrSingleRS;   
static int n_ReceiveType_KeyStrSingleU;
static int n_ReceiveType_KeyStrSingleD;    
static int n_ReceiveType_KeyStrSingleL;
static int n_ReceiveType_KeyStrSingleR;
static int n_ReceiveType_SkillNameStrStart;
static int n_ReceiveType_SkillNameStrEnd;
static int n_ReceiveType_ImageStrNpcStart;
static int n_ReceiveType_ImageStrNpcEnd;
static int n_ReceiveType_CharaSelectedStart;
static int n_ReceiveType_CharaSelectedEnd;
static int n_ReceiveType_CharaVariationStart;
static int n_ReceiveType_CharaVariationEnd;
static int n_ReceiveType_DLCUnlockFlag;
static int n_ReceiveType_JoyConSingleFlag;
static int n_ReceiveType_WaitLoadNum;

static int n_ReceiveType_CostumeNum; 
static int n_ReceiveType_CharacterTableStart;
static int n_ReceiveType_CostumeID;
static int n_ReceiveType_CID;
static int n_ReceiveType_MID;
static int n_ReceiveType_PID;
static int n_ReceiveType_UnlockNum;
static int n_ReceiveType_Gokuaku;
static int n_ReceiveType_SelectVoice1;
static int n_ReceiveType_SelectVoice2;
static int n_ReceiveType_DlcKey;
static int n_ReceiveType_CustomCostume;
static int n_ReceiveType_AvatarSlotID;
static int n_ReceiveType_AfterTU9Order;
static int n_ReceiveType_CharacterTableEnd;

static int n_ReceiveType_UseCustomList;
static int n_ReceiveType_CustomListNum;
static int n_ReceiveType_CustomList_CID_Start;
static int n_ReceiveType_CustomList_CID_End;
static int n_ReceiveType_CustomList_MID_Start;
static int n_ReceiveType_CustomList_MID_End;
static int n_ReceiveType_CustomList_PID_Start;
static int n_ReceiveType_CustomList_PID_End;
static int n_ReceiveType_CustomList_PartnerJudge_Start;
static int n_ReceiveType_CustomList_PartnerJudge_End;
static int n_ReceiveType_Num;

static std::string x2s_raw;

static SendToAS3Type SendToAS3;
static SendToAS3Type SendToAS3_2;
static SendToAS3Type SendToAS3_3;
static Func1Type func1;
static Func2Type func2;
static CheckUnlockType check_unlock;
static SetBodyShapeType SetBodyShape;
//static ResultPortraitsType ResultPortraits;
static ResultPortraits2Type ResultPortraits2;
static Behaviour10FuncType Behaviour10Func;
static ApplyCacMatsType ApplyCacMats;


static uint8_t *find_blob_string(uint8_t *blob, size_t size, const char *str)
{
	size_t len = strlen(str);
	size_t max_search = size - len;
	
	for (size_t i = 0; i <= max_search; i++)
	{
		if (memcmp(blob+i, str, strlen(str)) == 0)
		{
			return blob+i;
		}
	}
	
	return nullptr;
}

static uint32_t guess_character_max()
{
	const std::string iggy_path = myself_path + CONTENT_ROOT + "data/ui/iggy/CHARASELE.iggy";
	IggyFile iggy;
	
	if (!iggy.LoadFromFile(iggy_path, false))
	{
		DPRINTF("Auto character max assumes %d because there is no custom iggy file. (assuming too the one in cpk is untouched).\n", CharacterMax);
		return CharacterMax;
	}
	
	uint32_t size;
	uint8_t *abc_blob = iggy.GetAbcBlob(&size);
	
	if (!abc_blob)
	{
		DPRINTF("Auto character max assumes %d because it was not able to extract actionscript from iggy.\n", CharacterMax);
		return CharacterMax;
	}
	
	if (!find_blob_string(abc_blob, size, "AvatarSlotID"))
	{
		UPRINTF("The version of CHARASELE.iggy at data/ui/iggy is not longer compatible with this game version.\n"
				"Delete your old mods install and update your mods/tools to compatible versions.\n");
				
		exit(-1);
	}
	
	uint8_t *ptr = abc_blob;
	for (uint32_t i = 0; i < 0x40; i++, ptr++)
	{
		uint64_t double_val = *(uint64_t *)ptr;
		
		if (double_val == 0x4018000000000000) // 6.0
		{
			// Read next double
			double ImageStrEnd_double = *(double *)(ptr+8);
			
			if (!std::isfinite(ImageStrEnd_double) || ImageStrEnd_double < 23.0 || ImageStrEnd_double > 100013.0)
			{
				DPRINTF("Auto detecting character max failed. We'll asume the default of %d.\n", CharacterMax);
				return CharacterMax;
			}
			
			uint32_t ret = lrint(ImageStrEnd_double) - 15;
			DPRINTF("Auto character max has been estimated: %d\n", ret);
			return ret;
		}
	}
	
	DPRINTF("Auto detecting character max failed (2). We'll asume the default of %d.\n", CharacterMax);
	return CharacterMax;
}

static void ReadSlotsFile()
{
	static time_t mtime = 0;
	const std::string path = myself_path + CONTENT_ROOT + SLOTS_FILE;
	time_t current_mtime;
	
	if (Utils::GetFileDate(path, &current_mtime) && current_mtime != mtime)
	{
		Utils::ReadTextFile(path, x2s_raw, false);
		mtime = current_mtime;
	}
}

PUBLIC void CharaSetup(SendToAS3Type orig)
{
	SendToAS3 = orig;
	
	ini.GetIntegerValue("NewChara", "CharacterMax", &n_CharacterMax, CharacterMax);
	
	if (n_CharacterMax == 0)
	{
		n_CharacterMax = guess_character_max();
	}		
	
	ReadSlotsFile();

	n_ReceiveType_ImageStrEnd = ReceiveType_ImageStrStart + n_CharacterMax - 1; 
	n_ReceiveType_UnlockVarStart = n_ReceiveType_ImageStrEnd + 1;
	n_ReceiveType_UnlockVarEnd = n_ReceiveType_UnlockVarStart + CharaVarIndexNum * n_CharacterMax - 1;
	n_ReceiveType_KeyStrL1 = n_ReceiveType_UnlockVarEnd + 1;
	n_ReceiveType_KeyStrR1 = n_ReceiveType_KeyStrL1 + 1;
	n_ReceiveType_KeyStrL2 = n_ReceiveType_KeyStrR1 + 1;
	n_ReceiveType_KeyStrR2 = n_ReceiveType_KeyStrL2 + 1;
	n_ReceiveType_KeyStrRU = n_ReceiveType_KeyStrR2 + 1;
	n_ReceiveType_KeyStrRD = n_ReceiveType_KeyStrRU + 1;
	n_ReceiveType_KeyStrRL = n_ReceiveType_KeyStrRD + 1;
	n_ReceiveType_KeyStrRR = n_ReceiveType_KeyStrRL + 1;
	n_ReceiveType_KeyStrSingleLS = n_ReceiveType_KeyStrRR + 1;
	n_ReceiveType_KeyStrSingleRS = n_ReceiveType_KeyStrSingleLS + 1;
	n_ReceiveType_KeyStrSingleU = n_ReceiveType_KeyStrSingleRS + 1;
	n_ReceiveType_KeyStrSingleD = n_ReceiveType_KeyStrSingleU + 1;    
	n_ReceiveType_KeyStrSingleL = n_ReceiveType_KeyStrSingleD + 1;
	n_ReceiveType_KeyStrSingleR = n_ReceiveType_KeyStrSingleL + 1;
	n_ReceiveType_SkillNameStrStart = n_ReceiveType_KeyStrSingleR + 1;
	n_ReceiveType_SkillNameStrEnd = n_ReceiveType_SkillNameStrStart + SkillMax - 1;
	n_ReceiveType_ImageStrNpcStart = n_ReceiveType_SkillNameStrEnd + 1;
	n_ReceiveType_ImageStrNpcEnd = n_ReceiveType_ImageStrNpcStart + PlayerNumFri - 1;
	n_ReceiveType_CharaSelectedStart = n_ReceiveType_ImageStrNpcEnd + 1;
	n_ReceiveType_CharaSelectedEnd = n_ReceiveType_CharaSelectedStart + n_CharacterMax - 1;
	n_ReceiveType_CharaVariationStart = n_ReceiveType_CharaSelectedEnd + 1;
	n_ReceiveType_CharaVariationEnd = n_ReceiveType_CharaVariationStart + CharaVarIndexNum - 1;
	n_ReceiveType_DLCUnlockFlag = n_ReceiveType_CharaVariationEnd + 1;	
	n_ReceiveType_JoyConSingleFlag = n_ReceiveType_DLCUnlockFlag + 1;
	n_ReceiveType_WaitLoadNum = n_ReceiveType_JoyConSingleFlag; 	
	
	n_ReceiveType_CostumeNum = n_ReceiveType_JoyConSingleFlag + 1; 
	n_ReceiveType_CharacterTableStart = n_ReceiveType_CostumeNum + 1; 
	n_ReceiveType_CostumeID = n_ReceiveType_CharacterTableStart; 
	n_ReceiveType_CID = n_ReceiveType_CostumeID + 1; 
	n_ReceiveType_MID = n_ReceiveType_CID + 1; 
	n_ReceiveType_PID = n_ReceiveType_MID + 1;  
	n_ReceiveType_UnlockNum = n_ReceiveType_PID + 1; 
	n_ReceiveType_Gokuaku = n_ReceiveType_UnlockNum + 1; 
	n_ReceiveType_SelectVoice1 = n_ReceiveType_Gokuaku + 1;
	n_ReceiveType_SelectVoice2 = n_ReceiveType_SelectVoice1 + 1; 
	n_ReceiveType_DlcKey = n_ReceiveType_SelectVoice2 + 1; 
	n_ReceiveType_CustomCostume = n_ReceiveType_DlcKey + 1; 
	n_ReceiveType_AvatarSlotID = n_ReceiveType_CustomCostume + 1;
	n_ReceiveType_AfterTU9Order = n_ReceiveType_AvatarSlotID + 1;
	n_ReceiveType_CharacterTableEnd = n_ReceiveType_CharacterTableStart + CharacterTableMax * CharacterTableData; 
	
	n_ReceiveType_UseCustomList = n_ReceiveType_CharacterTableEnd + 1;	
	n_ReceiveType_CustomListNum = n_ReceiveType_UseCustomList + 1; 
	n_ReceiveType_CustomList_CID_Start = n_ReceiveType_CustomListNum + 1; 
	n_ReceiveType_CustomList_CID_End = n_ReceiveType_CustomList_CID_Start + CustomListMax - 1; 
	n_ReceiveType_CustomList_MID_Start = n_ReceiveType_CustomList_CID_End + 1; 
	n_ReceiveType_CustomList_MID_End = n_ReceiveType_CustomList_MID_Start + CustomListMax - 1; 
	n_ReceiveType_CustomList_PID_Start = n_ReceiveType_CustomList_MID_End + 1; 
	n_ReceiveType_CustomList_PID_End = n_ReceiveType_CustomList_PID_Start + CustomListMax - 1; 
	n_ReceiveType_CustomList_PartnerJudge_Start = n_ReceiveType_CustomList_PID_End + 1; 
	n_ReceiveType_CustomList_PartnerJudge_End = n_ReceiveType_CustomList_PartnerJudge_Start + CustomListMax - 1; 
	n_ReceiveType_Num = n_ReceiveType_CustomList_PartnerJudge_End + 1; 
	
	//DPRINTF("%x - %x, %x - %x\n", n_ReceiveType_UnlockVarStart, n_ReceiveType_UnlockVarEnd, n_ReceiveType_CharaVariationStart, n_ReceiveType_CharaVariationEnd);	
}

PUBLIC void AS3Send2Setup(SendToAS3Type orig)
{
	SendToAS3_2 = orig;
}

PUBLIC void AS3Send3Setup(SendToAS3Type orig)
{
	SendToAS3_3 = orig;
}

PUBLIC void PatchReceiveTypeKey(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_KeyStrL1 || code > ReceiveType_KeyStrSingleR)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeKey. Code=0x%x\n", code);
	}
	else
	{	
		code = (code - ReceiveType_KeyStrL1) + n_ReceiveType_KeyStrL1;
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeCharaSelected(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CharaSelectedStart || code >= (ReceiveType_CharaSelectedStart+n_CharacterMax))
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCharaSelected. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CharaSelectedStart) + n_ReceiveType_CharaSelectedStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeVariation(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CharaVariationStart || code > ReceiveType_CharaVariationEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeVariation. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CharaVariationStart) + n_ReceiveType_CharaVariationStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeSkillName(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_SkillNameStrStart || code > ReceiveType_SkillNameStrEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeSkillName. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_SkillNameStrStart) + n_ReceiveType_SkillNameStrStart;
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeImageStr(void *pthis, int32_t code, uint64_t data)
{
	// This function doesn't really need patch, but we are gonna check limits anyway
	if (code < ReceiveType_ImageStrStart || code >= (ReceiveType_ImageStrStart+n_CharacterMax))
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeImageStr. Code=0x%x\n", code);
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeImageStrNpc(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_ImageStrNpcStart || code > ReceiveType_ImageStrNpcEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeImageStrNpc. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_ImageStrNpcStart) + n_ReceiveType_ImageStrNpcStart;
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeUnlockVar(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_UnlockVarStart || code >= (ReceiveType_UnlockVarStart + CharaVarIndexNum * n_CharacterMax))
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeUnlockVar. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_UnlockVarStart) + n_ReceiveType_UnlockVarStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeDLCUnlockFlag(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_DLCUnlockFlag)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeDLCUnlockFlag. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_DLCUnlockFlag) + n_ReceiveType_DLCUnlockFlag;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeUseCustomList(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_UseCustomList)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeUseCustomList. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_UseCustomList) + n_ReceiveType_UseCustomList;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchJoyConSingleFlag(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_JoyConSingleFlag)
	{
		DPRINTF("ERROR: unexpected code at PatchJoyConSingleFlag. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_JoyConSingleFlag) + n_ReceiveType_JoyConSingleFlag;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeCustomListNum(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_CustomListNum)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCustomListNum. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomListNum) + n_ReceiveType_CustomListNum;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_CID_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_CID_Start || code >  ReceiveType_CustomList_CID_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveType_CustomList_CID_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_CID_Start) + n_ReceiveType_CustomList_CID_Start;
	}
	
	SendToAS3_3(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_MID_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_MID_Start || code >  ReceiveType_CustomList_MID_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCustomList_MID_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_MID_Start) + n_ReceiveType_CustomList_MID_Start;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_PID_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_PID_Start || code >  ReceiveType_CustomList_PID_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCustomList_PID_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_PID_Start) + n_ReceiveType_CustomList_PID_Start;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_PartnerJudge_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_PartnerJudge_Start || code > ReceiveType_CustomList_PartnerJudge_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveType_CustomList_PartnerJudge_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_PartnerJudge_Start) + n_ReceiveType_CustomList_PartnerJudge_Start;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CostumeNum(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_CostumeNum)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveType_CostumeNum. Code=0x%x\n", code);		
	}
	else
	{
		code = n_ReceiveType_CostumeNum;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CharacterTableStart(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CharacterTableStart || code > ReceiveType_CharacterTableEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveType_CharacterTableStart. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CharacterTableStart) + n_ReceiveType_CharacterTableStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CharacterTableStart2(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CharacterTableStart || code > ReceiveType_CharacterTableEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveType_CharacterTableStart2. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CharacterTableStart) + n_ReceiveType_CharacterTableStart;
	}
	
	SendToAS3_3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeNum(uint32_t *num)
{
	uint32_t old = *num;	
	PatchUtils::Write32(num, n_ReceiveType_Num);
	
	DPRINTF("ReceiveTypeNum sucesfully changed from %d to %d\n", old, *num);
}

PUBLIC void IncreaseChaselMemory(uint32_t *psize)
{
	uint32_t old = *psize;
	PatchUtils::Write32(psize, Utils::Align2(0x18C0+(n_CharacterMax*32), 0x100));
	
	DPRINTF("Chasel object memory succesfully changed from 0x%x to 0x%x\n", old, *psize);
}

PUBLIC void SetupIncreaseChaselSlotsArray(Func1Type orig)
{
	func1 = orig;
}

/*void TestBreakpoint(void *pc, void *addr)
{
	DPRINTF("Accesed at address %p (rel %Ix)\n", pc, RelAddress(pc));
	DPRINTF("Address that was accesed: %p (rel %Ix)\n", addr, RelAddress(addr));
	
	exit(-1);
}*/

PUBLIC void IncreaseChaselSlotsArray(void *arg1, uint32_t arg2, uint32_t size, void *arg4, void *arg5)
{
	/*uint8_t *arg1_8 = (uint8_t *)arg1;
	arg1_8 = arg1_8 - 0x18B0;
	if (!PatchUtils::SetMemoryBreakpoint(arg1_8+0x5E0, 0x1000, TestBreakpoint))
	{
		DPRINTF("Failed to set memory breakpoint.\n");
		exit(-1);
	}*/
	
	size = n_CharacterMax;	
	func1(arg1, arg2, size, arg4, arg5);
}

PUBLIC void SetupIncreaseChaselSlotsArray2(Func2Type orig)
{
	func2 = orig;
}

PUBLIC void IncreaseChaselSlotsArray2(void *arg1, uint32_t arg2, uint32_t size, void *arg4)
{
	size = n_CharacterMax;
	func2(arg1, arg2, size, arg4);	
}

// This irrgular patch is for 1.10v2 due to the 0x5E0 = 0x2F << 5 optimization. It will likely be gone in next version if the address 0x5E0 changes.
// When it happens, delete this, and the asm code.
extern void ModifyArrayOffset4Asm();
PUBLIC void ModifyArrayOffset4(uint8_t *buf)
{
	PatchUtils::Write16((uint16_t *)buf, 0xB848); // mov rax, XXXXXXXXXXXXXX
	
	uintptr_t asm_addr = (uintptr_t) ModifyArrayOffset4Asm;
	PatchUtils::Write64((uint64_t *)(buf+2), asm_addr);
	PatchUtils::Write16((uint16_t *)(buf+10), 0xE0FF); // jmp rax
	PatchUtils::Write32(buf+12, 0x90909090); // nops  (not really necessary, code wont reach here, but to not destroy asm view in a debugger)
	
	uint64_t *ret_addr = (uint64_t *)(asm_addr+0x1B);
	if (*ret_addr != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in ModifyArrayOffset4\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr, (uint64_t)(buf+0x10)); // buf+0x10 -> address to return to
}

static inline bool is_original_playable_char(int32_t chara)
{
	// Mira final form (0.41 fix)
	if (chara == 0x7D)
		return false;
	
	// between goku and  pan
	if (chara >= 0 && chara <= 0x2A)
		return true;
	
	// between san-shinron and satan
	if (chara >= 0x30 && chara <= 0x34)
		return true;
	
	// between buu small and raspberry
	if (chara >= 0x3A && chara <= 0x3C)
		return true;
	
	// between mira and towa
	if (chara >= 0x3E && chara <= 0x3F)
		return true;
	
	// whis or jaco or cell first form
	if (chara == 0x41 || chara == 0x43 || chara == 0x4B)
		return true;
	
	// between Goku SSGSS and Janemba
	if (chara >= 0x50 && chara <= 0x58)
		return true;
	
	// Races
	if (chara >= 0x64 && chara <= 0x6C)
		return true;
	
	// Gogeta - Hit
	if (chara >= 0x78 && chara <= 0x7E)
		return true;
	
	// Janemba or Goku black or future trunks
	if (chara >= 0x82 && chara <= 0x84)
		return true;
	
	return false;
}

PUBLIC void SetupCheckUnlockFunction(CheckUnlockType orig)
{
	check_unlock = orig;
}

PUBLIC int UnlockCharaMods(void *pthis, int32_t chara, int32_t var)
{
	if (is_original_playable_char(chara))
		return check_unlock(pthis, chara, var);
	
	//DPRINTF("Not original, returning 1 for 0x%x\n", chara);
	
	return 1;
}

// This function is an imitation of the DIMPS one, but without the 16 bits calculation overflow.
// 1.08: this is not longer needed, commenting it
/*
PUBLIC void PscInitReplacement(uint8_t *psc_buf)
{
	if (*psc_buf == 0x21) // Already processed
		return;
		
	if (memcmp(psc_buf+1, "PSC", 3) != 0)
		return;
	
	PSCHeader *hdr = (PSCHeader *)psc_buf;
	
	if (hdr->endianess_check != 0xFFFE)
		return;
	
	PSCEntry *file_entries = (PSCEntry *)(psc_buf+hdr->header_size);
    PSCSpecEntry *file_specs = (PSCSpecEntry *)(file_entries+hdr->num_entries);
	
	for (uint32_t i = 0; i < hdr->num_entries; i++)
	{
		file_entries[i].unk_08 = Utils::DifPointer(file_specs, &file_entries[i]);
		file_specs += file_entries[i].num_specs;
	}
	
	*psc_buf = 0x21; // Set as processed
}*/

const std::string &GetSlotsData()
{
	ReadSlotsFile();
	return x2s_raw;
}

// XV2Patcher extensions

static std::vector<std::string> ozarus = { "OSV", "OSB", "OBB", "OSG", "OSN" }; // The original list, as it is in game (including non-existing OSG)
static uint8_t body_shapes_lookup[LOOKUP_SIZE];
uint8_t auto_btl_portrait_lookup[LOOKUP_SIZE]; // lookup table, we need O(1) access. 

uint16_t cus_aura_lookup[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
uint8_t cus_aura_bh11_lookup[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
static uint32_t cus_aura_int2_lookup[LOOKUP_SIZE];
static uint8_t cus_aura_bh10_lookup[LOOKUP_SIZE];
static uint32_t cus_aura_int3_lookup[LOOKUP_SIZE];
uint8_t force_teleport[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
static uint8_t cus_aura_bh13_lookup[LOOKUP_SIZE];
uint8_t cus_aura_bh66_lookup[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
static uint32_t bcs_hair_colors[LOOKUP_SIZE];
static uint32_t bcs_eyes_colors[LOOKUP_SIZE];
static uint8_t remove_hair_accessories_lookup[LOOKUP_SIZE];
uint8_t any_dual_skill_lookup[LOOKUP_SIZE]; // This one must be accessible by chara_patch_asm.S

// Aliases
static std::unordered_map<std::string, std::string> ttc_files_map;

// Pseudo-cacs
std::unordered_map<uint32_t, BcsColorsMap> pcac_colors;

extern void GetRealAura();
extern void GetCABehaviour11();
extern void ForceTeleport();

PUBLIC void PreBakeSetup(size_t)
{
	// Set up lookup tables first
	memset(body_shapes_lookup, 0xFF, sizeof(body_shapes_lookup));	
	memset(auto_btl_portrait_lookup, 0, sizeof(auto_btl_portrait_lookup));	
	memset(cus_aura_lookup, 0xFF, sizeof(cus_aura_lookup));
	memset(cus_aura_bh11_lookup, 0xFF, sizeof(cus_aura_bh11_lookup));
	memset(cus_aura_int2_lookup, 0, sizeof(cus_aura_int2_lookup));
	memset(cus_aura_bh10_lookup, 0xFF, sizeof(cus_aura_bh10_lookup));
	memset(cus_aura_int3_lookup, 0, sizeof(cus_aura_int3_lookup));
	memset(force_teleport, 0, sizeof(force_teleport));
	memset(cus_aura_bh13_lookup, 0xFF, sizeof(cus_aura_bh13_lookup));
	memset(cus_aura_bh66_lookup, 0xFF, sizeof(cus_aura_bh66_lookup));
	memset(bcs_hair_colors, 0xFF, sizeof(bcs_hair_colors));
	memset(bcs_eyes_colors, 0xFF, sizeof(bcs_eyes_colors));
	memset(remove_hair_accessories_lookup, 0xFF, sizeof(remove_hair_accessories_lookup));
	memset(any_dual_skill_lookup, 0, sizeof(any_dual_skill_lookup));
		
	// Original aura mapping
	cus_aura_lookup[0] = 5;
	cus_aura_lookup[1] = 6;
	cus_aura_lookup[2] = 7;
	cus_aura_lookup[3] = cus_aura_lookup[10] = 0xF;
	cus_aura_lookup[4] = cus_aura_lookup[5] = cus_aura_lookup[6] = 0xD;
	cus_aura_lookup[7] = 0xE;
	cus_aura_lookup[8] = 0x12;
	cus_aura_lookup[9] = 0x13;
	cus_aura_lookup[11] = 0x18;
	cus_aura_lookup[12] = 0x19;
	cus_aura_lookup[13] = 0x14;
	cus_aura_lookup[15] = cus_aura_lookup[17] = cus_aura_lookup[18] = 0x16;
	cus_aura_lookup[19] = 0x1E;
	cus_aura_lookup[16] = 0x17;
	cus_aura_lookup[20] = cus_aura_lookup[21] = 0x21;
	cus_aura_lookup[24] = 0x15;
	cus_aura_lookup[25] = 0x27;
	// Original behaviour_11 values: nothing (the 0xFF init will ensure that)
	// Original int 2 values (only non zero)
	cus_aura_int2_lookup[1] = cus_aura_int2_lookup[5] = cus_aura_int2_lookup[21] = cus_aura_int2_lookup[23] = 1; 
	cus_aura_int2_lookup[2] = cus_aura_int2_lookup[3] = cus_aura_int2_lookup[6] = 2;
	// Original behaviour_10 values: nothing (the 0xFF init will ensure that)
	// Original int 3 values (only non zero)
	cus_aura_int3_lookup[0] = 1;
	for (int i = 11; i <= 19; i++)
		cus_aura_int3_lookup[i] = 1;
	
	cus_aura_int3_lookup[1] = cus_aura_int3_lookup[7] = cus_aura_int3_lookup[8] = cus_aura_int3_lookup[9] = 2;
	cus_aura_int3_lookup[2] = cus_aura_int3_lookup[3] = 3;
	// Default force_teleport are all 0
	// Original behaviour_13 values: nothing (the 0xFF init will ensure that)
	// Original cus_aura_bh66_lookup values: nothing (the 0xFF init will ensure that)
	// Original bcs hair colors: only ssj blue and evolution, the 0xFF memset ensure rest get nothing.
	bcs_hair_colors[24] = 60; // SSJ Blue original
	bcs_hair_colors[25] = 67; // SSJ Blue Evolution original
	// Original bcs eyes colors: only ssj blue and evolution, the 0xFF memset ensure rest get nothing.
	bcs_eyes_colors[24] = 60; // SSJ Blue original
	bcs_eyes_colors[25] = 67; // SSJ Blue Evolution original
	// Original remove_hair_accessories_lookup: nothing (the 0xFF init will ensure that)
	
	Xv2PreBakedFile pbk;
	const std::string pbk_path = myself_path + CONTENT_ROOT + "data/pre-baked.xml";
	
	if (!pbk.CompileFromFile(pbk_path, false))
	{
		if (Utils::FileExists(pbk_path))
		{
			UPRINTF("Compilationof \"%s\" failed.\n", pbk_path.c_str());
			exit(-1);
		}
		
		return;
	}
	
	std::vector<std::string> &additional_ozarus = pbk.GetOzarus();	
	for (const std::string &ozaru : additional_ozarus)
		ozarus.push_back(ozaru);
	
	const std::vector<BodyShape> &body_shapes = pbk.GetBodyShapes();	
	for (const BodyShape &shape : body_shapes)
	{
		if (shape.cms_entry < LOOKUP_SIZE)
			body_shapes_lookup[shape.cms_entry] = (uint8_t)shape.body_shape;
	}
	
	
	const std::vector<uint32_t> &auto_btl_portraits_list = pbk.GetAutoBtlPortraitList();		
	for (uint32_t cms_entry : auto_btl_portraits_list)
	{
		if (cms_entry < LOOKUP_SIZE)
			auto_btl_portrait_lookup[cms_entry] = 1;
	}
	
	const std::vector<CusAuraData> &cus_aura_datas = pbk.GetCusAuraDatas();
	for (const CusAuraData &data : cus_aura_datas)
	{
		if (data.cus_aura_id < LOOKUP_SIZE)
		{
			cus_aura_lookup[data.cus_aura_id] = data.aur_aura_id;
			cus_aura_bh11_lookup[data.cus_aura_id] = data.behaviour_11;
			cus_aura_int2_lookup[data.cus_aura_id] = data.integer_2;
			cus_aura_bh10_lookup[data.cus_aura_id] = data.behaviour_10;
			cus_aura_int3_lookup[data.cus_aura_id] = data.integer_3;
			force_teleport[data.cus_aura_id] = data.force_teleport;
			cus_aura_bh13_lookup[data.cus_aura_id] = data.behaviour_13;
			cus_aura_bh66_lookup[data.cus_aura_id] = data.behaviour_66;
			remove_hair_accessories_lookup[data.cus_aura_id] = data.remove_hair_accessories;
			bcs_hair_colors[data.cus_aura_id] = data.bcs_hair_color;
			bcs_eyes_colors[data.cus_aura_id] = data.bcs_eyes_color;			
		}
	}
	
	const std::vector<PreBakedAlias> &aliases = pbk.GetAliases();	
	for (const PreBakedAlias &alias : aliases)
	{
		ttc_files_map[alias.cms_name] = alias.ttc_files;
	}
	
	const std::vector<uint32_t> &any_dual_skills = pbk.GetAnyDualSkillList();
	for (uint32_t cms_entry : any_dual_skills)
	{
		if (cms_entry < LOOKUP_SIZE)
			any_dual_skill_lookup[cms_entry] = 1;
	}
	
	pcac_colors = pbk.GetColorsMap();
}

// Old implementation that simplye skipped call, (not longer works in 1.10)
/*PUBLIC void ApplyCacMatsPatched(uint64_t *object, uint32_t arg2, const char *mat)
{
	uint32_t *object2 = (uint32_t *)object[0x30/8];
	// Don' the call in mods chars.
	bool do_call = true;
	
	//DPRINTF("Apply Cacs Mats. Cms 0x%x. arg2=0x%x, arg3=%s, called from %p\n", object2[0x90/4], arg2, mat, BRA(0));
	
	if (object2)
	{
		uint32_t cms_entry = object2[0x90/4];	
		
		if (cms_entry >= 0x9C)
		{
			do_call = false;
		}
	}
	
	if (do_call)
		ApplyCacMats(object, arg2, mat);
}*/

static const std::unordered_map<std::string, uint16_t> bcs_part_to_index =
{
	{ "SKIN_", 0 },
	{ "SKIN_A_", 0}, // Same as above
	{ "SKIN_B_", 1 },
	{ "SKIN_C_", 2 },
	{ "SKIN_D_", 3 },
	{ "HAIR_", 4 },
	{ "eye_", 5 },
	{ "CC00_BUST_", 6 },
	{ "CC01_BUST_", 7 },
	{ "CC02_BUST_", 8 },
	{ "CC03_BUST_", 9 },
	{ "CC00_PANTS_", 10 },
	{ "CC01_PANTS_", 11 },
	{ "CC02_PANTS_", 12 },
	{ "CC03_PANTS_", 13 },
	{ "CC00_RIST_", 14 },
	{ "CC01_RIST_", 15 },
	{ "CC02_RIST_", 16 },
	{ "CC03_RIST_", 17 },
	{ "CC00_BOOTS_", 18 },
	{ "CC01_BOOTS_", 19 },
	{ "CC02_BOOTS_", 20 },
	{ "CC03_BOOTS_", 21 },
	{ "PAINT_A_", 22 },
	{ "PAINT_B_", 23 },
	{ "PAINT_C_", 24 }
};

PUBLIC void ApplyCacMatsPatched(uint64_t *object, uint32_t arg2, const char *mat)
{
	uint32_t *object2 = (uint32_t *)object[0x30/8];
	// Don' the call in mods chars.
	bool do_call = true;
	
	//DPRINTF("Apply Cacs Mats. Object = %p, object2 = %p, Cms 0x%x. arg2=0x%x, arg3=%s, called from %p\n", object, object2, object2[0x90/4], arg2, mat, BRA(0));
	
	if (object2)
	{
		uint32_t cms_entry = object2[0x90/4];	
		uint32_t costume = object2[0x94/4] & 0xFFFF;
		uint16_t *colors = (uint16_t *)(object2 + 0xD8/4);
		
		uint32_t key = (cms_entry << 16) | costume;
		
		auto it = pcac_colors.find(key);
		if (it != pcac_colors.end())
		{
			do_call = true;		
			memset(colors, 0xFF, 25*sizeof(uint16_t));
			
			const BcsColorsMap &map = it->second;			
			
			for (auto &it2 : map.map)
			{
				auto it3 = bcs_part_to_index.find(it2.first);
				if (it3 != bcs_part_to_index.end())
				{
					colors[it3->second] = it2.second;
				}
			}
		}
		
		else if (cms_entry >= 0x9C) // Don't do the call for mods, otherwise some mods like Towifla break
		{
			do_call = false;
		}
	}
	
	if (do_call)
		ApplyCacMats(object, arg2, mat);
}

PUBLIC void ApplyCacMats_Setup(ApplyCacMatsType orig)
{
	ApplyCacMats = orig;
}

PUBLIC int IsOzaruReplacement(const char *char_code)
{
	for (const std::string &ozaru : ozarus)
	{
		if (strncmp(ozaru.c_str(), char_code, 3) == 0)
			return 1;
	}
		
	return 0;	
}

PUBLIC void SetBodyShape_Setup(SetBodyShapeType orig)
{
	SetBodyShape = orig;
}

PUBLIC uint64_t SetBodyShape_Patched(void *object, int32_t body_shape, int32_t arg3, float arg4)
{
	// Note, if object offsets of this function change, change the ones in ApplyCacMatsPatched too 
	
	if (body_shape < 0 && object)
	{
		uint64_t *object64 = (uint64_t *)object;
		uint32_t *object2 = (uint32_t *)object64[0x30/8];
		
		if (object2)
		{
			uint32_t cms_entry = object2[0x90/4];		

			//DPRINTF("*********** CMS ENTRY = 0x%x\n", cms_entry);
			
			if (cms_entry < LOOKUP_SIZE && body_shapes_lookup[cms_entry] != 0xFF)
				body_shape = body_shapes_lookup[cms_entry];
		}
	}
	
	return SetBodyShape(object, body_shape, arg3, arg4);
}

PUBLIC int UseAutobattlePortrait(int32_t cms_entry)
{
	if (cms_entry >= 0 && cms_entry < LOOKUP_SIZE && auto_btl_portrait_lookup[cms_entry])
		return 1;
	
	// Otherwise, do the same as game implementation	
	if (cms_entry >= 0x64 && cms_entry <= 0x77)
		return 1;
	
	return 0;
}

PUBLIC void ResultPortraits2Setup(ResultPortraits2Type orig)
{
	ResultPortraits2 = orig;
}

PUBLIC uint64_t ResultPortraits2Patched(Battle_HudCockpit *pthis, int idx, void *r8, void *r9, void *arg5, void *arg6, void *arg7, void *arg8)
{
	uint32_t *undo = nullptr;
	
	if (idx >= 0 && idx < 0xE)
	{
		Battle_HudCharInfo *info = &pthis->char_infos[idx];
		uint32_t cms_entry = info->cms_entry;
		
		//DPRINTF("-----------Cms entry is 0x%x 0x%x\n", cms_entry, info->cms_entry2);
				
		if (cms_entry < LOOKUP_SIZE && auto_btl_portrait_lookup[cms_entry])
		{
			undo = &info->is_cac;
			
			if (*undo == 0)
				*undo = 1;
			else
				undo = nullptr;
		}
	}
	
	uint64_t ret = ResultPortraits2(pthis, idx, r8, r9, arg5, arg6, arg7, arg8);
	
	if (undo)
		*undo = 0;
	
	return ret;
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC void CusAuraMapPatch(uint8_t *buf)
{
	// movsxd  rcx, eax
	PatchUtils::Write8(buf, 0x48); PatchUtils::Write8(buf+1, 0x63); PatchUtils::Write8(buf+2, 0xC8);	
	PatchUtils::Write16((uint16_t *)(buf+3), 0xBA48); // mov rdx, XXXXXXXXXXXXXX
	
	uint64_t gra_addr = (uint64_t)GetRealAura;
	PatchUtils::Write64((uint64_t *)(buf+5), gra_addr);
	PatchUtils::Write16((uint16_t *)(buf+13), 0xE2FF); // jmp rdx
	
	uint64_t *ret_addr = (uint64_t *)(gra_addr+0x1E);
	if (*ret_addr != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in CusAuraMapPatch\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr, (uint64_t)(buf+0x94)); // buf+0x94 -> the address of end of switch	
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC void CusAuraPatchBH11(uint8_t *buf)
{
	PatchUtils::Write16((uint16_t *)buf, 0xB948); // mov rcx, XXXXXXXXXXXXXX
	
	uint64_t g11_addr = (uint64_t)GetCABehaviour11;
	PatchUtils::Write64((uint64_t *)(buf+2), g11_addr);
	PatchUtils::Write16((uint16_t *)(buf+10), 0xE1FF); // jmp rcx
	
	uint64_t *ret_addr = (uint64_t *)(g11_addr+0x62);
	if (*ret_addr != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in CusAuraPatchBH11\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr, (uint64_t)(buf+0x43)); // buf+0x43 -> address to return to
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC uint32_t CusAuraPatchInt2(Battle_Mob *pthis)
{
	if (pthis->IsTransformed()) // if awaken skill? needs confirm...
	{
		uint32_t cus_aura = (uint32_t)pthis->trans_control;
		
		if (cus_aura == 0xFFFFFFFF)
			return 0xFFFFFFFF;
		
		if (cus_aura < LOOKUP_SIZE)
			return cus_aura_int2_lookup[cus_aura];
		
		return 0;
	}
	
	return 0xFFFFFFFF;
}

PUBLIC void Behaviour10Setup(Behaviour10FuncType orig)
{
	Behaviour10Func = orig;
}

PUBLIC uint32_t Behaviour10FuncPatched(void *obj, uint32_t cus_aura, void *unk)
{
	if (cus_aura < LOOKUP_SIZE && cus_aura_bh10_lookup[cus_aura] <= BEHAVIOUR_MAX)
		cus_aura = cus_aura_bh10_lookup[cus_aura];
	
	return Behaviour10Func(obj, cus_aura, unk);
}

PUBLIC uint32_t CusAuraPatchInt3(Battle_Mob *pthis)
{
	if (pthis->IsTransformed()) 
	{
		uint32_t cus_aura = (uint32_t) pthis->trans_control;
		
		if (cus_aura < LOOKUP_SIZE)
			return cus_aura_int3_lookup[cus_aura];
	}
	
	return 0;
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC void CusAuraPatchTeleport(uint8_t *buf)
{
	PatchUtils::Write16((uint16_t *)buf, 0xB948); // mov rcx, XXXXXXXXXXXXXX
	
	uint64_t ft_addr = (uint64_t)ForceTeleport;
	PatchUtils::Write64((uint64_t *)(buf+2), ft_addr);
	PatchUtils::Write16((uint16_t *)(buf+10), 0xE1FF); // jmp rcx
	
	uint64_t *ret_addr1 = (uint64_t *)(ft_addr+0x42);
	if (*ret_addr1 != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in CusAuraPatchTeleport\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr1, (uint64_t)(buf+0xC)); // buf+0xC -> address return for teleport
	
	uint64_t *ret_addr2 = (uint64_t *)(ft_addr+0x4E);
	if (*ret_addr2 != 0xFEDCBA987654321)
	{
		UPRINTF("Internal error in CusAuraPatchTeleport (2)\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr2, (uint64_t)(buf+0x599)); // buf+0x599 -> address return for no teleport   
}

PUBLIC uint32_t Behaviour13(Battle_Mob *pthis)
{
	uint32_t cus_aura = (uint32_t) pthis->trans_control;
	
	if (cus_aura < LOOKUP_SIZE && cus_aura_bh13_lookup[cus_aura] <= BEHAVIOUR_MAX)
		cus_aura = cus_aura_bh13_lookup[cus_aura];

	return cus_aura;
}

typedef void (* SetBcsColorType)(void *, int, uint64_t, const char *);

void SetBcsHairColorPatched(void *pthis, Battle_Mob *mob, uint64_t unk, const char *part)
{
	int hair_color = -1;
	int eyes_color = -1;
	
	if (mob && mob->trans_control >= 0 && mob->trans_control < LOOKUP_SIZE)
	{
		hair_color = (int)bcs_hair_colors[mob->trans_control];
		eyes_color = (int)bcs_eyes_colors[mob->trans_control];
	}
	
	uint64_t *vtable = (uint64_t *) *(uint64_t *)pthis;
	SetBcsColorType SetBcsColor = (SetBcsColorType) vtable[0x3E8/8];
	
	if (hair_color >= 0)
	{
		SetBcsColor(pthis, hair_color, unk, part);		
	}
	
	if (eyes_color >= 0)
	{
		SetBcsColor(pthis, eyes_color, unk, "eye_");
	}
}

PUBLIC void ApplyBcsHairColorPatch(uint8_t *addr)
{
	// First patch, destroy the conditional
	PatchUtils::Write16(addr+7, 0x9090);
	
	// Second patch, replace edx=60 by rdx=rdi=Battle_Mob ptr 
	PatchUtils::Write32(addr+0x1D, 0x90FA8948); // mov rdx, rdi; nop
		
	// Third patch, hook the method 0x3E8	
	PatchUtils::Write16(addr+0x21, 0xE890);
	PatchUtils::HookCall(addr+0x21+1, nullptr, (void *)SetBcsHairColorPatched);
	
	// Fourth patch, skip eyes call
	PatchUtils::Nop(addr+0x3F, 6);
	
	// Fifth patch, skip SSJ blue evolution code 
	PatchUtils::Write8(addr+0x4C, 0xEB); // jne to jmp	
}

PUBLIC void ApplyBcsHairColorPatch2(uint8_t *addr)
{
	// First patch, destroy the conditional
	PatchUtils::Write16(addr+7, 0x9090);
	
	// Second patch, replace edx=60 by rdx=rbx=Battle_Mob ptr 
	PatchUtils::Write32(addr+0x1A, 0x90DA8948); // mov rdx, rbx; nop
	PatchUtils::Write8(addr+0x1A+4, 0x90);
		
	// Third patch, hook the method 0x3E8	
	PatchUtils::Write16(addr+0x23, 0xE890);
	PatchUtils::HookCall(addr+0x23+1, nullptr, (void *)SetBcsHairColorPatched);
	
	// Fourth patch, skip eyes call
	PatchUtils::Nop(addr+0x43, 6);
	
	// Fifth patch, skip SSJ blue evolution code 
	PatchUtils::Write8(addr+0x50, 0xEB); // jne to jmp	
}

typedef void (* RemoveAccessoriesType)(void *, int);

void RemoveAccessoriesPatched(void *pthis, Battle_Mob *mob)
{
	bool do_call = false;
	
	if (mob && mob->trans_control >= 0 && mob->trans_control < LOOKUP_SIZE)
	{
		uint8_t remove = remove_hair_accessories_lookup[mob->trans_control];
		
		if (remove == 0)
		{
			// Nothing, do_call is already false
		}
		else if (remove == 1)
		{
			do_call = true;
		}
		else
		{
			// Default game behaviour, restoring here the destroyed conditionals			
			if (mob->trans_control == 2)
			{
				if (mob->IsCac())
				{
					do_call = true;
				}
			}
		}
	}
	
	//DPRINTF("Remove hair accesory: 0x%x %d\n", mob->trans_control, do_call);
	
	if (do_call)
	{
		uint64_t *vtable = (uint64_t *) *(uint64_t *)pthis;
		RemoveAccessoriesType RemoveAccessories = (RemoveAccessoriesType) vtable[0x380/8];
		
		RemoveAccessories(pthis, 5); // 5 = hair part
	}
}

PUBLIC void ApplyConditionalRemoveHairAccessories(uint8_t *addr)
{
	// First patch, destroy first conditional
	PatchUtils::Write16(addr+7, 0x9090);
	
	// Second patch, destroy the second conditional
	PatchUtils::Write16(addr+16, 0x9090);
	
	// Third patch, replace edx=5 by rdx=rbx=Battle_Mob ptr
	PatchUtils::Write32(addr+28, 0x90DA8948); // mov rdx, rbx; nop
	PatchUtils::Write8(addr+28+4, 0x90);
	
	// Fourth patch, hook the method +0x370
	PatchUtils::Write16(addr+33, 0xE890);
	PatchUtils::HookCall(addr+33+1, nullptr, (void *)RemoveAccessoriesPatched);
}

typedef void (* ChangePartsetType)(Battle_Mob *, int, int, int);
static ChangePartsetType ChangePartset;

bool test_144 = false;

PUBLIC void SetupChangePartset(ChangePartsetType orig)
{
	test_144 = true;
	ChangePartset = orig;
}

PUBLIC void ChangePartsetPatched(Battle_Mob *pthis, int partset, int unk1, int trans_control)
{
	//DPRINTF("Change partset: cms 0x%x partset=%d unk1=%d trans_control=0x%x default=%d\n", pthis->cms_id, partset, unk1, trans_control, pthis->default_partset);
	if (partset == 272 && unk1 == 1 && (trans_control == 0x38 || trans_control == 0x3F))
	{
		pthis->default_partset = 273;
	}
	else if (partset == 280 && unk1 == 1 && trans_control == 0x3B)
	{
		pthis->default_partset = 281;
	}
	
	ChangePartset(pthis, partset, unk1, trans_control);
}

typedef void (* UntransformType)(Battle_Mob *, int);
static UntransformType Untransform;

static bool untransform_exclude_lookup[LOOKUP_SIZE];
static bool apply_to_roster;

PUBLIC void SetupUntransform(UntransformType orig)
{
	Untransform = orig;
	
	std::vector<int> exclude_trans;
	ini.GetMultipleIntegersValues("UNTRANSFORM_CUT_SCENES", "exclude_trans", exclude_trans);
	
	memset(untransform_exclude_lookup, 0, sizeof(untransform_exclude_lookup));
	
	for (int trans : exclude_trans)
	{
		if (trans >= 0 && trans < LOOKUP_SIZE)
			untransform_exclude_lookup[trans] = true;
	}
	
	ini.GetBooleanValue("UNTRANSFORM_CUT_SCENES", "apply_to_roster", &apply_to_roster, false);
}

PUBLIC void PatchUntransform(Battle_Mob *pthis, int unk1)
{
	//DPRINTF("Untransform cut scene: 0x%x\n", pthis->cms_id);
	if (apply_to_roster || (pthis->cms_id >= 0x64 && pthis->cms_id <= 0x77))
	{
		bool exclude = false;
		
		if (pthis->trans_control >= 0 && pthis->trans_control < LOOKUP_SIZE && untransform_exclude_lookup[pthis->trans_control])
			exclude = true;
		
		if (!exclude)
			return;
	}	
	
	Untransform(pthis, unk1);
}

typedef StdString * (* StringAppendType)(StdString *, const char *, size_t);
static StringAppendType StringAppend;

PUBLIC void SetupStringAppend(StringAppendType orig)
{
	StringAppend = orig;
}

PUBLIC StdString *TtcFilesAlias(StdString *path, const char *cms, size_t append_len)
{
	if (cms)
	{
		auto it = ttc_files_map.find(cms);
		
		if (it != ttc_files_map.end())
		{
			cms = it->second.c_str();
		}
	}
	
	return StringAppend(path, cms, append_len);
}

extern void CanUseAnyDualSkill();

PUBLIC void AnyDualSkillPatch(uint8_t *buf)
{
	PatchUtils::Write16((uint16_t *)buf, 0xB848); // mov rax, XXXXXXXXXXXXXX
	
	uintptr_t cds_addr = (uintptr_t) CanUseAnyDualSkill;
	PatchUtils::Write64((uint64_t *)(buf+2), cds_addr);
	PatchUtils::Write16((uint16_t *)(buf+10), 0xE0FF); // jmp rax
	PatchUtils::Write8(buf+12, 0x90); // nop  (not really necessary, code wont reach here, but to not destroy asm view in a debugger)
	
	uint64_t *ret_addr1 = (uint64_t *)(cds_addr+0x28);
	if (*ret_addr1 != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in AnyDualSkillPatch\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr1, (uint64_t)(buf+0xD)); // buf+0xD -> address return for default dual skill usage
	
	uint64_t *ret_addr2 = (uint64_t *)(cds_addr+0x34);
	if (*ret_addr2 != 0xFEDCBA987654321)
	{
		UPRINTF("Internal error in AnyDualSkillPatch (2)\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr2, (uint64_t)(buf+0xC1)); // buf+0xC1 -> address return for any dual skill
}


} // extern "C"