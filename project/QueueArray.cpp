#include "QueueArray.h"



ArrayQueue::ArrayQueue(int size){
	if (size <= 0)
		maxSize = 100;
	else
		maxSize = size;

	front = 0;

	arry = new int [maxSize];
	rear = maxSize - 1;
	length = 0;
}

bool ArrayQueue::isEmpty()
{
	return length == 0;
}

bool ArrayQueue::isFull()
{
	return length == maxSize;
}


void ArrayQueue::addQueue(int Element)
{
	if (isFull())
	{
		cout << "Queue Full Can't Enqueue ...!";
	}
	else
	{
		rear = (rear + 1) % maxSize;
		arry[rear] = Element;
		length++;
	}


}

void ArrayQueue::deleteQueue()
{
	if (isEmpty())
	{
		cout << "Empty Queue Can't Dequeue ...!";
	}
	else
	{
		front = (front + 1) % maxSize;
		--length;

	}


}






ArrayQueue::~ArrayQueue() {
	delete[]arry;
}


