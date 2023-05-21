#pragma once
#include "Process.h"
#include <iostream>
#include <stdexcept>
using namespace std;
class CircularQueue {
public:
    CircularQueue();
    ~CircularQueue();
    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    void enqueue(Process* process);
    Process* getItem(int index);
    void dequeue();
    Process* front() const;
    Process* back() const;
    void print();
    Process* Remove();

private:
    void resize();

    static const int INITIAL_SIZE = 10;

    Process** data;
    int front_;
    int rear_;
    int maxSize;
    int currentSize;
};
