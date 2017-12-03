#include "map_common.h"
#include "Constants.h"
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/console.h>
#include <cstdio>
#include <thread>
#include <chrono>

class MapUI {
	// display offset for better visibility
	static const int XOFF = 2;
	static const int YOFF = 1;
	cpen333::console display_;
	cpen333::process::shared_object<SharedData> memory_;
	cpen333::process::mutex mutex_;
	// previous positions of runners
	int lastpos_[MAX_ROBOTS][2];
	int exit_[2];   // exit location

public:
	MapUI(std::string map_memory, std::string map_mutex) : display_(), memory_(map_memory), mutex_(map_mutex) {
		// clear display and hide cursor
		display_.clear_all();
		display_.set_cursor_visible(false);

		// initialize last known runner positions
		for (size_t i = 0; i < MAX_ROBOTS; ++i) {
			lastpos_[i][COL_IDX] = -1;
			lastpos_[i][ROW_IDX] = -1;
		}

		for (size_t i = 0; i < MAX_ROBOTS; ++i) {
			lastpos_[i][COL_IDX] = -1;
			lastpos_[i][ROW_IDX] = -1;
		}

		// initialize exit location
		exit_[COL_IDX] = -1;
		exit_[ROW_IDX] = -1;
	}

	void draw_map() {
		MapInfo& minfo = memory_->minfo;
		RobotInfo& rinfo = memory_->rinfo;
		// clear display
		display_.clear_display();
		for (int r = 0; r < minfo.rows; ++r) {
			display_.set_cursor_position(YOFF + r, XOFF);
			for (int c = 0; c < minfo.cols; ++c) {
				char ch = minfo.map[c][r];
				if (ch == WALL_CHAR) {
					std::printf("%c", WALL_CHAR);
				}
				else if (ch == SHELF_CHAR) {
					std::printf("%c", SHELF_CHAR);
				}
				else if (ch == LOAD_CHAR) {
					std::printf("%c", LOAD_CHAR);
				}
				else if (ch == START_CHAR) {
					std::printf("%c", START_CHAR);
				}
				else {
					std::printf("%c", EMPTY_CHAR);
				}
			}
		}
		fflush(stdout);  // force output buffer to flush 
	}

	void draw_robots() {
		RobotInfo& rinfo = memory_->rinfo;
		// draw all runner locations
		mutex_.lock();
		for (size_t i = 0; i<rinfo.nrobots; ++i) {
			char me = 'R';//'A' + i;
			int newr = rinfo.rloc[i][ROW_IDX];
			int newc = rinfo.rloc[i][COL_IDX];
			// if not already at the exit...
			if (rinfo.rInMap[i]) {
				if (newc != lastpos_[i][COL_IDX]
					|| newr != lastpos_[i][ROW_IDX]) {
					// zero out last spot and update known location
					display_.set_cursor_position(YOFF + lastpos_[i][ROW_IDX], XOFF + lastpos_[i][COL_IDX]);
					std::printf("%c", EMPTY_CHAR);
					lastpos_[i][COL_IDX] = newc;
					lastpos_[i][ROW_IDX] = newr;
				}
				display_.set_cursor_position(YOFF + newr, XOFF + newc);
				std::printf("%c", me);
			}
			else {
				// erase old position if now finished
				if (lastpos_[i][COL_IDX] != exit_[COL_IDX] || lastpos_[i][ROW_IDX] != exit_[ROW_IDX]) {
					display_.set_cursor_position(YOFF + lastpos_[i][ROW_IDX], XOFF + lastpos_[i][COL_IDX]);
					std::printf("%c", EMPTY_CHAR);
					lastpos_[i][COL_IDX] = newc;
					lastpos_[i][ROW_IDX] = newr;
					// display a completion message
					display_.set_cursor_position(YOFF, XOFF + memory_->minfo.cols + 2);
					//std::printf("robot %c completed!!", me);
				}
			}
		}
		fflush(stdout);  // force output buffer to flush
		mutex_.unlock();
	}



};

int main() {
	std::cout << "Please let me know the warehouse ID: " << std::endl;
	int warehouseid;
	std::cin >> warehouseid;
	std::string memory_id;
	std::string mutex_id;
	switch (warehouseid) {
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
	cpen333::process::shared_object<SharedData> memory(memory_id);
	if (memory->magic != 11) {
		std::cout << "error";
		return 0;
	}
	//initialize previous locations of characters
	MapUI ui(memory_id, mutex_id);
	ui.draw_map();
	// continue looping until main program has quit
	while (!memory->quit) {
		ui.draw_robots();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	return 0;
}
