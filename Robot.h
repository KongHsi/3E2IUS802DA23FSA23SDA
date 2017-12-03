#ifndef ROBOT_H
#define ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>
#include "DynamicQueue.h"
#include "safe_printf.h"
#include "map_common.h"
#include <cpen333/process/mutex.h>
#include "Constants.h"

class MapRobot {

	cpen333::process::mutex mutex_;

	// local copy of map
	MapInfo minfo_;
	// runner info
	size_t idx_;   // runner index
	int loc_[2];   // current location

public:
	cpen333::process::shared_object<SharedData> memory_;
	int locC;
	int locR;

	MapRobot() : memory_(MAP_MEMORY_NAME), mutex_(MAP_MUTEX_NAME),
		minfo_(), idx_(0), loc_() {
		while (memory_->magic != 11);
		// copy map contents
		minfo_ = memory_->minfo;
		{
			// protect access of number of runners
			std::lock_guard<decltype(mutex_)> lock(mutex_);
			idx_ = memory_->rinfo.nrobots;
			memory_->rinfo.nrobots++;
		}
		// get current location
		loc_[COL_IDX] = memory_->rinfo.rloc[idx_][COL_IDX];
		loc_[ROW_IDX] = memory_->rinfo.rloc[idx_][ROW_IDX];
		locC = loc_[COL_IDX];
		locR = loc_[ROW_IDX];
	}

	int x_dir[4] = { 0, 0, 1, -1 };
	int y_dir[4] = { 1, -1, 0, 0 };

	int go(MapInfo& minfo, int pre_c, int pre_r, int id, int shelfX, int shelfY, int deck) {

		if (memory_->quit)
			return -1;

		memory_->rinfo.rInMap[id] = true;
		memory_->rinfo.rloc[id][ROW_IDX] = 1;
		memory_->rinfo.rloc[id][COL_IDX] = 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		for (int i = 0; i < 2 * shelfY; i++) {
			memory_->rinfo.rloc[id][COL_IDX]++;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < 1 + shelfX; i++) {
			memory_->rinfo.rloc[id][ROW_IDX]++;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		for (int i = 0; i < minfo.rows - 4 - shelfX; i++) {
			memory_->rinfo.rloc[id][ROW_IDX]++;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < 2 * shelfY; i++) {
			memory_->rinfo.rloc[id][COL_IDX]--;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		for (int i = 0; i < minfo.rows - 3; i++) {
			memory_->rinfo.rloc[id][ROW_IDX]--;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		memory_->rinfo.rInMap[id] = false;
		/*
		// current location
		int c = memory_->rinfo.rloc[idx_][COL_IDX];
		int r = memory_->rinfo.rloc[idx_][ROW_IDX];

		if (memory_->minfo.map[c][r] == 'E')
			return 1;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//==========================================================
		// TODO: NAVIGATE map
		//==========================================================
		int temp_c = c;
		int temp_r = r;
		for (int i = 0; i < 4; i++) {
			c = c + x_dir[i];
			r = r + y_dir[i];
			if (c >= 0 && r >= 0 && c < memory_->minfo.cols && c < memory_->minfo.rows && memory_->minfo.map[c][r] != 'X' && minfo.map[c][r] != 'X' && !(c == pre_c && r == pre_r)) {
				memory_->rinfo.rloc[idx_][COL_IDX] = c;
				memory_->rinfo.rloc[idx_][ROW_IDX] = r;
				int ret = go(minfo, temp_c, temp_r);
				if (ret == 1)
					return 1;
				else if (ret == -1)
					return -1;
			}
			c = c - x_dir[i];
			r = r - y_dir[i];
			memory_->rinfo.rloc[idx_][COL_IDX] = c;
			memory_->rinfo.rloc[idx_][ROW_IDX] = r;
		}
		minfo.map[c][r] = 'X';
		*/
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return 0;
	}

};

class Robot : public cpen333::thread::thread_object {

public:
	int id;
	DynamicQueue* tasks;
	Robot(int id, DynamicQueue* tasks) :
		id(id), tasks(tasks) {}


	int main() {
		cpen333::process::mutex mutex(WAREHOUSE_PRINT_MUTEX);
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			safe_printf("Robot %d started\n", id);
		}

		
		while (true) {
			Task* task = tasks->get();
			safe_printf("Robot %d: I am heading to row: %d, col: %d, shelf: %d\n",id,task->loc.x, task->loc.y, task->loc.shelf);
			MapRobot runner;
			MapInfo minfo = runner.memory_->minfo;
			runner.go(minfo, runner.locC, runner.locR, id, task->loc.x, task->loc.y, task->dest);
			std::this_thread::sleep_for(std::chrono::seconds(2));
			safe_printf("Robot %d: I am heading to loading deck: %d\n", id, task->dest);
		}
	return 0;
	}
};

#endif 
