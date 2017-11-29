
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include "Database.h"
#include "Constants.h"
#include "Order.h"
#include "WarehouseDatabase.h"
#include <cpen333/process/socket.h>
#include <cpen333/process/mutex.h>

void service_warehouse(int id_warehouse, cpen333::process::socket client_warehouse) {
	std::cout << "Client " << id_warehouse << " connected" << std::endl;

	int size;
	client_warehouse.read(&size, sizeof(size));
	char cstr[200];
	client_warehouse.read(cstr, size);
	cstr[size] = '\0';
	int received = 1;
	client_warehouse.write(&received, sizeof(received));
	Order order(cstr);
	order.printOrder();
}

void main_thread(int warehouseID) {
	std::this_thread::sleep_for(std::chrono::seconds(1)); // let connection starts first
	WarehouseDatabase warehouseDatabase(warehouseID);
}

int main() {
	int warehouseID;
	std::cout << "--------------AmaZOOM warehouse log in--------------" << std::endl;
	std::cout << "Please input warehouse ID: " << std::endl;
	std::cin >> warehouseID;

	//initialize main thread
	std::thread thread(main_thread, warehouseID);
	thread.detach();

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
	while (server_warehouse.accept(client_warehouse)) {
		std::thread thread(service_warehouse, warehouseID, std::move(client_warehouse));
		thread.detach();
	}
	// close server
	server_warehouse.close();
	return 0;
}

