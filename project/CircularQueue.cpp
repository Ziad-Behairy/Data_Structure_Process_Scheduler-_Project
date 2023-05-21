#include "CircularQueue.h"
CircularQueue::CircularQueue() {
    data = new Process * [INITIAL_SIZE];
    front_ = -1;
    rear_ = -1;
    maxSize = INITIAL_SIZE;
    currentSize = 0;
}

CircularQueue::~CircularQueue() {
    delete[] data;
}

bool CircularQueue::isEmpty() const {
    return currentSize == 0;
}

bool CircularQueue::isFull() const {
    return currentSize == maxSize;
}

int CircularQueue::size() const {
    return currentSize;
}

void CircularQueue::enqueue(Process* process) {
    if (isFull()) {
        resize();
    }
    rear_ = (rear_ + 1) % maxSize;
    data[rear_] = process;
    currentSize++;
    if (front_ == -1) {
        front_ = rear_;
    }
}

Process* CircularQueue::getItem(int index)
{
    if (index < 0 || index >= currentSize)
        return nullptr;

    int position = (front_ + index) % maxSize;
    return data[position];
}

void CircularQueue::dequeue() {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    front_ = (front_ + 1) % maxSize;
    currentSize--;
    if (currentSize == 0) {
        front_ = -1;
        rear_ = -1;
    }
}

Process* CircularQueue::front() const {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    else {
        return data[front_];
    }
}

Process* CircularQueue::back() const {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    else {
        return data[rear_];
    }
}

void CircularQueue::print() {
    if (isEmpty()) {
        cout << "Queue is empty." << endl;
    }
    else {
        cout << "CircularQueue contents:" << endl;
        int i = front_;
        do {
            cout << "Process ID: " << data[i]->getPID() << endl;
            i = (i + 1) % maxSize;
        } while (i != (rear_ + 1) % maxSize);
    }
}

Process* CircularQueue::Remove()
{
    if (isEmpty()) {
       return nullptr;
    }

    Process* removedProcess = data[front_];
    front_ = (front_ + 1) % maxSize;
    currentSize--;

    if (currentSize == 0) {
        front_ = -1;
        rear_ = -1;
    }

    return removedProcess;
}

void CircularQueue::resize() {
    int newMaxSize = maxSize * 2;
    Process** newData = new Process * [newMaxSize];

    int i = 0;
    while (!isEmpty()) {
        newData[i] = front();
        dequeue();
        i++;
    }

    delete[] data;

    data = newData;
    maxSize = newMaxSize;
    currentSize = i;
    front_ = 0;
    rear_ = i - 1;
}
