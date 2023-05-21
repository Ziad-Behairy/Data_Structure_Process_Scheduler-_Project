#pragma once
#include<string>

#include"FCFS_Processor.h"
#include"SJF_Processor.h"
#include"RR_Processor.h"
#include"QueueLinkedList.h"

using namespace std;
class RR_Processor;
class UI
{

public:
	//friend Process;
	string getfilepath();

	void printProcessInfo(FCFS_Processor* p1, int numofFCFS, SJF_Processor* p2,
		int numoSJF, RR_Processor* p3, int numoRR, linkedQueue* BLK_List,
		linkedQueue* TRM_List, int timestep);

	bool Next_Step();

};

