#ifndef ROBOT_H
#define ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/process/mutex.h>
#include <iostream>
#include "DynamicQueue.h"
#include "safe_printf.h"
#include "map_common.h"
#include "Constants.h"
#include "WarehouseDatabase.h"

class MapRobot {

	cpen333::process::mutex mutex_;
	MapInfo minfo_;
public:
	cpen333::process::shared_object<SharedData> memory_;
	int locC;
	int locR;

	MapRobot(std::string map_memory, std::string map_mutex) : memory_(map_memory), mutex_(map_mutex),
		minfo_(){
		while (memory_->magic != 11);
		// copy map contents
		minfo_ = memory_->minfo;
		{
			// protect access of number of runners
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			memory_->rinfo.nrobots++;
		}
	}

	int x_dir[4] = { 0, 0, 1, -1 };
	int y_dir[4] = { 1, -1, 0, 0 };

	int go_deliver(MapInfo& minfo, int pre_c, int pre_r, int id, int shelfX, int shelfY, int deck) {

		if (memory_->quit)
			return -1;
		{
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			memory_->rinfo.rInMap[id] = true;
			memory_->rinfo.rloc[id][ROW_IDX] = 1;
			memory_->rinfo.rloc[id][COL_IDX] = 1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		for (int i = 0; i < 2 * shelfY + 4; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][COL_IDX]++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < 1 + shelfX; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][ROW_IDX]++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		safe_printf("Robot %d deliver: I am heading to loading deck: %d\n", id, deck);
		for (int i = 0; i < minfo.rows - 4 - shelfX; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][ROW_IDX]++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < 2 * shelfY + 4; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][COL_IDX]--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		safe_printf("Robot %d deliver: I am heading back\n", id);
		for (int i = 0; i < minfo.rows - 3; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][ROW_IDX]--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		{
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			memory_->rinfo.rInMap[id] = false;
			memory_->rinfo.rloc[id][ROW_IDX] = -1;
			memory_->rinfo.rloc[id][COL_IDX] = -1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return 0;
	}

	int go_restock(MapInfo& minfo, int pre_c, int pre_r, int id, int shelfX, int shelfY, int deck, int shelf) {
		if (memory_->quit)
			return -1;
		{
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			memory_->rinfo.rInMap[id] = true;
			memory_->rinfo.rloc[id][ROW_IDX] = 1;
			memory_->rinfo.rloc[id][COL_IDX] = 1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		for (int i = 0; i < minfo.rows - 3; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][ROW_IDX]++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < deck; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][COL_IDX]++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		safe_printf("Robot %d restock: I am heading to row: %d, col: %d, shelf: %d\n", id, shelfX, shelfY, shelf);
		for (int i = 0; i < 2 * shelfY - deck + 4; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][COL_IDX]++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < minfo.rows - 4 - shelfX; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][ROW_IDX]--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		safe_printf("Robot %d restock: I am heading back\n", id);
		for (int i = 0; i < 1 + shelfX; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][ROW_IDX]--; 
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < 2 * shelfY + 4; i++) {
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				memory_->rinfo.rloc[id][COL_IDX]--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		{
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			memory_->rinfo.rInMap[id] = false;
			memory_->rinfo.rloc[id][ROW_IDX] = -1;
			memory_->rinfo.rloc[id][COL_IDX] = -1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return 0;
	}
};

class Robot : public cpen333::thread::thread_object {

public:
	int id;
	int warehouse;
	DynamicQueue* tasks;
	bool shouldEnd;
	WarehouseDatabase& warehouseDatabase;
	Robot(int id, WarehouseDatabase& warehouseDatabase, int warehouse) :
		id(id), warehouseDatabase(warehouseDatabase), warehouse(warehouse) {
		tasks = warehouseDatabase.taskQueue;
		shouldEnd = false;
	}


	int main() {
		cpen333::process::mutex mutex(WAREHOUSE_PRINT_MUTEX);
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Robot %d started\n", id);
		}

		std::string memory_id;
		std::string mutex_id;
		switch (warehouse) {
		case 1:
			memory_id = MAP_MEMORY_NAME_1;
			mutex_id = MAP_MUTEX_NAME_1;
			break;
		case 2:
			memory_id = MAP_MEMORY_NAME_2;
			mutex_id = MAP_MUTEX_NAME_2;
			break;
		case 3:
			memory_id = MAP_MEMORY_NAME_3;
			mutex_id = MAP_MUTEX_NAME_3;
			break;
		default:
			memory_id = MAP_MEMORY_NAME_1;
			mutex_id = MAP_MUTEX_NAME_1;
		}
		
		do {
			Task* task = tasks->get();
			if (shouldEnd) {
				tasks->add(task);
				break;
			}
			MapRobot runner(memory_id, mutex_id);
			MapInfo minfo = runner.memory_->minfo;
			RobotInfo rinfo = runner.memory_->rinfo;
			cpen333::process::mutex mutex_(mutex_id);
			{
				std::lock_guard<decltype(mutex_)> lock(mutex_);
				rinfo.rloc[id][COL_IDX] = -1;
				rinfo.rloc[id][ROW_IDX] = -1;
				rinfo.rInMap[id] = false;
			}
			if (task->type == 0) {
				safe_printf("Robot %d deliver: I am heading to row: %d, col: %d, shelf: %d\n", id, task->loc.x, task->loc.y, task->loc.shelf);
				runner.go_deliver(minfo, runner.locC, runner.locR, id, task->loc.x, task->loc.y, task->dest);
			}
			else {
				safe_printf("Robot %d restock: I am heading to deck: %d\n", id, task->dest);
				runner.go_restock(minfo, runner.locC, runner.locR, id, task->loc.x, task->loc.y, task->dest, task->loc.shelf);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		} while (!shouldEnd);

	{
		std::lock_guard<cpen333::process::mutex> lock(mutex);
		safe_printf("Robot %d ended\n", id);
	}
	return 0;
	}
};

#endif 
