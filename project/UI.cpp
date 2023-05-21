#include "UI.h"

string UI::getfilepath()
{
	cout << "write the file path filename.txt \n";
	string filepath;
	getline(cin, filepath);
	getline(cin, filepath);
	return filepath;
}


void UI::printProcessInfo(FCFS_Processor* p1, int numofFCFS, SJF_Processor* p2,
							int numoSJF, RR_Processor* p3, int numoRR, linkedQueue* BLK_List,
							linkedQueue* TRM_List, int timestep)
{
	cout << "current time step : " << timestep << endl;
	cout << "FCFS processors " << endl;
	for (int i = 0; i < numofFCFS; i++)
	{
		cout << "----------------- RDY PROCESSES IN PROCESSOR " << i + 1 << "-----------------------" << endl;
		p1[i].Get_RDY_List()->print();
	}


	cout << "SJF processors " << endl;
	for (int i = 0; i < numoSJF; i++)
	{
		cout << "----------------- RDY PROCESSES IN PROCESSOR " << i + 1 << "-----------------------" << endl;

		p2[i].Get_RDY_List()->print();
	}



	cout << "RR processors " << endl;
	for (int i = 0; i < numoRR; i++)
	{
		cout << "----------------- RDY PROCESSES IN PROCESSOR " << i + 1 << "-----------------------" << endl;
		p3[i].getrdylist()->print();
	}



	cout << "----------------- BLK PROCESSES-----------------------" << endl;
	
		BLK_List->print();
	

	cout << "----------------- RUN PROCESSES PROCESSOR FCFS" << "-----------------------" << endl;
	for (int i = 0; i < numofFCFS; i++) // FCFS RUN PROCESSORS
	{
		Process* run = p1[i].GetRunProcess();
		cout << "proccessor ( " << i + 1 << ")";
		if (run->getPID() != -1)
			cout << "    Process ID: " << run->getPID() << endl;
		else
			cout << endl;
	}

	cout << "----------------- RUN PROCESSES PROCESSOR SJF " << "-----------------------" << endl;
	for (int i = 0; i < numoSJF; i++)
	{
		Process* run = p2[i].GetRunProcess();
		cout << "proccessor ( " << i + 1 << ")";
		if (run->getPID() != -1)
			cout << "    Process ID: " << run->getPID() << endl;
		else
			cout << endl;
	}

	cout << "----------------- RUN PROCESSES PROCESSOR RR " << "-----------------------" << endl;
	for (int i = 0; i < numoRR; i++)
	{
		Process* run = p3[i].GetRunProcess();
		cout << "proccessor ( " << i+1 << ")";
		if (run->getPID() != -1)
			cout << "    Process ID: " << run->getPID() << endl;
		else
			cout << endl;
	}

	cout << "----------------- TRM PROCESSES-----------------------" << endl;
	TRM_List->print();
	

}

bool UI::Next_Step() {
	char option;
	cin >> option;
	
	return !(option == '0');

}