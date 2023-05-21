#pragma once
#include <iostream>
using namespace std;

class ArrayQueue
{
	int rear;
	int front;

	int* arry;

	int length;
	int maxSize;

public:

	ArrayQueue(int size);

	int getFront() {
		return arry[front];
	}
	int getRear() {
		return arry[rear];
	}

	int getIO_D() {
		return arry[front + 1];
	}

	int getLength() {
		return length;
	}
	
	bool isEmpty();
	bool isFull();

	void addQueue(int Element);
	void deleteQueue();


	


	~ArrayQueue();
};

