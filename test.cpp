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
#include <string>
#include <fstream>



void check_testDatabase(int a, int b, int function, int test, int line) {
	if (a == b)
		std::cout << "The result is correct." << std::endl;
	else
		std::cout << "The result is incorrect." << " <Database.h>  function:" << function << "  test:" << test << "  line:" << line << std::endl;
}

void testDatabase(Database db) {

	//client_id, str, orderCount, warehouse
	//test 1  CLIENT1
	db.createOrder(1, "FISH", 2, 1);
	std::cout << std::endl << " 1. test create order" << std::endl;
	check_testDatabase(db.orders[1]->orders["FISH"], 2, 1, 1, 1);
	check_testDatabase(db.orders[1]->warehouse_IDs["FISH"], 1, 1, 1, 2);
	check_testDatabase(db.orders[1]->id, 0, 1, 1, 3); // FIND PROBLEM HERE  id start at 0
	check_testDatabase(db.orders[1]->userID, 1, 1, 1, 4);
	check_testDatabase(db.orders[1]->warehouse, 1, 1, 1, 5);

	//test 2 CLIENT3
	db.createOrder(3, "ICE", 5, 2);
	check_testDatabase(db.orders[3]->orders["ICE"], 5, 1, 2, 1);
	check_testDatabase(db.orders[3]->warehouse_IDs["ICE"], 2, 1, 2, 2);
	check_testDatabase(db.orders[3]->id, 1, 1, 2, 3);
	check_testDatabase(db.orders[3]->userID, 3, 1, 2, 4);
	check_testDatabase(db.orders[3]->warehouse, 2, 1, 2, 5);

	//2. test update order
	//client_id, str, orderCount,WAREHOUSE
	std::cout << std::endl << " 2. test update order" << std::endl;
	//test1:item already exist in the order
	db.updateOrder(1, "FISH", 1, 1);
	check_testDatabase(db.orders[1]->orders["FISH"], 3, 2, 1, 1);
	//test2:item does not exit in the order ,same warehouse
	db.updateOrder(1, "BEEF", 2, 1);
	check_testDatabase(db.orders[1]->orders["BEEF"], 2, 2, 2, 1);
	check_testDatabase(db.orders[1]->warehouse_IDs["BEEF"], 1, 2, 2, 2);
	//test3:item does not exit in the order ,different warehouse
	db.updateOrder(1, "BANANA", 2, 3);
	check_testDatabase(db.orders[1]->orders["BANANA"], 2, 2, 3, 1);
	check_testDatabase(db.orders[1]->warehouse_IDs["BANANA"], 3, 2, 3, 2);

	//3. database to String      
	std::string string_database = db.databaseToString();
	std::cout << std::endl << "3. check  database to String " << std::endl;
	std::cout << string_database << std::endl;

	//4.printDatabase
	std::cout << std::endl << "4. check  printDatabase " << std::endl;
	db.printDatabase();

	// 5.order to string (Order)
	std::string string_Order = db.orders[1]->orderToString();
	std::cout << std::endl << "5. check  order to String " << std::endl;
	std::cout << string_Order << std::endl;

	//6. printOrder(Order)
	std::cout << std::endl << "6. check  printOrder " << std::endl;
	db.orders[1]->printOrder();

	// 7.test count total(Order)
	std::cout << std::endl << " 7.test count total" << std::endl;
	check_testDatabase(db.orders[1]->countTotal(), 7, 9, 1, 1);
	check_testDatabase(db.orders[3]->countTotal(), 5, 9, 2, 1);
	std::cout << std::endl;

}


void testInventory(Inventory inventory) {
	std::cout << "Test create inventory: " << std::endl;
	if (inventory.name == "NAME")
		std::cout << "inventory name passed." << std::endl;
	if (inventory.count == 3)
		std::cout << "inventory id passed." << std::endl;
}

void testItem(Item item) {
	std::cout << "Test create item: " << std::endl;
	if (item.name == "NAME")
		std::cout << "Item name passed." << std::endl;
	if (item.number == 3)
		std::cout << "Item id passed." << std::endl;
	if (item.warehouse == 1)
		std::cout << "Item warehouse id passed." << std::endl;
}

void testLocation(Location loc, int x, int y, int s) {
	std::cout << "Test create Location: " << std::endl;
	if (loc.x == x)
		std::cout << "Location x passed." << std::endl;
	if (loc.y == y)
		std::cout << "Location y passed." << std::endl;
	if (loc.shelf == s)
		std::cout << "Location shelf passed." << std::endl;
}

void testTask(Task t, Location loc, int des, int type) {
	std::cout << "Test create Task: " << std::endl;
	if (t.loc.x == loc.x)
		std::cout << "Task location passed." << std::endl;
	if (t.dest == des)
		std::cout << "Task dest passed." << std::endl;
	if (t.type == type)
		std::cout << "Task type passed." << std::endl;
}

void testDynamicQueue(DynamicQueue& dq, Task t1, Task t2, Task t3) {
	dq.add(&t1);
	dq.add(&t2);
	dq.add(&t3);
	if (dq.get()->dest = t1.dest)
		std::cout << "Dynamic queue add & get 1 passed" << std::endl;
	if (dq.get()->dest = t2.dest)
		std::cout << "Dynamic queue add & get 1 passed" << std::endl;
	if (dq.get()->dest = t3.dest)
		std::cout << "Dynamic queue add & get 1 passed" << std::endl;
}

int main() {
	//put all initialization here
	Database db;
	Item item("NAME",3,1);
	Inventory inventory("NAME", 3);
	Location loc3(3, 3, 3);
	Location loc2(2, 2, 2);
	Location loc1(1, 1, 1);
	Task t1(loc1, 1);
	Task t2(loc2, 2);
	Task t3(loc3, 3);
	DynamicQueue dq;
	system("cls"); //clear screen

	//run test cases
	std::cout << "==================================================" << std::endl;
	testDatabase(db);
	std::cout << "==================================================" << std::endl;
	testLocation(loc1, 1, 1, 1);
	testLocation(loc2, 2, 2, 2);
	testLocation(loc3, 3, 3, 3);
	std::cout << "==================================================" << std::endl;
	testTask(t1, loc1, 1, 0);
	testTask(t2, loc2, 2, 0);
	testTask(t3, loc2, 3, 0);
	std::cout << "==================================================" << std::endl;
	testDynamicQueue(std::ref(dq), t1, t2, t3);
	std::cout << "==================================================" << std::endl;
	testInventory(inventory);
	std::cout << "==================================================" << std::endl;
	testItem(item);
	std::cout << "==================================================" << std::endl;
	cpen333::pause();
	return 0;
}