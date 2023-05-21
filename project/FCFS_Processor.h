#pragma once
#include"Processor.h"
#include "Process.h"
#include "QueueLinkedList.h"

class FCFS_Processor:public Processor
{
private: 
	linkedQueue* RDY_List; // ready list 
	int timeslice;

	Process* RUN_Process = nullptr;

public:
	FCFS_Processor();
	~FCFS_Processor();

	bool IsProcessor(Process* );
	Process* GetRunProcess();
	void  Set_Run(Process* p);
	void settimeslice(int t);
	void ScheduleAlgo()override;
	//void Add_to_RDY_LIST(Process* p);
	linkedQueue* Get_RDY_List();
	Process** Signal_Kill(int, int, int, linkedQueue*, int&);
	
};

