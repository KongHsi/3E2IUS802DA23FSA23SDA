#ifndef TRUCK_H
#define TRUCK_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>
#include "DynamicQueue.h"
#include "safe_printf.h"
#include "map_common.h"
#include <cpen333/process/mutex.h>
#include "Constants.h"
#include "Order.h"

class Truck : public cpen333::thread::thread_object {

public:
	int id;
	std::vector<Order*> orders;
	Truck(int id) :
		id(id) {}


	int main() {
		cpen333::process::mutex mutex(WAREHOUSE_PRINT_MUTEX);
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Truck %d started\n", id);
		}


		while (true) {

		}
		return 0;
	}
};

#endif 