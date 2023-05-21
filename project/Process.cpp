#include"Process.h"
#include<iostream>



Process::Process(int inputArrivalTime, int inputID, int inputCPUTime, int inputNumIO, ArrayQueue* inputList, int input_total_IOD)
{
	pID = inputID;
	arrivalTime = inputArrivalTime;
	
	CPUTime = inputCPUTime;
	
	cpuTimeUsed = 0;
	numIO = inputNumIO;

	IO_List = inputList;

	//cout << IO_List->getFront() <<endl;

	state = NEW;

	total_IOD = input_total_IOD;

}


ProcessState Process::getstate()
{
	return state;
}

void Process::SetState(ProcessState s)
{
	
	state=s ;
}

void Process::SetRemainingTime(int t)
{
	remaining_time = t;
}


int Process::getCPUtime()
{
	return CPUTime;

}


int Process::getCPUTimeUsed()
{
	return cpuTimeUsed;
}
void Process::setCPUTimeUsed() {
	cpuTimeUsed++;
}

int Process::getPID()
{
	if (this)
		return pID;
	else
		return -1;

}
int Process::getArrivalTime()
{
	return arrivalTime;

}
int Process::getWatingTime()
{
	return intervalTime- arrivalTime;
}
int Process::getRemainingTime()
{ 
  return remaining_time;
}
int Process::getNumIO() {
	return numIO;
}


void Process::SetEndTime(int end_time) {
	this->EndTime = end_time;
}



Process::~Process()
{
}
