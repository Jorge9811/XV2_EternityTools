#include "PatchUtils.h"
#include "xv2patcher.h"
#include "ui_patch.h"
#include "debug.h"

#define	PLAYER_INDEX 			0
#define CPU_WITH_RED_RING_INDEX	1
#define CPU1					7
#define CPU_EXPERT				9

static uint8_t orig_tbui_ins[10];
static uint8_t tbui_patch[10] = { 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x1F, 0x44, 0x00, 0x00 }; // mov edi, -1; 5-byte nop
static void *tbui_patch_address;
int should_extend_pause_menu;


extern "C"
{
	
PUBLIC void OnLocatePortraitEnemyForEmbSwitchTable(void *addr)
{
	uint32_t *switch_table = (uint32_t *) ((uintptr_t)(*(uint32_t *)addr) + (uintptr_t)GetModuleHandle(nullptr));	
	
	// Case 7&8 to match cases 9-15
	PatchUtils::Write32(&switch_table[7], switch_table[9]);
	PatchUtils::Write32(&switch_table[8], switch_table[9]);
}

PUBLIC void ShowTheRedRing(void *addr)
{
	uint32_t **btl_hud_array = (uint32_t **) ((uintptr_t)(*(uint32_t *)addr) + (uintptr_t)GetModuleHandle(nullptr));
	
	PatchUtils::Write32(&btl_hud_array[CPU1][0], btl_hud_array[CPU_WITH_RED_RING_INDEX][0]);
}

bool IsBattleUIHidden()
{
	if (!tbui_patch_address)
		return false;
	
	return (memcmp(tbui_patch_address, tbui_patch, sizeof(tbui_patch)) == 0);
}

void ToggleBattleUI()
{
	if (!tbui_patch_address)
		return;
	
	if (IsBattleUIHidden())
	{
		PatchUtils::Copy(tbui_patch_address, orig_tbui_ins, sizeof(orig_tbui_ins));
	}
	else
	{
		PatchUtils::Copy(tbui_patch_address, tbui_patch, sizeof(tbui_patch));
	}
}

bool IsToggleUIEnabled()
{
	return (tbui_patch_address != nullptr);
}

PUBLIC void OnLocateToggleBattleUIPatch(void *addr)
{
	tbui_patch_address = addr;
	memcpy(orig_tbui_ins, addr, sizeof(orig_tbui_ins));
	
	/*bool hide_battle_ui;
	ini.GetBooleanValue("Patches", "hide_battle_ui", &hide_battle_ui, false);
	
	if (hide_battle_ui)
		ToggleBattleUI();*/
}

typedef void *(* PopupPauseCtorType)(void *);
static PopupPauseCtorType PopupPauseCtor;
static void *battle_ra;

PUBLIC void SetupPopupPauseCtor(PopupPauseCtorType orig)
{
	PopupPauseCtor = orig;
}

PUBLIC void *PopupPauseCtorHooked(void *pthis)
{
	should_extend_pause_menu = (BRA(0) == battle_ra);
	return PopupPauseCtor(pthis);
}

PUBLIC void OnPopupPauseBattleRALocated(void *addr)
{
	battle_ra = addr;
}

} // extern "C"
