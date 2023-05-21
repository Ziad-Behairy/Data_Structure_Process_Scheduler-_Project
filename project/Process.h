#pragma once
#include<iostream>
//#include <list>
//#include <utility>
#include "QueueArray.h"

using namespace std;
enum ProcessState {
	NEW,
	RDY,
	RUN,
	BLK,
	TRM
};


class Process
{
private:
	int pID;
	int arrivalTime, CPUTime, numIO;
	int cpuTimeUsed; // to know the duartion of each process 
	int EndTime;

	int intervalTime;
	int remaining_time = (CPUTime - cpuTimeUsed); 
	

	int terminationTime;
	int firstTimeCPU;

	int responseTime; // difference between arrivalTime, firstTimeCPU
	int turnroundTime; // difference between arrivalTime, terminationTime

	int waitingTime; // difference between turnroundTime, CPUTime

	int total_IOD;

	
	ProcessState state; // the current state of process 

	ArrayQueue* IO_List = nullptr;
	Process* Child = nullptr;

public:
	Process();
	Process(int inputArrivalTime, int inputID, int inputCPUTime, int inputNumIO, ArrayQueue* inputList, int input_total_IOD);
	// ------------------------- get the cpu time for the process ----------------------------
	int getCPUtime();
	
	int getCPUTimeUsed();
	void setCPUTimeUsed();

	void setChild(Process* pChild) { Child = pChild; }
	Process* getChild() { return Child; }
	int getPID();
	int getArrivalTime();
	int getWatingTime();
	void setIntervalTime(int inputIntervalTime) {
		intervalTime = inputIntervalTime;
	}
	int getIntervalTime() {
		return intervalTime;
	}

	int getRemainingTime();
	

	void setNumIO() {
		numIO--;
	}
	int getNumIO();
	friend class UI;

	ArrayQueue* get_IO_List() {
		return IO_List;
	}




	bool operator==(const Process& other) const {
		return pID == other.pID;
	}
	



	// OPERATOR OVERLOAD TO COMPARE BETWEEN CT FOR PROCESSORS IN SJF TYPE 
	 bool operator()(const Process& p1, const Process& p2) { 
		return p1.CPUTime > p2.CPUTime; // compare burst time
	}
	 ProcessState getstate();

	void SetState(ProcessState s);
	void SetEndTime(int);
	void SetRemainingTime(int);




	int getTT() {
		return terminationTime;
	}
	void setTT(int TT) {
		terminationTime = TT;
	}
	int getFirstTimeCPU() {
		return firstTimeCPU;
	}
	void setFirstTimeCPU(int inputTime) 
	{
				firstTimeCPU = inputTime;
	}

	int getRT() {
		return responseTime;
	}
	void setRT() {
		responseTime = firstTimeCPU - arrivalTime;
	}

	int getTRT() {
		return turnroundTime;
	}
	void setTRT() {
		turnroundTime = terminationTime - arrivalTime;
	}

	int getWT() {
		return waitingTime;
	}
	void setWT() {
		waitingTime = turnroundTime - arrivalTime;
	}

	int getTotal_IOD() {
		return total_IOD;
	}



	~Process();


	//friend ostream& operator<<(ostream& os,  Process& process);


};

//ostream& operator<<(ostream& os,  Process& process) {
//	os << "Process " << process.getPID();
//	return os;
//}





