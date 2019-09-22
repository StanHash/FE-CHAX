
#include "LTF.h"

// I'm just borrowing padding between sections
#define gLTFEnableFlag (*((u8*)(0x03000006)))

extern const struct ProcInstruction LTFPlayerPhaseProcHook[];

extern const u8 gLTFChapterEnableList[];

void LTFT_UpdateLTFEnable(void)
{
	const u8* it = gLTFChapterEnableList;

	while (it[0] != 0xFF)
	{
		if (it[0] == gChapterData.chapterIndex)
		{
			gLTFEnableFlag = TRUE;
			return;
		}

		it++;
	}

	gLTFEnableFlag = FALSE;
}

int LTFT_IsEnabled(void)
{
	return !!gLTFEnableFlag; //return !!CheckEventId(gLTFEnableFlag);
}

void LTFT_PlayerPhaseEndHook(struct Proc* proc)
{
	if (LTFT_IsEnabled())
		Proc_JumpToPointer(proc, LTFPlayerPhaseProcHook);
	else
		ProcGoto(proc, 0);
}

void LTFT_CpDecideLoopCheck(struct Proc* proc)
{
	if (!LTFT_IsEnabled())
		ProcGoto(proc, 0);
}
