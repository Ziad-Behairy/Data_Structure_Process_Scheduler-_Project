#include "SJF_Processor.h"

SJF_Processor::SJF_Processor()
{
	RDY_List = new PiorityQueue;
}

SJF_Processor::~SJF_Processor()
{
}

void SJF_Processor::ScheduleAlgo()
{
	if (RDY_List->isEmpty() && RUN_Process) {
		RUN_Process = nullptr;
		return;
	};
	if (!RDY_List->isEmpty()) {
		Process* p = RDY_List->Dequeue();
		Set_Run(p);
}
}

Process* SJF_Processor::GetRunProcess()
{
	return RUN_Process;
}

void SJF_Processor::Set_Run(Process* p)
{
	RUN_Process = p;
	p->SetState(RUN);
	//cpu time used ; 
}

PiorityQueue* SJF_Processor::Get_RDY_List()
{
	return RDY_List;
}


