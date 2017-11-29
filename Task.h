#ifndef TASK_H
#define TASK_H
#include <iostream>
#include "Location.h"

class Task {
public:
	Location loc;
	int dest;
	Task(){}
	Task(Location loc, int dest) :
		loc(loc), dest(dest) {}
};

#endif