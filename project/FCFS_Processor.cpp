#include "FCFS_Processor.h"


FCFS_Processor::FCFS_Processor()
{
	RDY_List = new linkedQueue();
	//RUN_Process = new Process;
}

FCFS_Processor::~FCFS_Processor()
{
}

void FCFS_Processor::settimeslice(int t)
{
	timeslice = t;
}

void FCFS_Processor::ScheduleAlgo()
{
	if (RDY_List->isEmpty() && RUN_Process) {
		RUN_Process = nullptr;
		return;
	};
	// IF there is processes in the queue move the first process to run list according to FCFS algorithm
	Process* p = RDY_List->front();
	RDY_List->dequeue();
	Set_Run(p);
	
}


Process* FCFS_Processor::GetRunProcess() {
	return RUN_Process;
}
void FCFS_Processor::Set_Run(Process* p)
{
	RUN_Process = p;
	p->SetState(RUN);
	//cpu time used ; 
}


//void FCFS_Processor::Add_to_RDY_LIST(Process* p)
//{
//	RDY_List->enqueue(p);
//	p.SetState(RDY);
//}

linkedQueue* FCFS_Processor::Get_RDY_List()
{
	return RDY_List;
}

bool FCFS_Processor::IsProcessor(Process* P)
{
	for (int i = 0; i < RDY_List->getLength(); i++)
	{
		if (P == RDY_List->getItem(i)) return true;
	}

	if (P == RUN_Process) return true;

	return false;
}

    Process** FCFS_Processor::Signal_Kill(int Kill_Time, int Kill_ID, int TimeStep, linkedQueue* TRM_List, int& size)
    {
        size = RDY_List->getLength();
        Process** ReturnedChildren = nullptr;
        int index = 0;
        for (int i = 0; i < RDY_List->getLength(); i++) {
            Process* ProcessToKill = RDY_List->getItem(i);

            if (ProcessToKill->getPID() == Kill_ID and TimeStep == Kill_Time)
            {
                if (ProcessToKill->getChild())
                {
                    if (ReturnedChildren == nullptr) {
                        ReturnedChildren = new Process* [size];
                    }
                    ReturnedChildren[index] = ProcessToKill->getChild();
                    index++;
                }

                ProcessToKill->SetState(TRM);
                ProcessToKill->setTT(TimeStep);
                TRM_List->enqueue(ProcessToKill);
                RDY_List->KillProcess(ProcessToKill);
            }
        }

        if (RUN_Process->getPID() == Kill_ID and TimeStep == Kill_Time)
        {
            if (RUN_Process->getChild())
            {
                if (ReturnedChildren == nullptr) {
                    ReturnedChildren = new Process * [size];
                }
                ReturnedChildren[index] = RUN_Process->getChild();
                index++;
            }

            RUN_Process->SetState(TRM);
            RUN_Process->setTT(TimeStep);
            TRM_List->enqueue(RUN_Process);

            ScheduleAlgo();

        }

        if (ReturnedChildren == nullptr) {
            size = 0;
        }
        else {
            size = index;
        }

        return ReturnedChildren;
    }






