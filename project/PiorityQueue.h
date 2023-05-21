#pragma once
#include "Process.h"
#include <stdexcept>
#include <iostream>
using namespace std;
class PiorityQueue  // using heapsort and arrays 
{
private:
    Process** arr;
    int maxSize;
    int currentSize;
    int front;
    int rear;

    void swap(Process& a, Process& b);
    void heapifyUp(Process** arr, int size, int index);
    void heapifyDown(Process** arr, int size, int index);

public:
    PiorityQueue();
    ~PiorityQueue();

    bool isEmpty();
    bool isFull();
    int getsize();
    Process* getItem(int index);
    void addPriorityQueue(Process* element);
    Process* Dequeue();

   void print();
};


