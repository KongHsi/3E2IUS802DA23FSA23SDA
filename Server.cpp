#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <mutex>

#include "Constants.h"
#include <cpen333/process/socket.h>
#include <cpen333/process/mutex.h>

void service(cpen333::process::socket client) {
	std::cout << "Client " << 0 << " connected" << std::endl;
	int id;
	do {
		client.read(&id, sizeof(int));
		std::cout << id;
		if (id == CLIENT_VIEW_SERVER)
			std::cout << "request view products" << std::endl;
		else if (id == CLIENT_RESERVE_SERVER)
			std::cout << "request reserve an order" << std::endl;
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
	while (server.accept(client)) {
		std::thread thread(service, std::move(client));
		thread.detach();
	}

	// close server
	server.close();
	return 0;
}