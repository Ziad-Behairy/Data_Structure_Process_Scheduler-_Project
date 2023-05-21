#pragma once
#include <iostream>
#include <cassert>
#include "Node.h"
#include"Process.h"

using namespace std;

class linkedQueue
{
private:
	int length;
	Node* frontPtr, * rearPtr;

public:

	linkedQueue();
	bool isEmpty();
	int getLength() {
		return length;
	}

	void dequeue();

	void KillProcess(Process* item);
	Process* Remove();
	void enqueue(Process* item);
	Process* front();
	Process* rear();

	Process* getItem(int index) {
		Node* current = frontPtr;
		for (int i = 0; i < index; i++)
			current = current->next;
		return current->data;
	}


	void setItem(Process* P, int index) {
		Node* current = frontPtr;
		for (int i = 0; i < index; i++)
			current = current->next;
		current->data = P;
	}



	void clearQueue();

	void print();

	//void display();
	//void search(Process* item);
};
