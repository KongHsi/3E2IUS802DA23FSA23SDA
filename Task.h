#ifndef TASK_H
#define TASK_H
#include <iostream>
#include "Location.h"

class Task {
public:
	Location loc;
	int dest;
	int type; // 0, shelf to deck, 1 deck to shelf
	Task(){}
	Task(Location loc, int dest, int type = 0) :
		loc(loc), dest(dest), type(type) {}
};

#endif