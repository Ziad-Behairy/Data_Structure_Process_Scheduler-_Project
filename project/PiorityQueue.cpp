
#include "PiorityQueue.h"

PiorityQueue::PiorityQueue() {
    this->maxSize = 1000;
    this->currentSize = 0;
    arr = new Process * [maxSize];
    front = 0;
    rear = -1;
}

PiorityQueue::~PiorityQueue() {
    delete[] arr;
}

bool PiorityQueue::isEmpty() {
    return currentSize == 0;
}

bool PiorityQueue::isFull() {
    return currentSize == maxSize;
}

int PiorityQueue::getsize()
{
    return currentSize;
}

Process* PiorityQueue::getItem(int index)
{
    if (index < 0 || index >= currentSize)
        return nullptr;

    return arr[front + index];
}

void PiorityQueue::swap(Process& a, Process& b) {
    Process temp = a;
    a = b;
    b = temp;
}

void PiorityQueue::addPriorityQueue(Process* element) {
    if (isFull()) {                               // exception handling for the queue if it is full 
        throw std::overflow_error("Queue is full");
    }
    else {
        currentSize++;
        int newindex = rear + 1;
        arr[newindex] = element;
        heapifyUp(arr, currentSize, newindex);
        rear = rear + 1;
    }
}

Process* PiorityQueue::Dequeue() {
    if (!isEmpty()) {
        Process* root = arr[front];
        front++;
        currentSize--;
        heapifyDown(arr, currentSize, front);
        return root;
    }
}

void PiorityQueue::print() {
    for (int i = front; i <= rear; i++) {
        if (!isEmpty())
            std::cout << "Process ID: " << arr[i]->getPID() << std::endl;
    }
}
   

void PiorityQueue::heapifyUp(Process** arr, int size, int index) { // mistro fun 
    int parentIndex = (index - 1) / 2;

    if (index > 0 && arr[index]->getCPUtime() < arr[parentIndex]->getCPUtime()) {
        swap(*arr[index], *arr[parentIndex]);
        heapifyUp(arr, size, parentIndex);
    }
}

void PiorityQueue::heapifyDown(Process** arr, int size, int index) {   // mistro fun
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;
    int minIndex = index;

    if (leftChildIndex < size && arr[leftChildIndex]->getCPUtime() < arr[minIndex]->getCPUtime()) {
        minIndex = leftChildIndex;
    }

    if (rightChildIndex < size && arr[rightChildIndex]->getCPUtime() < arr[minIndex]->getCPUtime()) {
        minIndex = rightChildIndex;
    }

    if (minIndex != index) {
        swap(*arr[index], *arr[minIndex]);
        heapifyDown(arr, size, minIndex);
    }
}


