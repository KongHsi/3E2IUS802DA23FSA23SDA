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
	// status = 0 created
	// status = 1 in process
	// status = 2 completed
	int status;
	int warehouse;
	Order(int id, int userID, int warehouse) : id(id), userID(userID), warehouse(warehouse) { status = 0; }
	Order(char* in, int status = 0) {
		int index = 0;
		char id_arr[2];
		while (*(in) != '\n') id_arr[index++] = *in++;
		in++;
		id = atoi(id_arr);
		index = 0;
		char userID_arr[2];
		while (*(in) != '\n') userID_arr[index++] = *in++;
		in++;
		userID = atoi(userID_arr);
		index = 0;
		char count_arr[3];
		while (*(in) != '\n') count_arr[index++] = *in++;
		in++;
		int size = atoi(count_arr);
		for (int i = 0; i < size; i++) {
			index = 0;
			char item_arr[15];
			char count_arr[4];
			while (*(in) != '\n') item_arr[index++] = *in++;
			item_arr[index] = '\0';
			in++;
			index = 0;
			while (*(in) != '\n') count_arr[index++] = *in++;
			count_arr[index] = '\0';
			orders[std::string(item_arr)] = atoi(count_arr);
			in++;
		}
		//status = status;
	}
	
	void printOrder() {
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << "Order ID: " << id << std::endl;
		std::cout << "User ID: " << userID << std::endl;
		for (std::map<std::string, int>::iterator iterator = orders.begin(); iterator != orders.end(); iterator++) {
			std::cout << "Item: " << iterator->first << " Quantity: " << iterator->second << std::endl;
		}
		std::cout << "--------------------------------------------------" << std::endl;
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
