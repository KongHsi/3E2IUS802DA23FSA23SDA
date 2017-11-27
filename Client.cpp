#include <cpen333/process/socket.h>
#include "Constants.h"
#include <iostream>
#include <limits>

void print_menu() {
	std::cout << "=========================================" << std::endl;
	std::cout << "=                  MENU                 =" << std::endl;
	std::cout << "=========================================" << std::endl;
	std::cout << " (1) View products" << std::endl;
	std::cout << " (2) Reserve order" << std::endl;
	std::cout << " (3) Make an order" << std::endl;
	std::cout << " (0) Quit" << std::endl;
	std::cout << "=========================================" << std::endl;
	std::cout << "Enter number: ";
	std::cout.flush();
}

int main() {
	// start client
	cpen333::process::socket socket("localhost", PORT_NUMBER);
	std::cout << "Client connecting...";
	std::cout.flush();
	if (socket.open()) {
		std::cout << "connected." << std::endl;
		// keep reading commands until the user quits
		char cmd = 0;
		while (cmd != CLIENT_QUIT) {
			print_menu();
			std::cin >> cmd;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			int request = 0;
			int resultSize = 0;
			char str[256];
			switch (cmd) {
			case CLIENT_VIEW:
				request = 1;
				socket.write(&request, sizeof(request));
				socket.read(&resultSize, sizeof(resultSize));
				socket.read(&str, resultSize);
				std::cout << "Welcome the biggeest duoshou website in Canada!" << std::endl;
				std::cout << "We have:" << std::endl;
				for (int i = 0; i<resultSize; ++i)
					std::cout << str[i];
				break;
			case CLIENT_RESERVE:
				request = 2;
				socket.write(&request, sizeof(request));
				break;
			case CLIENT_ORDER:
				request = 3;
				socket.write(&request, sizeof(request));
				break;
			case CLIENT_QUIT:
				request = 0;
				socket.write(&request, sizeof(request));
				break;
			default:
				std::cout << "Invalid command number " << cmd << std::endl << std::endl;
			}
			cpen333::pause();
		}
	}
	else {
		std::cout << "failed." << std::endl;
	}
	return 0;
}

