#pragma once
#include"Processor.h"
#include "Process.h"
#include"PiorityQueue.h"
class SJF_Processor:public Processor
{
private:
	PiorityQueue* RDY_List;

	Process* RUN_Process = nullptr;

public:
	SJF_Processor();
	~SJF_Processor();


	Process* GetRunProcess();
	void  Set_Run(Process* p);



	void ScheduleAlgo() override;
	PiorityQueue* Get_RDY_List();
};

