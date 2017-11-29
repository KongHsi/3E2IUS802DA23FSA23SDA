#include "map_common.h"
#include "Constants.h"
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/console.h>
#include <cstdio>
#include <thread>
#include <chrono>

class MazeUI {
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
	MazeUI() : display_(), memory_(MAP_MEMORY_NAME), mutex_(MAP_MUTEX_NAME) {
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

	void draw_maze() {

		MapInfo& minfo = memory_->minfo;
		RobotInfo& rinfo = memory_->rinfo;

		// clear display
		display_.clear_display();
		// draw maze
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
};

int main() {

	cpen333::process::shared_object<SharedData> memory(MAP_MEMORY_NAME);
	
	if (memory->magic != 11) {
		std::cout << "error";
		return 0;
	}

	//initialize previous locations of characters
	MazeUI ui;
	ui.draw_maze();
	// continue looping until main program has quit
	while (!memory->quit) {
		//ui.draw_runners();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
