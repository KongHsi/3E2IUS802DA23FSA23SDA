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
			if (fin.is_open()) {
				while (!fin.eof()) {
					char buffer[30];
					char number[10];
					char warehouseNumber[10];
					fin.getline(buffer, 30);
					fin.getline(number, 10);
					fin.getline(warehouseNumber, 10);
					std::string str(buffer);
					Item* item = new Item(str, atoi(number), atoi(warehouseNumber));
					database[str] = item;
				}
			}
			printDatabase();
		}

		std::string databaseToString() {
			std::string str;
			for (std::map<std::string, Item*>::iterator it = database.begin(); it != database.end(); ++it) {
				str.append("\t");
				str.append(it->first);
				str.append("\t");
				str.append(std::to_string(it->second->number));
				str.append("\n");
			}
			return str;
		}

		void printDatabase() {
			for (std::map<std::string, Item*>::iterator iterator = database.begin(); iterator != database.end(); iterator++) {
				Item* item = iterator->second;
				std::cout << "Item name: " << item->name << ", "
					<< "Number:  " << item->number << ", "
					<< "Warehouse: " << item->warehouse << std::endl;
			}
		}
};
