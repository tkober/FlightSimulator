
extern void (*token_processing)(char token);
extern int position_to_write;

extern void serial_setup();

extern int read_initialization();
extern int process_initialization_data();

extern void read_serial();
extern void store_token(char token, char destination[], int size, int *updated);
