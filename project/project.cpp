#include <iostream>
#include "LinkedList.h"
#include "Node.h"
#include "Process.h"
#include"QueueLinkedList.h"
#include"QueueArray.h"
#include "Schedular.h"
#include"PiorityQueue.h"
#include<list>
#include "UI.h"
#include<string>

using namespace std;

int main()
{
	
	Schedular Schedul;
	cout << "chose 1 for the interactive mode" << endl;
	cout << "chose 2 for the step_bystep_mode" << endl;
	cout << "chose 3 for the silentmode" << endl;
	int mode;
	cin >> mode;
	if (mode == 1)
	{
		Schedul.Interactive_Mode();
		Schedul.Silent_Mode();
	}
	if (mode == 2)
	{
		// Schedul.step_bystep_mode();
	}
	if (mode == 3)
	{
		Schedul.Silent_Mode();
	}
	return 0;











	////Testing The Node and Process
	//Process P1(20);
	//Node N1(P1);

	////Testing The Linked List
	//LinkedList L;
	//L.insertFirst(P1);
	//cout << L.listSize() << endl;
	//L.print();

	////Testing The Queue LinkedList
	//linkedQueue Q;
	//Q.enqueue(P1);
	//cout << Q.front().pID << endl;
	//cout << Q.rear().pID << endl;
	//Q.search(P1);
	//Q.display();
	//Q.clearQueue();
	//Q.display();

	////Testing The Queue Array
	//ArrayQueue Q2(2);
	//Q2.addQueue(P1);
	//Q2.printQueue();
	//Q2.queueSearch(P1);
	//cout << Q2.frontQueue().pID << endl;
	//cout << Q2.rearQueue().pID << endl;
	//Q2.deleteQueue();
	//Q2.printQueue();



	/*Schedular S;
	S.ReadFile();*/
	//PiorityQueue queue; // Test the PiorityQueue
	//list<pair<int, int>> ioList1 = { {1, 2}, {3, 4} };
	//list<pair<int, int>> ioList2 = { {5, 6}, {7, 8}, {9, 10} };

	//// Create some processes and add them to the priority queue
	//Process* p1 = new Process(0, 1, 10, 2, ioList1);
	//Process* p2 = new Process(5, 2, 5, 3, ioList2);
	//Process* p3 = new Process(10, 3, 15, 1, {});

	//if (!p1 || !p2 || !p3) {
	//	cout << "Failed to allocate memory for processes" << endl;
	//	return 1;
	//}

	//queue.addPriorityQueue(p1);
	//queue.addPriorityQueue(p2);
	//queue.addPriorityQueue(p3);



	//// Dequeue and process the processes in order of their CPU time
	//while (!queue.isEmpty()) {
	//	Process* current = queue.Dequeue();
	//	current->SetState(RUN);
	//	current->display();
	//	// Process the current process here
	//	// ...
	//	current->SetState(TRM);
	//	delete current;
	//}
}

