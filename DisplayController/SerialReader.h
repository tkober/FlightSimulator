
extern void (*token_processing)(char token);
extern int position_to_write;

extern void serial_setup();
extern void read_serial();
extern void store_token(char token, char destination[], int size, int *updated);
