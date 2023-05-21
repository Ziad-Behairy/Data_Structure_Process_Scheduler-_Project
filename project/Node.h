#pragma once
#include"Process.h"

class Node {
public:
    Process* data;
    Node* next;

    Node(Process* value);
};


