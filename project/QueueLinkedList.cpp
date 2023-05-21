#include "QueueLinkedList.h"

linkedQueue::linkedQueue(){
	frontPtr = NULL;
	rearPtr = NULL;
	length = 0;
}


bool linkedQueue::isEmpty()
{
	return (length == 0);
}

void linkedQueue::dequeue()
{
	if (isEmpty())
		cout << "Empty Queue" << endl;
	else if (length == 1)
	{
		delete frontPtr;
		frontPtr = rearPtr = NULL;
		length = 0;
	}
	else
	{
		Node* current = frontPtr;
		frontPtr = frontPtr->next;
		current->next = NULL;
		delete current;//free(current)
		length--;
	}
}



void linkedQueue::KillProcess(Process* item)
{
	if (isEmpty())
		cout << "Empty Queue" << endl;
	else if (length == 1)
	{
		delete frontPtr;
		frontPtr = rearPtr = NULL;
		length = 0;
	}
	else
	{
		int c = 0;
		Node* current = frontPtr, *preCurrent = frontPtr;
		while (item->getPID() != current->data->getPID()) {
			if(c==0)
				current = current->next;
			else {
				preCurrent = preCurrent->next;
				current = current->next;
			}
			c++;
				
		}
		preCurrent->next = current->next;
		current->next = NULL;
		delete current;//free(current)
		length--;
	}
}

Process* linkedQueue::Remove()
{
	if (isEmpty())
	{
		return nullptr;
	}
	else if (length == 1)
	{
		Process* removedProcess = frontPtr->data;
		delete frontPtr;
		frontPtr = rearPtr = nullptr;
		length = 0;
		return removedProcess;
	}
	else
	{
		Node* current = frontPtr;
		frontPtr = frontPtr->next;
		current->next = nullptr;
		Process* removedProcess = current->data;
		delete current;
		length--;
		return removedProcess;
	}
}



void linkedQueue::enqueue(Process* item)
{
	Node* newNode = new Node(item);
	newNode->next = NULL;

	if (length == 0)
		rearPtr = frontPtr = newNode;
	else
	{
		rearPtr->next = newNode;
		rearPtr = newNode;
	}
	length++;
}

Process* linkedQueue::front()
{
	assert(!isEmpty());
	return frontPtr->data;
}

Process* linkedQueue::rear()
{
	assert(!isEmpty());
	return rearPtr->data;
}

void linkedQueue::clearQueue()
{
	Node* current;

	while (frontPtr != NULL)
	{
		current = frontPtr;
		frontPtr = frontPtr->next;
		current->next = NULL;
		delete current;
	}
	rearPtr = NULL;
	length = 0;
}

void linkedQueue::print()
{
	Node* current = frontPtr;
	while (current != NULL) {
		if (!isEmpty()) {
			cout << "Process ID: " << current->data->getPID() << endl;
			current = current->next;
		}
	}
}


//void linkedQueue::display()
//{
//	Node* cur = frontPtr;
//	cout << "Item in the queue :[ ";
//	while (cur != NULL)
//	{
//		cout << cur->data->getPID() << " ";
//		cur = cur->next;
//	}
//	cout << "]" << endl;
//}


//void linkedQueue::search(Process item)
//{
//	Node* cur = frontPtr;
//	bool flag = true;
//	while (cur != NULL)
//	{
//		if (cur->data == item)
//		{
//			cout << "the item :" << item.pID << " found" << endl;
//			flag = false;
//			break;
//		}
//		cur = cur->next;
//	}
//	if (flag)
//		cout << "the item : " << item.pID << " not found" << endl;
//
//}

