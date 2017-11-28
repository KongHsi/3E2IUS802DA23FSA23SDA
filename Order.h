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

	std::string orderToString() {
		std::string str;
		str.append(std::to_string(id));
		str.append("\n");
		str.append(std::to_string(userID));
		str.append("\n");
		str.append(std::to_string(orders.size()));
		str.append("\n");
		for (std::map<std::string, int>::iterator iterator = orders.begin(); iterator != orders.end(); iterator++) {
			str.append(iterator->first);
			str.append("\n");
			str.append(std::to_string(iterator->second));
			str.append("\n");
		}
		return str;
	}

};

#endif
