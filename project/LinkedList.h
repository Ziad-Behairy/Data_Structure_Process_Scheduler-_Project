#pragma once
#include"Node.h"
//#include"Process.h"

class LinkedList
{

private:
	Node* first, * last;
	int length;

public:
	LinkedList();
	int listSize();
	bool isEmpty();

	int seqSearch(Process*);

	void remove(Process*);
	void insertFirst(Process*);
	void insertEnd(Process*);
	void insertAt(int, Process*);
	void removeAt(int);

	void print();
	

	//inline void print();

	void clearList();
	void removeFirst();
	void removeLast();
	void reverse();

	Process* getProcess(int index);

	~LinkedList();
};