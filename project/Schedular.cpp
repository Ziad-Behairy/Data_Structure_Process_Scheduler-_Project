#include "Schedular.h"

#include "Process.h"

#include <fstream>
#include <iostream>

#include <string>
//#include <list>
//#include <utility>
#include <sstream>

#include "QueueArray.h"

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>



using namespace std;

Schedular::Schedular()
{
    NEW_List = new LinkedList();
    BLK_List = new linkedQueue();
    TRM_List = new linkedQueue();
    numofmigrations = 0;
    numofworkstealing = 0;
}


void Schedular::ReadFile(string file) {
    ifstream infile(file);

    if (!infile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        exit(1);
    }

    // Read Processors, General Info  --> The first three lines 
    int num_FCFS, num_SJF, num_RR, timeSlice_RR, rtf, Maxw, stl, Forkprobability;
    infile >> num_FCFS >> num_SJF >> num_RR >> timeSlice_RR >> rtf >> Maxw >> stl >> Forkprobability;
    RTF = rtf; MaxW = Maxw; STL = stl;  forkprobability= Forkprobability;
    numFCFS = num_FCFS; numSJF = num_SJF; numRR = num_RR;
    Create_Processors(num_FCFS, num_SJF, num_RR, timeSlice_RR);

    // Read Processes Info -->
    int numProcesses;
    infile >> numProcesses;


    numOFProcesses = numProcesses;
    UniqueChildID = numOFProcesses +1;
    char l;  // to end the line and go to the next one
    infile.get(l);

    string line;

    int IO_R, IO_D;
    char comma, space;

    for (int i = 0; i < numProcesses; i++) {
        

        getline(infile, line);

        int val1, val2, val3, val4;
        // extract the first four values
        istringstream ss_first_four(line);
        if (ss_first_four.fail()) {
            continue;
        }
        else {
            ss_first_four >> val1 >> val2 >> val3 >> val4;
        }

        ArrayQueue* ioList = new ArrayQueue(2* val4);

        int totalIOD_File = 0;
        for (int g = 0; g < val4; g++) {

            // find the start and end positions of the round brackets
            size_t start_pos = line.find("(");
            size_t end_pos = line.find(")");

            // create a string stream and extract the values
            std::istringstream ss(line.substr(start_pos + 1, end_pos - start_pos - 1));

            ss >> IO_R >> comma >> IO_D >> space;
            ioList->addQueue(IO_R);
            ioList->addQueue(IO_D);
            totalIOD_File += IO_D;
        }
            
      

        
        Process* P = new Process(val1, val2, val3, val4, ioList, totalIOD_File);
        NEW_List->insertEnd(P);
    }
    Kill_List = new ArrayQueue(numProcesses*2);
    int KillTime, ProcessID;

    while (!infile.eof()) {
        infile >> KillTime >> ProcessID;
        Kill_List->addQueue(KillTime);
        Kill_List->addQueue(ProcessID);
    }

   

    infile.close();

}

void Schedular::Create_Processors(int inputNumFCFS, int inputNumSJF, int inputNumRR, int inputTimeSlice_RR)
{

    processorsList_FCFS = new FCFS_Processor[inputNumFCFS];


    processorsList_SJF = new SJF_Processor[inputNumSJF];

    processorsList_RR = new RR_Processor[inputNumRR];
    for (int i = 0; i < inputNumRR; i++) 
    {
        processorsList_RR[i].settimeslice(inputTimeSlice_RR);
    }


}

int Schedular::Get_Sum_CT_FCFS(linkedQueue* input_RDYList) 
{
    int sum = 0;
    if (input_RDYList->isEmpty())
        return 0;
    else 
    {
        for (int i = 0; i < input_RDYList->getLength(); i++)
            sum += input_RDYList->getItem(i)->getCPUtime();
    }
    return sum;
}
int Schedular::Get_Sum_CT_SJF(PiorityQueue* input_RDYList)
{
    int sum = 0;
    if (input_RDYList->isEmpty())
        return 0;
    else {
        for (int i = 0; i < input_RDYList->getsize(); i++)
            sum += input_RDYList->getItem(i)->getCPUtime();
    }
    return sum;
}
int Schedular::Get_Sum_CT_RR(CircularQueue* input_RDYList)
{
    int sum = 0;
    if (input_RDYList->isEmpty())
        return 0;
    else 
    {
        for (int i = 0; i < input_RDYList->size(); i++)
            sum += input_RDYList->getItem(i)->getCPUtime();
    }
    return sum;
}

void Schedular::Insert_Shortest_RDY_List(Process* P_New_RDY)
{

    if (P_New_RDY->getPID() > numOFProcesses)
    {
        int* sums_fcfs = new int[numFCFS];
        for (int i = 0; i < numFCFS; i++) {
            sums_fcfs[i] = Get_Sum_CT_FCFS(processorsList_FCFS[i].Get_RDY_List());
        }
        int min_FCFS = sums_fcfs[0];
        int index_FCFS = 0;
        for (int g = 0; g < numFCFS; g++) {
            if (sums_fcfs[g] < min_FCFS && processorsList_FCFS[g].getprocessorstate() != Stop) {
                min_FCFS = sums_fcfs[g];
                index_FCFS = g;
            }
        }
        processorsList_FCFS[index_FCFS].Get_RDY_List()->enqueue(P_New_RDY);
        return;
    }
    
    int* sums_fcfs = new int[numFCFS];
    for (int i = 0; i < numFCFS; i++) {
        sums_fcfs[i] = Get_Sum_CT_FCFS(processorsList_FCFS[i].Get_RDY_List());
    }
    int min_FCFS = sums_fcfs[0];
    int index_FCFS = 0;
    for (int g = 0; g < numFCFS; g++) {
        if (sums_fcfs[g] < min_FCFS && processorsList_FCFS[g].getprocessorstate() != Stop) 
        {
            min_FCFS = sums_fcfs[g];
            index_FCFS = g;
        }
    }

    int* sums_sjf = new int[numSJF];
    for (int i = 0; i < numSJF; i++) {
        sums_sjf[i] = Get_Sum_CT_SJF(processorsList_SJF[i].Get_RDY_List());
    }
    int min_SJF = sums_sjf[0];
    int index_SJF = 0;
    for (int g = 0; g < numSJF; g++)
    {

        if (sums_sjf[g] < min_SJF && processorsList_SJF[g].getprocessorstate() != Stop)
        {
            min_SJF = sums_sjf[g];
            index_SJF = g;
        }
    }

    int* sums_RR = new int[numRR];
    for (int i = 0; i < numRR; i++) {
        sums_RR[i] = Get_Sum_CT_RR(processorsList_RR[i].getrdylist());
    }
    int min_RR = sums_RR[0];
    int index_RR = 0;
    for (int g = 0; g < numRR; g++) {
        if (sums_RR[g] < min_RR && processorsList_SJF[g].getprocessorstate() != Stop) {
            min_RR = sums_RR[g];
            index_RR = g;
        }
    }



    //PiorityQueue* Temp_Rdy_List_SJF = processorsList_SJF[0].Get_RDY_List();
    //int min_SJF = Get_Sum_CT_SJF(processorsList_SJF[0].Get_RDY_List());  // here is the problem
    //int sum_SJF;
    //for (int i = 0; i < numSJF; i++) {
    //    sum_SJF = Get_Sum_CT_SJF(processorsList_SJF[i].Get_RDY_List());
    //    if (sum_SJF < min_SJF) {
    //        min_SJF = sum_SJF;
    //        Temp_Rdy_List_SJF = processorsList_SJF[i].Get_RDY_List();
    //    }
    //}


    //CircularQueue* Temp_Rdy_List_RR = processorsList_RR[0].getrdylist();
    //int min_RR = Get_Sum_CT_RR(processorsList_RR[0].getrdylist());  // here is the problem
    //int sum_RR;
    //for (int i = 0; i < numRR; i++) {
    //    sum_RR = Get_Sum_CT_RR(processorsList_RR[i].getrdylist());
    //    if (sum_RR < min_RR) {
    //        min_RR = sum_RR;
    //        Temp_Rdy_List_RR = processorsList_RR[i].getrdylist();
    //    }
    //}



    if (min_FCFS <= min_SJF && min_FCFS <= min_RR)
    {
        
       
            processorsList_FCFS[index_FCFS].Get_RDY_List()->enqueue(P_New_RDY);


    }
    else if (min_SJF <= min_FCFS && min_SJF <= min_RR)
        {
        processorsList_SJF[index_SJF].Get_RDY_List()->addPriorityQueue(P_New_RDY);

        }
    else if (min_RR < min_SJF && min_RR < min_FCFS)
    {
        processorsList_RR[index_RR].getrdylist()->enqueue(P_New_RDY);

    }
         


    P_New_RDY->SetState(RDY);
    NEW_List->remove(P_New_RDY);

}


void Schedular::RDY_to_RUN(int FirstCPU) {
    for (int i = 0; i < numFCFS; i++) {
        if (!processorsList_FCFS[i].Get_RDY_List()->isEmpty() && !processorsList_FCFS[i].GetRunProcess()) {
            processorsList_FCFS[i].ScheduleAlgo();
            processorsList_FCFS[i].GetRunProcess()->setFirstTimeCPU(FirstCPU);
        }
    }

    for (int i = 0; i < numSJF; i++) {
        if (!processorsList_SJF[i].Get_RDY_List()->isEmpty() && !processorsList_SJF[i].GetRunProcess()) {
            processorsList_SJF[i].ScheduleAlgo();
            processorsList_SJF[i].GetRunProcess()->setFirstTimeCPU(FirstCPU);
        }
    }

    for (int i = 0; i < numRR; i++) {
        if (!processorsList_RR[i].getrdylist()->isEmpty() && !processorsList_RR[i].GetRunProcess()) {
            processorsList_RR[i].ScheduleAlgo();
            processorsList_RR[i].GetRunProcess()->setFirstTimeCPU(FirstCPU);
        }
    }
}
void Schedular::RUN_to_BLK() {
    
    for (int i = 0; i < numFCFS; i++) {
        if (processorsList_FCFS[i].GetRunProcess() && processorsList_FCFS[i].GetRunProcess()->getPID() < numOFProcesses) {
            
            Process* RUN_NOW_FCFS = processorsList_FCFS[i].GetRunProcess();

            if (RUN_NOW_FCFS->get_IO_List()->getFront() == RUN_NOW_FCFS->getCPUTimeUsed() && RUN_NOW_FCFS->getNumIO() != 0) {
                RUN_NOW_FCFS->setIntervalTime(timeStep);

                RUN_NOW_FCFS->setNumIO();

                BLK_List->enqueue(RUN_NOW_FCFS);
                RUN_NOW_FCFS->SetState(BLK);

                RUN_NOW_FCFS->get_IO_List()->deleteQueue();

                processorsList_FCFS[i].ScheduleAlgo();
            }
        }
        
    }
   
    for (int i = 0; i < numSJF; i++) {
        if (processorsList_SJF[i].GetRunProcess()) {
            Process* RUN_NOW_SJF = processorsList_SJF[i].GetRunProcess();

            if (RUN_NOW_SJF->get_IO_List()->getFront() == RUN_NOW_SJF->getCPUTimeUsed() && RUN_NOW_SJF->getNumIO() != 0) {
                RUN_NOW_SJF->setIntervalTime(timeStep);

                RUN_NOW_SJF->setNumIO();

                BLK_List->enqueue(RUN_NOW_SJF);
                RUN_NOW_SJF->SetState(BLK);

                RUN_NOW_SJF->get_IO_List()->deleteQueue();

                processorsList_SJF[i].ScheduleAlgo();
            }
        }
        
    }
    for (int i = 0; i < numRR; i++) {
        if (processorsList_RR[i].GetRunProcess()) {
            Process* RUN_NOW_RR = processorsList_RR[i].GetRunProcess();

            if (RUN_NOW_RR->get_IO_List()->getFront() == RUN_NOW_RR->getCPUTimeUsed() && RUN_NOW_RR->getNumIO() != 0) {
                RUN_NOW_RR->setIntervalTime(timeStep);

                RUN_NOW_RR->setNumIO();

                BLK_List->enqueue(RUN_NOW_RR);
                RUN_NOW_RR->SetState(BLK);

                RUN_NOW_RR->get_IO_List()->deleteQueue();

                processorsList_RR[i].ScheduleAlgo();
            }
        }
       
    }
 
}
void Schedular::BLK_to_RDY() {

    for (int i = 0; i < BLK_List->getLength(); i++) {
        Process* from_BLK = BLK_List->getItem(i);

        if (from_BLK->get_IO_List()->getFront() + from_BLK->getIntervalTime() == timeStep) {

            Insert_Shortest_RDY_List(from_BLK);
            from_BLK->SetState(RDY);

            from_BLK->get_IO_List()->deleteQueue();
            BLK_List->KillProcess(from_BLK);
        }

    }
}

void Schedular::updateCPUTimeUsed() {
    for (int i = 0; i < numFCFS; i++) {
        if (processorsList_FCFS[i].GetRunProcess())
            processorsList_FCFS[i].GetRunProcess()->setCPUTimeUsed();
    }
        

    for (int i = 0; i < numSJF; i++) {
        if (processorsList_SJF[i].GetRunProcess())
            processorsList_SJF[i].GetRunProcess()->setCPUTimeUsed();
    }
        

    for (int i = 0; i < numRR; i++) {
        if (processorsList_RR[i].GetRunProcess())
            processorsList_RR[i].GetRunProcess()->setCPUTimeUsed();
    }
        
}

void Schedular::Run_to_TRM(int TTTime)
{

    for (int i = 0; i < numFCFS; i++)
    {

        Process* runProcess = processorsList_FCFS[i].GetRunProcess();
        if (runProcess != nullptr && runProcess->getRemainingTime() == 0) // The running process  ready to execution
        {
            int size;

            Process** Children = processorsList_FCFS[i].Signal_Kill(timeStep, runProcess->getChild()->getPID(), timeStep, TRM_List, size);
            if (Children) KillChildren(Children, size);

            runProcess->SetState(TRM);

            runProcess->setTT(TTTime);

            TRM_List->enqueue(runProcess);

            processorsList_FCFS[i].ScheduleAlgo();
        }
    }
    for (int i = 0; i < numSJF; i++)
    {

        Process* runProcess = processorsList_SJF[i].GetRunProcess();
        if (runProcess != nullptr && runProcess->getRemainingTime() == 0) // The running process  ready to execution
        {
            runProcess->SetState(TRM);

            runProcess->setTT(TTTime);

            TRM_List->enqueue(runProcess);
            processorsList_SJF[i].ScheduleAlgo();

        }
    }
    for (int i = 0; i < numRR; i++)
    {

        Process* runProcess = processorsList_RR[i].GetRunProcess();
        if (runProcess != nullptr && runProcess->getRemainingTime() == 0) // The running process  ready to execution
        {
            runProcess->SetState(TRM);

            runProcess->setTT(TTTime);

            TRM_List->enqueue(runProcess);
            processorsList_RR[i].ScheduleAlgo();

        }
    }
}



// migration 1 
void Schedular::Migration_RR_TO_SJF(Process* ProcessToMigrate)
{

    
    if (ProcessToMigrate!=nullptr && !(ProcessToMigrate->getPID()>numOFProcesses))
    {
        cout << "firstif" << endl;
        if (ProcessToMigrate->getRemainingTime() < RTF)
        {
            cout << "secondif" << endl;
            
            int properSJF = 0;
            for (int i = 0; i < numSJF; i++)
            {
                if (!processorsList_SJF[properSJF].Get_RDY_List()->isFull())
                {
                   
                    cout << "thirdif" << endl;
                    processorsList_SJF[properSJF].Get_RDY_List()->addPriorityQueue(ProcessToMigrate);
                    numofmigrations++; // divide it on the num of process to calculate the percantage
                    cout << "compelete migration" << endl;
                    return;

                }
                properSJF = (properSJF + 1) % numSJF;
            }

        }
    }
    else
    {
        cout << "there is no run process in first mig now" << endl;
    }
    
}
// migration 2 
void Schedular::Migration_FCFS_TO_RR(Process* ProcessToMigrate)
{
   
    if (ProcessToMigrate!=nullptr && !(ProcessToMigrate->getPID() > numOFProcesses))
    {
        cout << "firstif" << endl;
        if (ProcessToMigrate->getRemainingTime()< MaxW)
        {
            cout << "secondtif" << endl;
            // need to revise the algorith for migration 
            int properRR = 0;
            for (int i = 0; i <numRR; i++)
            {
                if (!processorsList_SJF[properRR].Get_RDY_List()->isFull())
                {
                    cout << "thirdif" << endl;
                    processorsList_RR[properRR].getrdylist()->enqueue(ProcessToMigrate);
                    numofmigrations++; // divide it on the num of process to calculate the percantage
                    return;

                }
                properRR = (properRR + 1) % numRR;
            }

        }
    }
    else 
    {
        cout << "there is no run process in second mig now" << endl;
    }
}
// --------------------------fork process and the while loop until the load be less than 40  ---------------------------//
void Schedular::WorkStealing()
{
    cout << "start work stealing" << endl;
    //---------------------------------------------------------------------//
// --------------------------shortest queue  ---------------------------//
    // FCFS 
    linkedQueue* shortestQueue_FCFS = processorsList_FCFS[0].Get_RDY_List();
    int shortestFinishTime_FCFS = Get_Sum_CT_FCFS(shortestQueue_FCFS);
    for (int i = 1; i < numFCFS; i++) {
        linkedQueue* currentQueue_FCFS = processorsList_FCFS[i].Get_RDY_List();
        int currentFinishTime = Get_Sum_CT_FCFS(currentQueue_FCFS);

        if (currentFinishTime < shortestFinishTime_FCFS) {
            shortestQueue_FCFS = currentQueue_FCFS;
            shortestFinishTime_FCFS = currentFinishTime;
        }
    }
    // FCFS RR
    CircularQueue* shortestQueue_RR = processorsList_RR[0].getrdylist();
    int shortestFinishTime_RR = Get_Sum_CT_RR(shortestQueue_RR);
    for (int i = 1; i < numRR; i++) {
        CircularQueue* currentQueue_RR = processorsList_RR[i].getrdylist();
        int currentFinishTime = Get_Sum_CT_RR(currentQueue_RR);

        if (currentFinishTime < shortestFinishTime_RR) {
            shortestQueue_RR = currentQueue_RR;
            shortestFinishTime_RR = currentFinishTime;
        }
    }
    // short job first 
    PiorityQueue* shortestQueue_SJF = processorsList_SJF[0].Get_RDY_List();
    int shortestFinishTime_SJF = Get_Sum_CT_SJF(shortestQueue_SJF);
    for (int i = 1; i < numSJF; i++)
    {
        PiorityQueue* currentQueue_SJF = processorsList_SJF[i].Get_RDY_List();
        int currentFinishTime = Get_Sum_CT_SJF(currentQueue_SJF);

        if (currentFinishTime < shortestFinishTime_SJF)
        {
            shortestQueue_SJF = currentQueue_SJF;
            shortestFinishTime_SJF = currentFinishTime;
        }
    }
    //---------------------------------------------------------------------//
    // --------------------------longest Queue ---------------------------//
    // FCFS 
    linkedQueue* longesttQueue_FCFS = processorsList_FCFS[0].Get_RDY_List();
    int longesttFinishTime_FCFS = Get_Sum_CT_FCFS(longesttQueue_FCFS);
    for (int i = 1; i < numFCFS; i++) {
        linkedQueue* currentQueue_FCFS = processorsList_FCFS[i].Get_RDY_List();
        int currentFinishTime = Get_Sum_CT_FCFS(currentQueue_FCFS);

        if (currentFinishTime > longesttFinishTime_FCFS) {
            longesttQueue_FCFS = currentQueue_FCFS;
            longesttFinishTime_FCFS = currentFinishTime;
        }
    }
    // FCFS RR
    CircularQueue* longesttQueue_RR = processorsList_RR[0].getrdylist();
    int longestFinishTime_RR = Get_Sum_CT_RR(longesttQueue_RR);
    for (int i = 1; i < numRR; i++) {
        CircularQueue* currentQueue_RR = processorsList_RR[i].getrdylist();
        int currentFinishTime = Get_Sum_CT_RR(currentQueue_RR);

        if (currentFinishTime> longestFinishTime_RR) {
            longesttQueue_RR = currentQueue_RR;
            longestFinishTime_RR = currentFinishTime;
        }
    }
    // short job first 
    PiorityQueue* longestQueue_SJF = processorsList_SJF[0].Get_RDY_List();
    int longestFinishTime_SJF = Get_Sum_CT_SJF(longestQueue_SJF);
    for (int i = 1; i < numSJF; i++) {
        PiorityQueue* currentQueue_SJF = processorsList_SJF[i].Get_RDY_List();
        int currentFinishTime = Get_Sum_CT_SJF(currentQueue_SJF);

        if (currentFinishTime > longestFinishTime_SJF) {
            longestQueue_SJF = currentQueue_SJF;
            longestFinishTime_SJF = currentFinishTime;
        }
    }
    
    //---------------------------------------------------------------------//
     // --------------------------stolen process ---------------------------//
    // case 1 the shortest is FCFS
    if (shortestFinishTime_FCFS < shortestFinishTime_RR && shortestFinishTime_FCFS < shortestFinishTime_SJF)
    {
        if (longesttFinishTime_FCFS > longestFinishTime_RR && longesttFinishTime_FCFS > longestFinishTime_SJF)
        {
            while ((longesttFinishTime_FCFS - shortestFinishTime_FCFS) / longesttFinishTime_FCFS > 0.4)
            {
                Process* stolenProcess = longesttQueue_FCFS->Remove();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_FCFS->enqueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longesttQueue_FCFS->enqueue(stolenProcess);
                }
               
            }
        }
        if (longestFinishTime_RR > longesttFinishTime_FCFS && longestFinishTime_RR > longestFinishTime_SJF)
        {
            while ((longestFinishTime_RR - shortestFinishTime_FCFS) / longestFinishTime_RR > 0.4)
            {
                Process* stolenProcess = longesttQueue_RR->Remove();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_FCFS->enqueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longesttQueue_RR->enqueue(stolenProcess);
                }
               
            }
        }
        if (longestFinishTime_SJF > longesttFinishTime_FCFS && longestFinishTime_SJF > longestFinishTime_RR)
        {
            while ((longestFinishTime_SJF - shortestFinishTime_FCFS) / longestFinishTime_SJF > 0.4)
            {
                Process* stolenProcess = longestQueue_SJF->Dequeue();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_FCFS->enqueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longestQueue_SJF->addPriorityQueue(stolenProcess);
                }
                
            }
        }
    }
    // case 2 the shortest is RR
    if (shortestFinishTime_RR < shortestFinishTime_FCFS && shortestFinishTime_RR < shortestFinishTime_SJF)
    {
        if (longesttFinishTime_FCFS > longestFinishTime_RR && longesttFinishTime_FCFS > longestFinishTime_SJF)
        {
            while ((longesttFinishTime_FCFS - shortestFinishTime_RR) / longesttFinishTime_FCFS > 0.4)
            {
                Process* stolenProcess = longesttQueue_FCFS->Remove();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_RR->enqueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longesttQueue_FCFS->enqueue(stolenProcess);
                }
               
            }
        }
        if (longestFinishTime_RR > longesttFinishTime_FCFS && longestFinishTime_RR > longestFinishTime_SJF)
        {
            while ((longestFinishTime_RR - shortestFinishTime_RR) / longestFinishTime_RR > 0.4)
            {
                Process* stolenProcess = longesttQueue_RR->Remove();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_RR->enqueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longesttQueue_RR->enqueue(stolenProcess);
                }
               
            }
        }
        if (longestFinishTime_SJF > longesttFinishTime_FCFS && longestFinishTime_SJF > longestFinishTime_RR)
        {
            while ((longestFinishTime_SJF - shortestFinishTime_RR) / longestFinishTime_SJF > 0.4)
            {
                Process* stolenProcess = longestQueue_SJF->Dequeue();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_RR->enqueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longestQueue_SJF->addPriorityQueue(stolenProcess);
                }
                
            }
        }
    }
    // case 3 the shortest is SJF
    if (shortestFinishTime_SJF < shortestFinishTime_FCFS && shortestFinishTime_SJF < shortestFinishTime_RR)
    {
        if (longesttFinishTime_FCFS > longestFinishTime_RR && longesttFinishTime_FCFS > longestFinishTime_SJF)
        {
            while ((longesttFinishTime_FCFS - shortestFinishTime_SJF) / longesttFinishTime_FCFS > 0.4)
            {
                Process* stolenProcess = longesttQueue_FCFS->Remove();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_SJF->addPriorityQueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longesttQueue_FCFS->enqueue(stolenProcess);
                }
                
            }
        }
        if (longestFinishTime_RR > longesttFinishTime_FCFS && longestFinishTime_RR > longestFinishTime_SJF)
        {
            while ((longestFinishTime_RR - shortestFinishTime_SJF) / longestFinishTime_RR > 0.4)
            {
                Process* stolenProcess = longesttQueue_RR->Remove();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_SJF->addPriorityQueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longesttQueue_RR->enqueue(stolenProcess);
                }
                
            }
        }
        if (longestFinishTime_SJF > longesttFinishTime_FCFS && longestFinishTime_SJF > longestFinishTime_RR)
        {
            while ((longestFinishTime_SJF - shortestFinishTime_SJF) / longestFinishTime_SJF > 0.4)
            {
                Process* stolenProcess = longestQueue_SJF->Dequeue();
                if (!(stolenProcess->getPID() > numOFProcesses))
                {
                    shortestQueue_SJF->addPriorityQueue(stolenProcess);
                    numofworkstealing++;
                }
                else
                {
                    longestQueue_SJF->addPriorityQueue(stolenProcess);
                }
              
            }
        }
    }
    cout << "End work stealing"<<endl;
   
}

void Schedular::handleoverheat_FCFS(FCFS_Processor* processor, int stoptime)
{
    cout << "Start handle overheat_FCFS" << endl;
    processor->setprocessorstate(Stop);
    processor->setstoptime(stoptime);
    //****** need to make functions to improve the repeated code //
    linkedQueue* shortestQueue_FCFS = processorsList_FCFS[0].Get_RDY_List();
    int shortestFinishTime_FCFS = Get_Sum_CT_FCFS(shortestQueue_FCFS);
    for (int i = 1; i < numFCFS; i++) {
        linkedQueue* currentQueue_FCFS = processorsList_FCFS[i].Get_RDY_List();
        int currentFinishTime = Get_Sum_CT_FCFS(currentQueue_FCFS);

        if (currentFinishTime < shortestFinishTime_FCFS) {
            shortestQueue_FCFS = currentQueue_FCFS;
            shortestFinishTime_FCFS = currentFinishTime;
        }
    }
    Process* movedrunprocess = processor->GetRunProcess();
   // processor-> dequeu the runprocess from the list
    if (movedrunprocess != nullptr)
    {
        shortestQueue_FCFS->enqueue(movedrunprocess);
    }
 
    // Move processes in RDY queues of the malfunctioning processor to the shortest RDY queue
   
         
            while (!processor->Get_RDY_List()->isEmpty()) 
            {
                Process* movedprocess = processor->Get_RDY_List()->Remove();
                if (movedrunprocess != nullptr)
                {
                    shortestQueue_FCFS->enqueue(movedprocess);
                }
         
            }
        
   

  

            cout << "End handle overheat_FCFS" << endl;
}
void Schedular::handleoverheat_RR(RR_Processor* processor, int stoptime)
{

    cout << "strat handle overheat_RR" << endl;
    processor->setprocessorstate(Stop);
    processor->setstoptime(stoptime);
    CircularQueue* shortestQueue_RR = processorsList_RR[0].getrdylist();
    int shortestFinishTime_RR = Get_Sum_CT_RR(shortestQueue_RR);
    for (int i = 1; i < numRR; i++)
    {
        CircularQueue* currentQueue_RR = processorsList_RR[i].getrdylist();
        int currentFinishTime = Get_Sum_CT_RR(currentQueue_RR);

        if (currentFinishTime < shortestFinishTime_RR)
        {
            shortestQueue_RR = currentQueue_RR;
            shortestFinishTime_RR = currentFinishTime;
        }
    }
    Process* movedrunprocess = processor->GetRunProcess();
    // processor-> dequeu the runprocess from the list
    if (movedrunprocess != nullptr)
    {
        shortestQueue_RR->enqueue(movedrunprocess);

    }
    // Move processes in RDY queues of the malfunctioning processor to the shortest RDY queue
   

        while (!processor->getrdylist()->isEmpty())
        {

            Process* movedprocess = processor->getrdylist()->Remove();
            if (movedrunprocess != nullptr)// check fun remove
            {
                shortestQueue_RR->enqueue(movedprocess);
            }
         
        }




        cout << "End handele overheat_RR" << endl;
}
void Schedular::handleoverheat_SJF(SJF_Processor* processor, int stoptime)
{
    cout << "start handele overheat_SJF" << endl;
    processor->setprocessorstate(Stop);
    processor->setstoptime(stoptime);
    PiorityQueue* shortestQueue_SJF = processorsList_SJF[0].Get_RDY_List();
    int shortestFinishTime_SJF = Get_Sum_CT_SJF(shortestQueue_SJF);
    for (int i = 1; i < numSJF; i++)
    {
        PiorityQueue* currentQueue_SJF = processorsList_SJF[i].Get_RDY_List();
        int currentFinishTime = Get_Sum_CT_SJF(currentQueue_SJF);

        if (currentFinishTime < shortestFinishTime_SJF)
        {
            shortestQueue_SJF = currentQueue_SJF;
            shortestFinishTime_SJF = currentFinishTime;
        }
    }
    Process* movedrunprocess = processor->GetRunProcess();
    // processor-> dequeu the runprocess from the list
    if (movedrunprocess != nullptr)
    {
        shortestQueue_SJF->addPriorityQueue(movedrunprocess);
    }
   
    // Move processes in RDY queues of the malfunctioning processor to the shortest RDY queue
    

        while (!processor->Get_RDY_List()->isEmpty())
        {
            Process* movedprocess = processor->Get_RDY_List()->Dequeue();
            if (movedrunprocess != nullptr)
            {
                shortestQueue_SJF->addPriorityQueue(movedprocess);
            }
            
        }


        cout << "End handele overheat_SJF" << endl;
}

void Schedular::OverHeating()
{
    cout << "start overheating" << endl;
    for (int g = 0; g < numFCFS; g++)
    {
        cout << "cehck FCFS overheating" << endl;
        if (processorsList_FCFS[g].processor_overheated())
        {
            handleoverheat_FCFS(&processorsList_FCFS[g], 30);
        }
       
       

    }
    for (int g = 0; g < numSJF; g++)
    {
        cout << "cehck SJF overheating" << endl;
        if (processorsList_SJF[g].processor_overheated())
        {
            handleoverheat_SJF(&processorsList_SJF[g], 20);
        }
       
    }

    for (int g = 0; g < numRR; g++)
    {
        cout << "cehck RR overheating" << endl;
        if (processorsList_RR[g].processor_overheated())
        {
            handleoverheat_RR(&processorsList_RR[g], 20);
        }
       

    }
    cout << "End overheating" << endl;
}

void Schedular::CoolingProcessor()
{
    cout << "start cooling"<<endl;
    for (int g = 0; g < numFCFS; g++)
    {
        processorsList_FCFS[g].setstoptime(processorsList_FCFS[g].getstoptime() - 1);
        if (processorsList_FCFS[g].getstoptime() == 0 && processorsList_FCFS[g].getprocessorstate()==Stop)
        {
            processorsList_FCFS[g].setprocessorstate(run);
        }
    }
    for (int g = 0; g < numSJF; g++)
    {
        processorsList_SJF[g].setstoptime(processorsList_SJF[g].getstoptime() - 1);
        if (processorsList_SJF[g].getstoptime() == 0 && processorsList_SJF[g].getprocessorstate() == Stop)
        {
            processorsList_SJF[g].setprocessorstate(run);
        }
    }
    for (int g = 0; g < numRR; g++)
    {
        processorsList_RR[g].setstoptime(processorsList_RR[g].getstoptime() - 1);
        if (processorsList_RR[g].getstoptime() == 0 && processorsList_RR[g].getprocessorstate() == Stop)
        {
            processorsList_RR[g].setprocessorstate(run);
        }
    }
    cout << "end cooling" << endl;
}



    void Schedular::InitiateFork() {

    for (int i = 0; i < numFCFS; i++)
    {
        if (!processorsList_FCFS[i].GetRunProcess()) continue;

        int rand_num = rand();
        double prob = (double)rand_num / RAND_MAX;
        if (prob <= forkprobability && !processorsList_FCFS[i].GetRunProcess()->getChild())
        {

            Process* P = new Process(timeStep, UniqueChildID, processorsList_FCFS[i].GetRunProcess()->getRemainingTime(), 0, nullptr, 0);

            UniqueChildID++;
            processorsList_FCFS[i].GetRunProcess()->setChild(P);
            P->SetState(RDY);
            Insert_Shortest_RDY_List(P);
            
        }
    }

}

void Schedular::KillSignal() {
    if (Kill_List_Copy == nullptr) {
        Kill_List_Copy = new ArrayQueue(*Kill_List);
    }
    else {
        
        while (!Kill_List_Copy->isEmpty()) {
            Kill_List_Copy->deleteQueue();
        }

        *Kill_List_Copy = *Kill_List;
    }

    int length = Kill_List_Copy->getLength();

    for (int i = 0; i < length / 2; i++) {
        int Kill_Time = Kill_List_Copy->getFront();
        Kill_List_Copy->deleteQueue();
        int Kill_ID = Kill_List_Copy->getFront();
        Kill_List_Copy->deleteQueue();

        int FCFSIndex;
        int size;

        for (int i = 0; i < numFCFS; i++) {
            Process** Children = processorsList_FCFS[i].Signal_Kill(Kill_Time, Kill_ID, timeStep, TRM_List, size);
            if (Children)
            {

                KillChildren(Children, size);
                
            }
        }
    }
}

void Schedular::KillChildren(Process** ChildList, int size)
{
    if (ChildList == nullptr)
        return;

    for (int i = 0; i < size; i++)
    {
        if (ChildList[i] != nullptr)
        {
            int FCFSIndex = DetectFCFSProcessor(ChildList[i]);
            int newSize;
            int ID = ChildList[i]->getPID();
            Process** Children = processorsList_FCFS[FCFSIndex].Signal_Kill(timeStep, ID, timeStep, TRM_List, newSize);
            if (Children) KillChildren(Children, newSize);
        }
    }

    delete[] ChildList;
}

int Schedular::DetectFCFSProcessor(Process* ProcessReceived)
{
    for (int i = 0; i < numFCFS; i++)
    {
        if (processorsList_FCFS[i].IsProcessor(ProcessReceived)) return i;
    }
}

void Schedular::Interactive_Mode() {
    ReadFile(userInterface->getfilepath());

    while (userInterface->Next_Step() && TRM_List->getLength() != numOFProcesses) {

if (!NEW_List->isEmpty()) {
            for (int i = 0; i < NEW_List->listSize(); i++) {
                if (NEW_List->getProcess(i)->getArrivalTime() == timeStep) {
                    Insert_Shortest_RDY_List(NEW_List->getProcess(i));
                }
            }
        }
        
        RDY_to_RUN(timeStep);
        InitiateFork();
        KillSignal();
        



        RUN_to_BLK();
        BLK_to_RDY();
        Run_to_TRM(timeStep);

       
        // cooling the stoping  process
         // migration 1 RR processors
         for (int k = 0; k < numRR; k++)
         {
             cout << "start mig now" << endl;
             Migration_RR_TO_SJF(processorsList_RR[k].GetRunProcess());
             cout << "End mig  SJF" << endl;
         }
         // migration 2 FCFS prosseors
         for (int k = 0; k < numFCFS; k++)
         {
             cout << "start second mig now" << endl;
             Migration_FCFS_TO_RR(processorsList_FCFS[k].GetRunProcess());
             cout << "End second mig now" << endl;
         }
          
        
         WorkStealing();
         // ------------ Overheating----------------/
         OverHeating();
         CoolingProcessor();

        userInterface->printProcessInfo(processorsList_FCFS, numFCFS, processorsList_SJF, numSJF, processorsList_RR, numRR, BLK_List, TRM_List, timeStep);

        timeStep++;
        updateCPUTimeUsed();
    }
}
void Schedular::Silent_Mode() {
    OutputFile(TRM_List);
}


void Schedular::selectionSort(linkedQueue* trm) {
    int i, j, minIndex;
    Process* tmp = nullptr;

    for (i = 0; i < trm->getLength()-1; i++) {
        minIndex = i;

        for (j = i + 1; j < trm->getLength(); j++) {
            if (trm->getItem(j)->getTT() < trm->getItem(minIndex)->getTT()) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            tmp = trm->getItem(i);
            trm->setItem(trm->getItem(minIndex), i);
            trm->setItem(tmp, minIndex);
        }
    }
}
void Schedular::OutputFile(linkedQueue* trmList) {
    ofstream output;
    output.open("OUTPUT.txt", std::ios::trunc);

    output << "TT PID CT IO_D WT RT TRT\n";

    selectionSort(trmList);

    int sumWT = 0, sumRT = 0, sumTRT = 0;

    for (int i = 0; i < trmList->getLength(); i++) {
        trmList->getItem(i)->setRT();
        trmList->getItem(i)->setTRT();
        trmList->getItem(i)->setWT();

        sumWT += trmList->getItem(i)->getWT();
        sumRT += trmList->getItem(i)->getRT();
        sumTRT += trmList->getItem(i)->getTRT();


        output << trmList->getItem(i)->getTT() << " " << trmList->getItem(i)->getPID() << " " << trmList->getItem(i)->getCPUtime() << " " << trmList->getItem(i)->getTotal_IOD() << " ";
        output << trmList->getItem(i)->getWT() << " " << trmList->getItem(i)->getRT() << " " << trmList->getItem(i)->getTRT() << "\n";
    }

    avgWT = sumWT / numOFProcesses;
    avgRT = sumRT / numOFProcesses;
    avgTRT = sumTRT / numOFProcesses;

    output << "Processes: " << numOFProcesses << "\n";
    output << "AVG WT = " << avgWT << "   " << "AVG RT = " << avgRT << "   " << "AVG TRT = " << avgTRT << "\n";

    output << "Migration %: " << "\n";
    output << "Work Steal %: " << "\n";
    output << "Forked Processes %: " << "\n";
    output << "Killed Processes %: " << "\n" << "\n";

    output << "Processors: " << (numFCFS + numSJF + numRR) << " [" << numFCFS << " FCFS, " << numSJF << " SJF, " << numRR << " RR]\n";

    output << "Processors Load\n";
    output << "P1: " << "P1: " << "P1: " << "P1: " << "\n";

    output << "Processors Utiliz\n";
    output << "P1: " << "P1: " << "P1: " << "P1: " << "\n";

    output << "AVG Utilization = ";




    output.close();
}
















//void Schedular::test() {
//    Process* P1 = new Process(1, 2, 3, 0, nullptr);
//    Process* P2 = new Process(2, 4, 5, 0, nullptr);
//    Process* P3 = new Process(3, 6, 7, 0, nullptr);
//    Process* P4 = new Process(4, 8, 9, 0, nullptr);
//    Process* P5 = new Process(5, 10, 11, 0, nullptr);
//    processorsList_FCFS[0].Get_RDY_List()->enqueue(P1);
//    processorsList_FCFS[0].Get_RDY_List()->enqueue(P4);
//
//    processorsList_SJF[0].Get_RDY_List()->addPriorityQueue(P2);
//    processorsList_SJF[0].Get_RDY_List()->addPriorityQueue(P5);
//
//    processorsList_RR[0].getrdylist()->enqueue(P3);
//    processorsList_RR[0].getrdylist()->enqueue(P4);
//
//    
//    processorsList_FCFS[1].Get_RDY_List()->enqueue(P2);
//    processorsList_FCFS[1].Get_RDY_List()->enqueue(P5);
//
//    processorsList_SJF[1].Get_RDY_List()->addPriorityQueue(P3);
//    processorsList_SJF[1].Get_RDY_List()->addPriorityQueue(P4);
//
//    processorsList_RR[1].getrdylist()->enqueue(P4);
//    processorsList_RR[1].getrdylist()->enqueue(P5);
//
//    
//    processorsList_FCFS[2].Get_RDY_List()->enqueue(P3);
//    processorsList_FCFS[2].Get_RDY_List()->enqueue(P1);
//
//    processorsList_SJF[2].Get_RDY_List()->addPriorityQueue(P5);
//    processorsList_SJF[2].Get_RDY_List()->addPriorityQueue(P4);
//
//
//    
//    processorsList_FCFS[3].Get_RDY_List()->enqueue(P1);
//    processorsList_FCFS[3].Get_RDY_List()->enqueue(P4);
//
//    processorsList_SJF[3].Get_RDY_List()->addPriorityQueue(P4);
//    processorsList_SJF[3].Get_RDY_List()->addPriorityQueue(P2);
//
//    
//    processorsList_FCFS[4].Get_RDY_List()->enqueue(P2);
//    processorsList_FCFS[4].Get_RDY_List()->enqueue(P5);
//
//
//}

Schedular::~Schedular()
{
}

//void Schedular::RDY_to_RUN() {
//    for (int i = 0; i < numFCFS; i++) {
//        processorsList_FCFS[i].ScheduleAlgo();
//
//        srand(time(nullptr));
//        random_number = rand() % 100 + 1;
//
//        if (random_number >= 1 && random_number <= 15)
//            BLK_List->enqueue(processorsList_FCFS[i].GetRunProcess());
//        else
//            if (random_number >= 20 && random_number <= 30)
//                processorsList_FCFS[i].Get_RDY_List()->enqueue(processorsList_FCFS[i].GetRunProcess());
//            else
//                if (random_number >= 50 && random_number <= 60)
//                    TRM_List->enqueue(processorsList_FCFS[i].GetRunProcess());
//    }
//
//    for (int i = 0; i < numSJF; i++) {
//        processorsList_SJF[i].ScheduleAlgo();
//
//        srand(time(nullptr));
//        random_number = rand() % 100 + 1;
//
//        if (random_number >= 1 && random_number <= 15)
//            BLK_List->enqueue(processorsList_SJF[i].GetRunProcess());
//        else
//            if (random_number >= 20 && random_number <= 30)
//                processorsList_SJF[i].Get_RDY_List()->addPriorityQueue(processorsList_SJF[i].GetRunProcess());
//            else
//                if (random_number >= 50 && random_number <= 60)
//                    TRM_List->enqueue(processorsList_SJF[i].GetRunProcess());
//    }
//
//    for (int i = 0; i < numRR; i++) {
//        processorsList_RR[i].ScheduleAlgo();
//
//        srand(time(nullptr));
//        random_number = rand() % 100 + 1;
//
//        if (random_number >= 1 && random_number <= 15)
//            BLK_List->enqueue(processorsList_RR[i].GetRunProcess());
//        else
//            if (random_number >= 20 && random_number <= 30)
//                processorsList_RR[i].getrdylist()->enqueue(processorsList_RR[i].GetRunProcess());
//            else
//                if (random_number >= 50 && random_number <= 60)
//                    TRM_List->enqueue(processorsList_RR[i].GetRunProcess());
//    }
//}
// 
// 
// 
//srand(time(nullptr));
       //random_number = rand() % 100 + 1;
       //if (random_number < 10) {
       //    processorsList_FCFS[0].Get_RDY_List()->enqueue(BLK_List->front());
       //    processorsList_SJF[0].Get_RDY_List()->addPriorityQueue(BLK_List->front());
       //    processorsList_RR[0].getrdylist()->enqueue(BLK_List->front());

     

//// Generating Random ID to Pick a process from the RDY list of a FCFS Processor, then kill it
//for (int k = 0; k < numFCFS; k++) {
//    int MAXSize = processorsList_FCFS[k].Get_RDY_List()->getLength();
//    srand(time(nullptr));
//    random_id = rand() % (MAXSize);
//    Process* P_Kill = processorsList_FCFS[k].Get_RDY_List()->getItem(random_id);
//    P_Kill->SetState(TRM);
//    processorsList_FCFS[k].Get_RDY_List()->KillProcess(P_Kill);

//}


