
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

void service_warehouse(int id_warehouse, cpen333::process::socket client_warehouse) {
	std::cout << "Client " << id_warehouse << " connected" << std::endl;

	int size;
	client_warehouse.read(&size, sizeof(size));
	char cstr[200];
	client_warehouse.read(cstr, size);
	cstr[size] = '\0';
	for (int i = 0; i < size; i++) {
		std::cout << cstr[i];
	}
	int received = 1;
	client_warehouse.write(&received, sizeof(received));
}

int main() {
	int warehouseID;
	std::cout << "--------------AmaZOOM warehouse log in--------------" << std::endl;
	std::cout << "Please input warehouse ID: " << std::endl;
	std::cin >> warehouseID;
	int warehouse_port_number;
	switch (warehouseID) {
		case 1:
			warehouse_port_number = PORT_NUMBER1;
			break;
		default:
			warehouse_port_number = PORT_NUMBER2;
	}
	cpen333::process::socket_server server_warehouse(warehouse_port_number);
	server_warehouse.open();
	std::cout << "Server started on port " << server_warehouse.port() << std::endl;
	cpen333::process::socket client_warehouse;
	int id_warehouse = 0;
	while (server_warehouse.accept(client_warehouse)) {
		std::thread thread(service_warehouse, id_warehouse++, std::move(client_warehouse));
		thread.detach();
	}
	// close server
	server_warehouse.close();
	return 0;
}

