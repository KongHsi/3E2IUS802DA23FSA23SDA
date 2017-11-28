
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include "Database.h"
#include "Constants.h"
#include <cpen333/process/socket.h>
#include <cpen333/process/mutex.h>

void service_warehouse(int id_warehouse) {
	std::cout << "Client " << id_warehouse << " connected" << std::endl;
}

int main() {
	int warehouseID;
	std::cout << "--------------AmaZOOM warehouse log in--------------" << std::endl;
	std::cout << "Please input warehouse ID: " << std::endl;
	std::cin >> warehouseID;
	cpen333::process::socket_server server_warehouse(52103);
	server_warehouse.open();
	std::cout << "Server started on port " << server_warehouse.port() << std::endl;
	cpen333::process::socket client_warehouse;
	int id_warehouse = 0;
	while (server_warehouse.accept(client_warehouse)) {
		std::thread thread(service_warehouse, id_warehouse++);
		thread.detach();
	}
	// close server
	server_warehouse.close();
	return 0;
}

