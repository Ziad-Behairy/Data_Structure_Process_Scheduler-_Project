#include "RR_Processor.h"
#include"Schedular.h"
#include"QueueLinkedList.h"

RR_Processor::RR_Processor()
{
    RDY_List = new CircularQueue;
	
}

RR_Processor::~RR_Processor()
{
    RDY_List = new CircularQueue;
}


void RR_Processor::ScheduleAlgo()
{
    if (RDY_List->isEmpty() && RUN_Process) {
        RUN_Process = nullptr;
        return;
    };
    // Get the current process from the ready list
    Process* p = RDY_List->front();
    RDY_List->dequeue();

    // If the current process has finished executing, remove it from the system
    if (p->getRemainingTime() <= time_slice) {
        p->SetState(TRM);
        p->SetEndTime(currnet_time);
    }
    // Otherwise, add it back to the ready list with updated remaining time
    else {

        Set_Run(p);


        p->SetState(RDY);
        p->SetRemainingTime(p->getRemainingTime() - time_slice);
        RDY_List->enqueue(p);
    }
   

}

Process* RR_Processor::GetRunProcess()
{
	return RUN_Process;
}
void RR_Processor::Set_Run(Process* p)
{
    RUN_Process = p;
    p->SetState(RUN);
    //cpu time used ; 
}


void RR_Processor::settimeslice(int t)
{
    time_slice = t;
}

CircularQueue* RR_Processor::getrdylist()
{
    return RDY_List;

}

