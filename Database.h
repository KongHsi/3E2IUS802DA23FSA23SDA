#include <string>
#include <iostream>
#include <map>
#include "Constants.h"
#include <cpen333/process/mutex.h>
#include <cpen333/process/subprocess.h>

using namespace std;

class Database {
	public:
		map<string, int> database;
		cpen333::process::mutex mutex(DATABASE_MUTEX);
};
