#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "Constants.h"
#include "Item.h"

class Database {
	public:
		std::map<std::string, Item*> database;

		Database(void) {
			std::cout << "Initializing database.\n" << std::endl;
			std::ifstream fin(ITEMS_INITIALIZATION_FILE);
			char buffer[30];
			char number[5];
			char warehouseNumber[5];
			if (fin.is_open()) {
				while (!fin.eof()) {
					fin.getline(buffer, 30);
					fin.getline(number, 5);
					fin.getline(warehouseNumber, 5);
					std::string str(buffer);
					Item* item = new Item(str, atoi(number), atoi(warehouseNumber));
					std::cout << "Item name: " << item->name << " , " 
						<< "Number:  "<< item->number << " , "
						<< "Warehouse: " <<item->warehouse << std::endl;
					database[str] = item;
				}
			}
		}
};
