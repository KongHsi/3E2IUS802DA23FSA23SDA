#ifndef ROBOT_H
#define ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>
#include "DynamicQueue.h"
#include "safe_printf.h"

class Robot : public cpen333::thread::thread_object {

public:
	int id;
	DynamicQueue* tasks;
	Robot(int id, DynamicQueue* tasks) :
		id(id), tasks(tasks) {}

	int main() {
		safe_printf("Robot %d started\n", id);
		while (true) {
			Task* task = tasks->get();
			//DO SOMETHING
			safe_printf("Robot %d: I am heading to row: %d, col: %d, shelf: %d\n",id,task->loc.x, task->loc.y, task->loc.shelf);
			std::this_thread::sleep_for(std::chrono::seconds(2));
			safe_printf("Robot %d: I am heading to loading deck: %d\n", id, task->dest);
		}
	return 0;
	}
};

#endif 
