#include <iostream>
#include <cpen333/process/socket.h>
#include <cpen333/process/socket.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cpen333/process/semaphore.h>
#include "Database.h"
#include "Constants.h"
#include "Order.h"
#include "WarehouseDatabase.h"
#include "Robot.h"
#include "DynamicQueue.h"
#include "safe_printf.h"
#include "map_common.h"
#include "Truck.h"


void check(int a, int b) {
	if(a == b)
		std::cout << "The result is correct." << std::endl;
	else
		std::cout << "The result is incorrect." << std::endl;
}

void testDatabase(Database db) {
	//test constructor
	//test create order
	db.createOrder(1, "FISH", 2, 1);
	check(db.orders[1]->orders["FISH"], 2);
	check(db.orders[1]->userID, 1);


}
void testDynamicQueue() {}
void testInventory() {}
void testItem() {}


int main() {
	//put all initialization here
	Database db;
	system("cls"); //clear screen

	//run test cases
	testDatabase(db);
	testDynamicQueue();
	testInventory();
	testItem();

	cpen333::pause();
	return 0;
}