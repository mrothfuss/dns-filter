typedef struct key_data * msp_data;

// Initiates the map
void msp_init(msp_data *map);

// Remove all entries from the map
void msp_erase(msp_data *map);

// Dump all entries from the map to the screen
void msp_dump(msp_data *map);

// Push a key/value onto the map
// return: 1 = success, 0 = failure
int msp_push(msp_data *map, void *string, int length, void *pointer);

// Find the matching pointer
void * msp_find(msp_data *map, void *string, int length);

// Find and remove the matching pointer
void * msp_pop(msp_data *map, void *string, int length);

// Perform a function on each entry
void msp_foreach(msp_data *map, void (*function)(void *string, int length, void *pointer));
