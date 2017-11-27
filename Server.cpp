
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

void service(int client_id, cpen333::process::socket client, Database& database) {
	std::cout << "Client " << client_id << " connected" << std::endl;
	int id = 0;
	do {
		client.read(&id, sizeof(int));
		if (id == CLIENT_VIEW_SERVER) {
			std::cout << "request view products" << std::endl;
			std::string str = database.databaseToString();
			int size = str.size();
			client.write(&size, sizeof(size));
			const char *cstr = str.c_str();
			client.write(cstr, size);
		}
		else if (id == CLIENT_RESERVE_SERVER) {
			std::cout << "request reserve an order" << std::endl;
			int UOsize;
			client.read(&UOsize, sizeof(UOsize));
			char cstr[50];
			client.read(cstr, UOsize);
			int orderCount;
			client.read(&orderCount, sizeof(orderCount));
			for (int i = 0; i < UOsize; i++) 
				std::cout << cstr[i];
			std::cout << " " << orderCount << std::endl;
			int success = 1;
			client.write(&success, sizeof(success));
		}
		else if (id == CLIENT_ORDER_SERVER)
			std::cout << "request make an order" << std::endl;
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

