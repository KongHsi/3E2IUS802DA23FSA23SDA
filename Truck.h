#ifndef TRUCK_H
#define TRUCK_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/semaphore.h>
#include <cpen333/process/condition_variable.h>
#include <iostream>
#include "WarehouseDatabase.h"
#include "DynamicQueue.h"
#include "safe_printf.h"
#include "map_common.h"
#include "Constants.h"
#include "Order.h"


class Truck : public cpen333::thread::thread_object {

public:
	int id;
	int capacity;
	int warehouseID;
	std::vector<Order*> orders;
	WarehouseDatabase& warehouseDatabase;
	std::map<int, Truck*>& deck_truck_map;
	int deck_id;
	Truck(int id, WarehouseDatabase& warehouseDatabase, std::map<int, Truck*>& deck_truck_map, int warehouseID) :
		id(id), warehouseDatabase(warehouseDatabase), deck_truck_map(deck_truck_map), warehouseID(warehouseID){}

	void restockTasks() {
		std::string truck_initialization_file = TRUCK_INITIALIZATION_FILEA;
		std::ifstream fin(truck_initialization_file);

		if (fin.is_open()) {
			char cap[10];
			fin.getline(cap, 10);
			capacity = atoi(cap);
			while (!fin.eof()) {
				char buffer[30];
				char number[10];
				fin.getline(buffer, 30);
				fin.getline(number, 10);
				std::string inventory = std::string(buffer);
				int count = atoi(number);
				Inventory* inv;
				if (warehouseDatabase.warehouseDatabase.find(inventory) == warehouseDatabase.warehouseDatabase.end()) {
					inv = new Inventory(inventory, count);
					warehouseDatabase.warehouseDatabase[inventory] = inv;
				}
				else {
					inv = warehouseDatabase.warehouseDatabase[inventory];
					inv->count += count;
				}
				for (int i = 0; i < count; i++) {
					Location* loc = warehouseDatabase.findLoc();
					inv->locations.push_back(loc);
					Task* task = new Task(*loc, deck_id, 1);
					warehouseDatabase.taskQueue->add(task);
				}
			}
		}
	}

	int main() {
		std::string deck_semaphore_name;
		int deck_semaphore_size;
		std::string deck_mutex_name;
		std::string deck_cv_name;
		if (warehouseID == 1) {
			deck_semaphore_name = DECK_SEMAPHORE1;
			deck_semaphore_size = DECK_SEMAPHORE_SIZE1;
			deck_mutex_name = DECK_MUTEX1;
			deck_cv_name = DECK_CV1;
		}
		else if (warehouseID == 2) {
			deck_semaphore_name = DECK_SEMAPHORE2;
			deck_semaphore_size = DECK_SEMAPHORE_SIZE2;
			deck_mutex_name = DECK_MUTEX2;
			deck_cv_name = DECK_CV2;
		}
		else {
			deck_semaphore_name = DECK_SEMAPHORE3;
			deck_semaphore_size = DECK_SEMAPHORE_SIZE3;
			deck_mutex_name = DECK_MUTEX3;
			deck_cv_name = DECK_CV3;
		}


		cpen333::process::semaphore semaphore_deck(deck_semaphore_name, deck_semaphore_size);
		cpen333::process::mutex mutex(WAREHOUSE_PRINT_MUTEX);
		cpen333::process::mutex deck_mutex(deck_mutex_name);
		cpen333::process::condition_variable deck_cv(deck_cv_name);
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Truck %d started\n", id);
			safe_printf("Truck %d waiting for available deck\n", id);
		}
		semaphore_deck.wait();
		deck_id = warehouseDatabase.decks_available.back();
		warehouseDatabase.decks_available.pop_back();
		deck_truck_map[deck_id] = this;
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Truck %d locates at deck %d, and currently unloading\n", id, deck_id);
		}
		restockTasks();
		warehouseDatabase.currentLoadingDeck.push_back(deck_id);
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Truck %d is currently loading\n", id);
		}
		while (warehouseDatabase.requestDeckToleave != deck_id) {
			{
				std::unique_lock<decltype(mutex)> lock(deck_mutex);
				deck_cv.wait(lock);
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));//try to avoid a race condition
		warehouseDatabase.requestDeckToleave = -1;
		warehouseDatabase.decks_available.push_back(deck_id);
		deck_truck_map.erase(deck_id);
		warehouseDatabase.currentLoadingDeck.erase(warehouseDatabase.currentLoadingDeck.begin() + deck_id);
		semaphore_deck.notify();
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Truck %d is left\n", id);
		}
		return 0;
	}
};

#endif 