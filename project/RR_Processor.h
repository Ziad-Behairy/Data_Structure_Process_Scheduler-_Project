#pragma once
#include"Processor.h"
#include "Process.h"
#include"CircularQueue.h"

class RR_Processor :public Processor
{
private:
	CircularQueue* RDY_List;
	int time_slice;
	Process* RUN_Process = nullptr;

public:
	RR_Processor();
	~RR_Processor();


	Process* GetRunProcess();
	void  Set_Run(Process* p);


	void ScheduleAlgo()override;
	void settimeslice(int t);
	CircularQueue* getrdylist();

};

