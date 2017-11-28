#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include <map>

class Order {
public:
	int id;
	int userID;
	std::map<std::string, int> orders;
	Order(int id, int userID) : id(id), userID(userID) {}

	void printOrder() {
		for (std::map<std::string, int>::iterator iterator = orders.begin(); iterator != orders.end(); iterator++) {
			std::cout << "Item: " << iterator->first << " Quantity: " << iterator->second << std::endl;
			std::cout << "--------------------------------------------------" << std::endl;;
		}
	}

};

#endif
