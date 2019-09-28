
#include "gbafe.h"

struct DecideMonitorProc
{
	PROC_HEADER;

	unsigned startTime;
};

static
void DecideMonitorDisplay(struct DecideMonitorProc* proc);

static const
struct ProcInstruction sProc_DecideMonitor[] =
{
	PROC_SET_NAME("Decide Monitor"),

	PROC_SET_DESTRUCTOR(DecideMonitorDisplay),
	PROC_BLOCK,

	PROC_END,
};

static
void DecideMonitorDisplay(struct DecideMonitorProc* proc)
{
	static const
	unsigned sPopupTime = 45;

	static const
	u32 pop[] =
	{
		0x07, (u32)("CpDecide took "),
		0x0B, 0, // number
		0x07, (u32)(" frames."),
		0x00, 0, // end
	};

	static const
	struct ProcInstruction sDummy[] =
	{
		PROC_CALL_ROUTINE(LockGameLogic),
		PROC_YIELD,
		PROC_CALL_ROUTINE(UnlockGameLogic),
		PROC_END,
	};

	SetPopupNumber(GetGameClock() - proc->startTime - sPopupTime);
	Popup_Create(pop, sPopupTime, 0, ProcStart(sDummy, ROOT_PROC_3));
}

int StartDecideMonitor(struct Proc* proc)
{
	EndEachProc(sProc_DecideMonitor);

	struct DecideMonitorProc* monitor = (void*) ProcStart(sProc_DecideMonitor, proc);
	monitor->startTime = GetGameClock();

	return FALSE;
}
