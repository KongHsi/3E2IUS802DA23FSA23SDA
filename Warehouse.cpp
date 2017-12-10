
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <thread>
#include <memory>
#include <mutex>
#include "Database.h"
#include "Constants.h"
#include "Order.h"
#include "WarehouseDatabase.h"
#include <cpen333/process/socket.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include "Robot.h"
#include "DynamicQueue.h"
#include "safe_printf.h"
#include "map_common.h"



void service_warehouse(int id_warehouse, cpen333::process::socket client_warehouse, WarehouseDatabase& warehouseDatabase) {
	std::cout << "Web Server " << id_warehouse << " connected" << std::endl;

	int size;
	client_warehouse.read(&size, sizeof(size));
	char cstr[200];
	client_warehouse.read(cstr, size);
	cstr[size] = '\0';
	int received = 1;
	client_warehouse.write(&received, sizeof(received));
	
	Order order = warehouseDatabase.addOrder(cstr);
	order.printOrder();

	for (std::map<std::string, int>::iterator iterator = order.orders.begin(); iterator != order.orders.end(); iterator++) {
		warehouseDatabase.warehouseDatabase[iterator->first]->count -= iterator->second;
		for (int i = 0; i < iterator->second; i++) {
			Location* loc = warehouseDatabase.warehouseDatabase[iterator->first]->locations.back();
			warehouseDatabase.warehouseDatabase[iterator->first]->locations.pop_back();
			Task* task = new Task(*loc, 1); //TODO: delivery dock
			warehouseDatabase.taskQueue->add(task);
		}
	}
}

void print_menu() {
	std::cout << "=========================================" << std::endl;
	std::cout << "=                  MENU                 =" << std::endl;
	std::cout << "=========================================" << std::endl;
	std::cout << " (1) View products and stock availability" << std::endl;
	std::cout << " (2) View orders" << std::endl;
	std::cout << " (3) Add robot" << std::endl;
	std::cout << " (4) Remove robot" << std::endl;
	std::cout << " (5) View all robots" << std::endl;
	std::cout << " (0) Quit" << std::endl;
	std::cout << "=========================================" << std::endl;
	std::cout << "Enter number: ";
	std::cout.flush();
}

// shared between manager thread and main_thread
std::vector<Robot*> robots;
std::set<int> robot_ids;
void manager_thread(int warehouseID, WarehouseDatabase& warehouseDatabase) {
	cpen333::process::mutex mutex(WAREHOUSE_PRINT_MUTEX);
	char cmd = 0;
	while (cmd != MANAGERQUIT) {
		{
			std::lock_guard<cpen333::process::mutex> lock(mutex);
			print_menu();
		}
		std::cin >> cmd;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (cmd == MANAGER_VIEW_STOCK) {
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				warehouseDatabase.printDatabase();
			}
		}
		else if (cmd == MANAGER_VIEW_ORDER) {
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				warehouseDatabase.printOrders();
			}
		}
		else if (cmd == MANAGER_ADD_ROBOT) {
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "What's robot's ID?" << std::endl;
			}
			int r_id;
			std::cin >> r_id;
			if (r_id >= MAX_ROBOTS || r_id < 0 )
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "Robot id is not valid." << std::endl;
			}
			else if (robot_ids.find(r_id) != robot_ids.end()) 
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "Robot id is already used." << std::endl;
			}
			else {
				robot_ids.insert(r_id);
				robots.push_back(new Robot(r_id, warehouseDatabase.taskQueue, warehouseID));
				{
					std::lock_guard<cpen333::process::mutex> lock(mutex);
					std::cout << "Robot " << r_id << " is added successfully." << std::endl;
				}
			}
		}
		else if (cmd == MANAGER_REMOVE_ROBOT) {
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "What's robot's ID?" << std::endl;
			}
			int r_id;
			std::cin >> r_id;
			if(r_id >= robots.size() || r_id < 0)
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "Robot id is not valid." << std::endl;
			} 
			else if (robot_ids.find(r_id) == robot_ids.end())
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "There's no such a robot." << std::endl;
			}
			else {
				Robot* r = robots.at(r_id);
				robots.erase(robots.begin() + r_id);
				robot_ids.erase(r_id);
				r->shouldEnd = true;
				{
					std::lock_guard<cpen333::process::mutex> lock(mutex);
					std::cout << "Robot " << r_id << " is removed successfully." << std::endl;
				}
			}
		}
		else if (cmd == MANAGER_VIEW_ROBOTS) {
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << "Available robots: " << std::endl;
				std::set<int>::iterator it = robot_ids.begin();
				while (it != robot_ids.end()) {
					std::cout << "\tRobot id: " << *it++ << std::endl;
				}
				std::cout << "--------------------------------------------------" << std::endl;
			}
		}
		else {
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				safe_printf("Command is not vaild\n");
			}
		}
	}
}
void main_thread(int warehouseID, WarehouseDatabase& warehouseDatabase) {
	//create robots
	const int nrobots = NROBOTS;
	for (int i = 0; i < nrobots; i++) {
		robot_ids.insert(i);
		robots.push_back(new Robot(i, warehouseDatabase.taskQueue, warehouseID));
	}
	
	for (auto& robot : robots) {
		robot->start();
		robot->detach();
	}
	/*
	for (auto& robot : robots) {
		robot->join();
	}
	*/
}

void initializeMap(int warehouseID, MapInfo& minfo, RobotInfo& rinfo) {

	minfo.rows = 0;
	minfo.cols = 0;
	std::string map_file;
	switch (warehouseID) {
	case 1:
		map_file = MAP_INITIALIZATION_FILEA;
		break;
	case 2:
		map_file = MAP_INITIALIZATION_FILEB;
		break;
	case 3:
		map_file = MAP_INITIALIZATION_FILEC;
		break;
	default:
		map_file = MAP_INITIALIZATION_FILEA;
	}
	std::ifstream fin(map_file);
	std::string line;
	if (fin.is_open()) {
		int row = 0;
		while (std::getline(fin, line)) {
			int cols = line.length();
			if (cols > 0) {
				if (cols > minfo.cols) {
					minfo.cols = cols;
				}
				for (size_t col = 0; col<cols; ++col) {
					minfo.map[col][row] = line[col];
				}
				++row;
			}
		}
		minfo.rows = row;
		fin.close();
	}
	
	rinfo.nrobots = NROBOTS;
	for (int i = 0; i < rinfo.nrobots; i++) {
		rinfo.rloc[i][COL_IDX] = -1;
		rinfo.rloc[i][ROW_IDX] = -1;
		rinfo.rInMap[i] = false;
	}

}

int main() {
	int warehouseID;
	cpen333::process::mutex mutex(WAREHOUSE_PRINT_MUTEX);
	{
		std::lock_guard<cpen333::process::mutex> lock(mutex);
		std::cout << "--------------AmaZOOM warehouse log in--------------" << std::endl;
		std::cout << "Please input warehouse ID: " << std::endl;
		std::cin >> warehouseID;
	}

	WarehouseDatabase warehouseDatabase(warehouseID);
	//initialize map
	std::string map_memory_id;
	switch (warehouseID) {
	case 1:
		map_memory_id = MAP_MEMORY_NAME_1;
		break;
	case 2:
		map_memory_id = MAP_MEMORY_NAME_2;
		break;
	case 3:
		map_memory_id = MAP_MEMORY_NAME_3;
		break;
	default:
		map_memory_id = MAP_MEMORY_NAME_1;
	}

	cpen333::process::shared_object<SharedData> memory(map_memory_id);
	initializeMap(warehouseID, memory->minfo, memory->rinfo);
	memory->magic = 11;
	memory->quit = false;


	//initialize main thread
	std::thread thread_main(main_thread, warehouseID, std::ref(warehouseDatabase));
	thread_main.detach();

	std::thread thread_manager(manager_thread, warehouseID, std::ref(warehouseDatabase));
	thread_manager.detach();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	int warehouse_port_number = warehouseID + WAREHOUSE_PORT_NUMBER - 1;

	cpen333::process::socket_server server_warehouse(warehouse_port_number);
	server_warehouse.open();
	safe_printf( "Server started on port %d.\n" ,server_warehouse.port());
	cpen333::process::socket client_warehouse;
	while (server_warehouse.accept(client_warehouse)) {
		std::thread thread(service_warehouse, warehouseID, std::move(client_warehouse), std::ref(warehouseDatabase));
		thread.detach();
	}
	// close server
	server_warehouse.close();
	return 0;
}

