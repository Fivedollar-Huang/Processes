#pragma once

#include <queue>

#include "structures.hpp"

// Edit resource amount in init() funciton if RCBTOTAL change;
const int RCBTOTAL = 4; 
const int PCBTOTAL = 16;
const int RLTOTAL = 3;

void init();

void release();

void create(int priorityLevel);

void destroy(int childNum);

void request(int resourceNum, int amount);

void release(int resourceNum, int amount);

void timeout();

void scheduler();


void addToReadyList(int pcbNum, int priorityLevel);

std::string removeFromReadyList(int pcbNum);

void printReadyList();

void printResourceWaitList();
