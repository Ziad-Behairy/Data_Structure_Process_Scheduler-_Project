#pragma once
#include "LinkedList.h"
#include "QueueArray.h"
#include "QueueLinkedList.h"
#include "Processor.h"
#include "FCFS_Processor.h"
#include "Process.h"
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <sstream>
#include "QueueArray.h"
#include "SJF_Processor.h"
#include "RR_Processor.h"
#include "UI.h"


class Schedular
{
public:
	Schedular();

	void ReadFile(string file);
	void Create_Processors(int inputNumFCFS, int inputNumSJF, int inputNumRR, int inputTimeSlice_RR);


	// *************************************** Function Will Be Modified In Phase ii ***********************************
	int Get_Sum_CT_FCFS(linkedQueue* input_RDYList);
	int Get_Sum_CT_SJF(PiorityQueue* input_RDYList);
	int Get_Sum_CT_RR(CircularQueue* input_RDYList);
	void Insert_Shortest_RDY_List(Process* P_New_RDY);

	void RDY_to_RUN(int FirstCPU);
	void RUN_to_BLK();
	void BLK_to_RDY();

	void updateCPUTimeUsed();

	void Run_to_TRM(int TTTime);

	void Migration_RR_TO_SJF(Process *ProcessToMigrate); // Function in Scheduler class, called by RR processors
	void Migration_FCFS_TO_RR(Process* ProcessToMigrate);//Function in Scheduler class, called by FCFS processors
	void WorkStealing();
	void handleoverheat_FCFS(FCFS_Processor* processor,int stoptime);
	void handleoverheat_RR(RR_Processor* processor, int stoptime);
	void handleoverheat_SJF(SJF_Processor* processor, int stoptime);
	void OverHeating();
	void CoolingProcessor();
	void Interactive_Mode();

	void KillSignal();

	void Silent_Mode();


	void OutputFile(linkedQueue* trmList);
	void selectionSort(linkedQueue*);
	void InitiateFork();
	void KillChildren(Process**, int);
	int DetectFCFSProcessor(Process* ProcessReceived);

	//void test();

	~Schedular();

private:
	int UniqueChildID;
	int timeStep = 0;
	//int random_number;
	//int random_id;
	LinkedList* NEW_List;
	int RTF, MaxW, STL, forkprobability;
	int numFCFS, numSJF, numRR, timeSliceRR;
	int numOFProcesses;

	int avgWT, avgRT, avgTRT;
	int numofmigrations;
	int numofworkstealing;

	FCFS_Processor* processorsList_FCFS = nullptr;
	SJF_Processor* processorsList_SJF = nullptr;
	RR_Processor* processorsList_RR = nullptr;


	linkedQueue* BLK_List;
	linkedQueue* TRM_List;
	ArrayQueue* Kill_List;
	ArrayQueue* Kill_List_Copy;
	UI* userInterface = new UI;
};
class UI;
