#ifndef AMAZON_CONSTANTS_H
#define AMAZON_CONSTANTS_H

//constants
#define CLIENT_VIEW '1'
#define CLIENT_RESERVE '2'
#define CLIENT_ORDER '3'
#define CLIENT_QUIT '0'

#define MANAGER_VIEW_STOCK '1'
#define MANAGER_VIEW_ORDER '2'
#define MANAGERQUIT '0'
#define WAREHOUSE_PRINT_MUTEX "warehouse_print_mutex"

#define CLIENT_VIEW_SERVER 1
#define CLIENT_RESERVE_SERVER 2
#define CLIENT_ORDER_SERVER 3
#define CLIENT_QUIT_SERVER 0
#define PORT_NUMBER 52102
#define WAREHOUSE_PORT_NUMBER 52103
#define DATABASE_MUTEX "database_mutex"

#define ITEMS_INITIALIZATION_FILE "data/items_initialization_file.txt"

#define WAREHOUSE_MEMORY "warehouse_memory"
#define WAREHOUSE_INITIALIZATION_FILEA "data/warehouse_initialization_fileA.txt"
#define WAREHOUSE_INITIALIZATION_FILEB "data/warehouse_initialization_fileB.txt"
#define WAREHOUSE_INITIALIZATION_FILEC "data/warehouse_initialization_fileC.txt"
#define MAP_INITIALIZATION_FILEA "data/map_initialization_fileA.txt"
#define MAP_INITIALIZATION_FILEB "data/map_initialization_fileB.txt"
#define MAP_INITIALIZATION_FILEC "data/map_initialization_fileC.txt"

#endif