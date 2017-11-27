#include <string>
#include <iostream>
#include <map>

class Item {
public:
	std::string name;
	int number;
	int warehouse; //warehouse ID that owns it
	std::map<int, int> reserved;
	Item(std::string name, int numebr, int warehouse) : 
		name(name), number(number), warehouse(warehouse) {}
};

