#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <map>
#include "Constants.h"
#include "Inventory.h"
#include "Location.h"

class WarehouseDatabase {
	public:
		int cols;
		int rows;
		int levels;
		std::map<std::string, Inventory*> warehouseDatabase;
		bool locations[20][20][10]; //current max cols*rows*levels is 20*20*10

		WarehouseDatabase(int warehouse_id) {
			std::cout << "Initializing warehouse database.\n" << std::endl;
			std::string layout_initialization_file;
			switch (warehouse_id) {
			case 1:
				layout_initialization_file = WAREHOUSE_INITIALIZATION_FILEA;
				break;
			default:
				layout_initialization_file = WAREHOUSE_INITIALIZATION_FILEB;
			}
			std::ifstream fin(layout_initialization_file);
			if (fin.is_open()) {
				char col[4];
				char row[4];
				char level[4];
				fin.getline(row, 4);
				fin.getline(col, 4);
				fin.getline(level, 4);
				cols = atoi(col);
				rows = atoi(row);
				levels = atoi(level);
				while (!fin.eof()) {
					char buffer[30];
					char number[10];
					fin.getline(buffer, 30);
					fin.getline(number, 10);
					Inventory* inventory = new Inventory(std::string(buffer), atoi(number));
					for (int i = 0; i < atoi(number); i++) {
						bool findPos = false;
						do {
							int tempRow = rand() % rows;
							int tempCol = rand() % cols;
							int tempLevel = rand() % levels;
							if (!locations[tempRow][tempCol][tempLevel]) {
								locations[tempRow][tempCol][tempLevel] = true;
								Location* loc = new Location(tempRow, tempCol, tempLevel);
								inventory->locations.push_back(loc);
								findPos = true;
							}
						} while (!findPos);
					}
					warehouseDatabase[std::string(buffer)] = inventory;
				}
			}
			printDatabase();
		}

		void printDatabase() {
			std::cout << "--------------------------------------------------" << std::endl;;
			std::cout << "Hi, Welcome to the warehouse." << std::endl;
			std::cout << "We have " << rows << " rows, " << cols << " cols, and " << levels << " levels for shelves." << std::endl;
			for (std::map<std::string, Inventory*>::iterator iterator = warehouseDatabase.begin(); iterator != warehouseDatabase.end(); iterator++) {
				Inventory* inventory = iterator->second;
				std::cout << "Item name: " << inventory->name << ", "
					<< "Number:  " << inventory->count << std::endl;
				std::cout << "It is located at: " << std::endl;
				for (int i = 0; i < inventory->locations.size(); i++) {
					std::cout << "\tRow: " << inventory->locations.at(i)->x << " Col: " << inventory->locations.at(i)->y << " Level: " << inventory->locations.at(i)->shelf << std::endl;
				}
			}

			std::cout << "--------------------------------------------------" << std::endl;;
		}
};