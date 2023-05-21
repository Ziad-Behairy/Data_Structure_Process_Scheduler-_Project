// this is an abstract class that should has a pure virtual function 
#pragma once
#include "Process.h"
#include <random>
#include <ctime>
enum ProcessorState {
	Stop,
	run
};
class Processor
{
protected:
	const double OVERHEAT_PROBABILITY = 0.03; // 3% probability
	//LinkedList SIGKILL; // Kill Signal list that contain all killed processes

	
	ProcessorState Pstate;
	// Run List 
	int currnet_time;
	int Stoptime;
	//int max_processes;

public:

    Processor();
	
	virtual ~Processor();
	virtual void ScheduleAlgo() = 0;


	//virtual linkedQueue* Get_RDY_List() = 0;
	//virtual void  Set_Run(Process* p) = 0;
	 int getstoptime();
	 void setstoptime(int t);
	 bool processor_overheated();
	 void setprocessorstate(ProcessorState p);
	 ProcessorState getprocessorstate();
	 friend class UI;
	 
};

