#pragma once
#include "functions.hpp"

struct LL{
    int value;
    struct LL* next;

    LL(int value);
    ~LL();

    void add(int nextVal);
};

struct LLResource{
    int num; // store resoruce Num in resources, pcb Num in waitlist
    int amount;
    struct LLResource* next;

    LLResource(int num, int amount);
    ~LLResource();

    void add(struct LLResource* next);
};
struct Resource{
    int avaliable;
    int maxAvaliability;
    struct LLResource* waitlist;
    
    Resource(int maxAvaliability);
    ~Resource();

    void request(int num);
    void restore(int num);
    void addToWaitlist(int pcbNum, int amountRequested);
    void removeFromWaitlist(struct LLResource* resource);
    bool removeFromWaitlist(int pcbNum);
};

// ****** User LL instead ******
// struct ReadyList{

// };

struct ProcessControlBlock{
    int priority;
    int parent;
    struct LL* childs;
    struct LLResource* resources;

    ProcessControlBlock(int parent, int priority);
    ~ProcessControlBlock();

    struct ProcessControlBlock* createChild(int childNum, int priority);
    // void requestResources(int resourceNum, int amount, int pcbNum);
    void appendResource(int resourceNum, int amount);
    void deleteResource(int resourceNum); // remove resource completely
};


