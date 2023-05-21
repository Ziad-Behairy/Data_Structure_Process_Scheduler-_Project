#include <iostream>
#include"LinkedList.h"
using namespace std;

LinkedList::LinkedList()
{
	first = last = NULL;
	length = 0;
}

void LinkedList::removeLast()
{
	if (length == 0)
		cout << "ERROR: EMPTY LIST" << endl;
	else if (length == 1)
	{
		delete first;
		last = first = NULL;
		length--;
	}
	else
	{
		Node* current = first;
		while (current->next != last)
			current = current->next;

		delete last;
		current->next = NULL;
		last = current;
		length--;
	}
}

void LinkedList::removeFirst()
{
	if (length == 0)
		cout << "ERROR: EMPTY LIST" << endl;
	else if (length == 1)
	{
		delete first;
		last = first = NULL;
		length--;
	}
	else
	{
		Node* current = first;
		first = first->next;
		delete current;
		length--;
	}
}


int LinkedList::listSize()
{
	return length;
}

bool LinkedList::isEmpty()
{
	return (length == 0);
}

int LinkedList::seqSearch(Process* item)
{
	Node* current = first;
	int loc = 0;
	while (current != NULL)
	{
		if (current->data == item)
			return loc;
		current = current->next;
		loc++;
	}
	return -1;
}

void LinkedList::remove(Process* item)
{
	if (isEmpty())
	{
		cout << "Can not remove from empty list\n";
		return;
	}

	Node* current, * trailCurrent;
	if (first->data == item)   //delete the first element, special case
	{
		current = first;

		first = first->next;
		delete current;
		length--;
		if (length == 0)
			last = NULL;
	}
	else
	{
		current = first->next;
		trailCurrent = first;
		while (current != NULL)
		{
			if (current->data == item)
				break;
			trailCurrent = current;
			current = current->next;
		}

		if (current == NULL)
			cout << "The item is not there\n";
		else
		{
			trailCurrent->next = current->next;
			if (last == current) //delete the last item
				last = trailCurrent;
			delete current;
			length--;
		}
	}
}


void LinkedList::insertFirst(Process* item)
{
	Node* newNode = new Node(item);
	if (length == 0) {
		first = last = newNode;
		newNode->next = NULL;
	}
	else {
		newNode->next = first;
		first = newNode;
	}
	length++;
}

void LinkedList::insertEnd(Process* item)
{
	Node* newNode = new Node(item);
	if (length == 0) {
		first = last = newNode;
		newNode->next = NULL;
	}
	else {
		last->next = newNode;
		newNode->next = NULL;
		last = newNode;
	}
	length++;
}


void LinkedList::insertAt(int loc, Process* item)
{
	if (loc < 0 || loc > length)
		cout << "ERROR: Out of range" << endl;
	else
	{
		Node* newNode = new Node(item);
		if (loc == 0) //insert at the begining
			insertFirst(item);
		else if (loc == length) //insert at the end;
			insertEnd(item);
		else
		{
			Node* current = first;
			for (int i = 1; i < loc; i++)
				current = current->next;

			newNode->next = current->next;
			current->next = newNode;
			length++;
		}

	}
}

void LinkedList::removeAt(int loc)
{
	if (loc < 0 || loc >= length)
		cout << "ERROR: Out of range" << endl;
	else
	{
		Node* current, * trailCurrent;
		if (loc == 0)
		{
			current = first;
			first = first->next;
			delete current;
			length--;
			if (length == 0)
				last = NULL;
		}
		else
		{
			current = first->next;
			trailCurrent = first;
			for (int i = 1; i < loc; i++)
			{
				trailCurrent = current;
				current = current->next;
			}

			trailCurrent->next = current->next;
			if (last == current) //delete the last item
				last = trailCurrent;
			delete current;
			length--;
		}
	}
}

void LinkedList::print()
{
	Node* current = first;
	while (current != NULL) {
		std::cout << "Process ID: " << current->data->getPID() << std::endl;
		current = current->next;
	}
}

// Watch Out --> Function using Cout




void LinkedList::clearList()
{
	Node* current;
	while (first != NULL)
	{
		current = first;
		first = first->next;
		delete current;
	}
	last = NULL;
	length = 0;
}








void LinkedList::reverse() {
	Node* prev, * next, * curr;
	prev = NULL;
	curr = first;
	next = curr->next;
	while (next != NULL)//curr!=NULL
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	first = prev;
}


Process* LinkedList::getProcess(int index) {
	Node* current = first;
	for (int g = 0; g < index; g++)
		current = current->next;

	return current->data;
}




LinkedList::~LinkedList()
{
	clearList();
}







