#ifndef AMAZON_CONSTANTS_H
#define AMAZON_CONSTANTS_H

//constants
#define CLIENT_VIEW '1'
#define CLIENT_RESERVE '2'
#define CLIENT_ORDER '3'
#define CLIENT_QUIT '0'

#define MANAGER_VIEW_STOCK '1'
#define MANAGER_VIEW_ORDER '2'
#define MANAGER_ADD_ROBOT '3'
#define MANAGER_REMOVE_ROBOT '4'
#define MANAGER_VIEW_ROBOTS '5'
#define MANAGERQUIT '0'
#define WAREHOUSE_PRINT_MUTEX "warehouse_print_mutex"

#define CLIENT_VIEW_SERVER 1
#define CLIENT_RESERVE_SERVER 2
#define CLIENT_ORDER_SERVER 3
#define CLIENT_QUIT_SERVER 0
#define PORT_NUMBER 52102
#define WAREHOUSE_PORT_NUMBER 52103
#define DATABASE_MUTEX "database_mutex"

#define DECK_SEMAPHORE1 "deck_semaphore"
#define DECK_SEMAPHORE_SIZE1 4
#define DECK_MUTEX1 "deck_mutex"
#define DECK_CV1 "deck_cv"
#define DECK_SEMAPHORE2 "deck_semaphore2"
#define DECK_SEMAPHORE_SIZE2 4
#define DECK_MUTEX2 "deck_mutex2"
#define DECK_CV2 "deck_cv2"
#define DECK_SEMAPHORE3 "deck_semaphore3"
#define DECK_SEMAPHORE_SIZE3 4
#define DECK_MUTEX3 "deck_mutex3"
#define DECK_CV3 "deck_cv3"







#define ITEMS_INITIALIZATION_FILE "data/items_initialization_file.txt"

#define WAREHOUSE_MEMORY "warehouse_memory"
#define WAREHOUSE_INITIALIZATION_FILEA "data/warehouse_initialization_fileA.txt"
#define WAREHOUSE_INITIALIZATION_FILEB "data/warehouse_initialization_fileB.txt"
#define WAREHOUSE_INITIALIZATION_FILEC "data/warehouse_initialization_fileC.txt"
#define MAP_INITIALIZATION_FILEA "data/map_initialization_fileA.txt"
#define MAP_INITIALIZATION_FILEB "data/map_initialization_fileB.txt"
#define MAP_INITIALIZATION_FILEC "data/map_initialization_fileC.txt"
#define TRUCK_INITIALIZATION_FILEA "data/truck_initialization_fileA.txt"
#endif