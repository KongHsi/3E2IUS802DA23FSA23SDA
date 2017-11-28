
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
#include "Order.h"

void service(int client_id, cpen333::process::socket client, Database& database) {
	cpen333::process::mutex mutex(DATABASE_MUTEX);
	std::cout << "Client " << client_id << " connected" << std::endl;
	int id = 0;
	do {
		client.read(&id, sizeof(int));
		if (id == CLIENT_VIEW_SERVER) {
			std::cout << "Request view products" << std::endl;
			std::string str = database.databaseToString();
			int size = str.size();
			client.write(&size, sizeof(size));
			const char *cstr = str.c_str();
			client.write(cstr, size);
		}
		else if (id == CLIENT_RESERVE_SERVER) {
			std::cout << "Request reserve an order" << std::endl;
			int UOsize;
			client.read(&UOsize, sizeof(UOsize));
			char cstr[50];
			client.read(cstr, UOsize);
			int orderCount;
			client.read(&orderCount, sizeof(orderCount));
			cstr[UOsize] = '\0';
			std::string str(cstr);
			std::cout << str;
			std::cout << " " << orderCount << std::endl;
			int success = 1;
			{
				std::lock_guard<cpen333::process::mutex> lock(mutex);
				if(database.database.find(str) == database.database.end()
					|| database.database[str]->number < orderCount)
					success = 0;
				else {
					database.database[str]->number -= orderCount;
					database.printDatabase();
					if (database.orders.find(client_id) == database.orders.end()) {
						database.createOrder(client_id, str, orderCount);
						std::cout << "--------------------------------------------------" << std::endl;;
						std::cout << "Created order: " << database.orders[client_id]->id
							<< " User id: " << database.orders[client_id]->userID << std::endl;
						database.orders[client_id]->printOrder();
					}
					else {
						database.updateOrder(client_id, str, orderCount);
						std::cout << "--------------------------------------------------" << std::endl;;
						std::cout << "Update order: " << database.orders[client_id]->id
							<< " User id: " << database.orders[client_id]->userID << std::endl;
						database.orders[client_id]->printOrder();
					}
				}
				
			}
			client.write(&success, sizeof(success));
		}
		else if (id == CLIENT_ORDER_SERVER) {
			std::cout << "Request make an order" << std::endl;
			if (database.orders.find(client_id) == database.orders.end()) {
				std::cout << "You have nothing in cart! Buy something first!" << std::endl;
				break;
			}

			cpen333::process::socket socket("localhost", PORT_NUMBER1);
			std::cout << "Client connecting...";
			std::cout.flush();
			if (socket.open()) {
				//std::this_thread::sleep_for(std::chrono::seconds(2));
				std::cout << "connected." << std::endl;
				std::string orderStr = database.orders[client_id]->orderToString();
				int size = orderStr.size();
				socket.write(&size, sizeof(size));
				const char *cstr = orderStr.c_str();
				socket.write(cstr, size);
				int success;
				socket.read(&success, sizeof(success));
				if (success == 1) {
					std::cout << "Order placed!" << std::endl;
					database.orders.erase(client_id);
				}
				else {
					std::cout << "Order not placed" << std::endl;
				}
				client.write(&success, sizeof(success));
			}
			
		}
	} while (id != CLIENT_QUIT_SERVER);
}



int main() {

	// start server
	cpen333::process::socket_server server(PORT_NUMBER);
	server.open();
	std::cout << "Server started on port " << server.port() << std::endl;
	cpen333::process::socket client;
	int id = 0;
	Database database;
	while (server.accept(client)) {
		std::thread thread(service, id++, std::move(client), std::ref(database));
		thread.detach();
	}
	// close server
	server.close();
	return 0;
}

