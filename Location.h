#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>

class Location {

public:
	int x;
	int y;
	int shelf; //shelf level
	Location(){}
	Location(int x, int y, int shelf) :
		x(x), y(y), shelf(shelf) {}
};

#endif