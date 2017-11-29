#ifndef MAP_COMMON_H
#define MAP_COMMON_H

#define MAP_MEMORY_NAME "mapr"
#define MAP_MUTEX_NAME "map_mutex"

#define WALL_CHAR 'W'
#define EMPTY_CHAR ' '
#define SHELF_CHAR 'S'
#define START_CHAR 'R'
#define LOAD_CHAR 'L'

#define COL_IDX 0
#define ROW_IDX 1

#define MAX_MAP_SIZE 50
#define MAX_ROBOTS  20

#define NROBOTS 6

struct MapInfo {
	int rows;           // rows in map
	int cols;           // columns in map
	char map[MAX_MAP_SIZE][MAX_MAP_SIZE];  // map storage
};

struct RobotInfo {
	int nrobots;      // number robots
	int rloc[MAX_ROBOTS][2];   // robot locations [col][row]
	int sloc[MAX_ROBOTS][2]; // robot go to shelf loc
	bool rInMap[MAX_ROBOTS];
};

struct SharedData {
	MapInfo minfo;    // map info
	RobotInfo rinfo;  // robot info
	bool quit;         // tell everyone to quit
	int magic;
};

#endif
