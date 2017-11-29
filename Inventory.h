#ifndef INVENTORY_H
#define INVENTORY_H


#include <string>
#include <iostream>
#include "Location.h"
#include <vector>

class Inventory {
public:
	std::string name;
	int count;
	std::vector<Location*> locations;

	Inventory(std::string name, int count) :
		name(name), count(count) {}
};

#endif