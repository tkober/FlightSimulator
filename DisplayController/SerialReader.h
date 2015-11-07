
extern void (*token_processing)(char token);
extern int position_to_write;

extern void serial_setup();

extern int read_initialization();
extern int process_initialization_data();

extern void read_serial();
extern void store_token(char token, char destination[], int size, int *updated);


#define DEFAULT_TOKEN_PROCESSING    determine_extraction_type

#define READ_COM1_FREQ              DEFAULT_TOKEN_PROCESSING
#define READ_COM1_SB_FREQ           DEFAULT_TOKEN_PROCESSING
#define READ_COM2_FREQ              DEFAULT_TOKEN_PROCESSING
#define READ_COM2_SB_FREQ           DEFAULT_TOKEN_PROCESSING
#define READ_NAV1_FREQ              DEFAULT_TOKEN_PROCESSING
#define READ_NAV1_SB_FREQ           DEFAULT_TOKEN_PROCESSING
#define READ_NAV2_FREQ              DEFAULT_TOKEN_PROCESSING
#define READ_NAV2_SB_FREQ           DEFAULT_TOKEN_PROCESSING
#define READ_TRANSPONDER_CODE       DEFAULT_TOKEN_PROCESSING
#define READ_COM1_SOUND_ACTIVE      DEFAULT_TOKEN_PROCESSING
#define READ_COM2_SOUND_ACTIVE      DEFAULT_TOKEN_PROCESSING
#define READ_NAV1_SOUND_ACTIVE      DEFAULT_TOKEN_PROCESSING
#define READ_NAV2_SOUND_ACTIVE      DEFAULT_TOKEN_PROCESSING
#define READ_SPEED                  DEFAULT_TOKEN_PROCESSING
#define READ_SPEED_ACTIVE           DEFAULT_TOKEN_PROCESSING
#define READ_HEADING                DEFAULT_TOKEN_PROCESSING
#define READ_HEADING_ACTIVE         DEFAULT_TOKEN_PROCESSING
#define READ_COURSE                 DEFAULT_TOKEN_PROCESSING
#define READ_AP_STATUS              DEFAULT_TOKEN_PROCESSING
#define READ_A_THR_STATUS           DEFAULT_TOKEN_PROCESSING
#define READ_ALTITUDE               DEFAULT_TOKEN_PROCESSING
#define READ_VERTICAL_SPEED         DEFAULT_TOKEN_PROCESSING
#define READ_ALTITUDE_ACTIVE        DEFAULT_TOKEN_PROCESSING
#define READ_APR_HOLD               DEFAULT_TOKEN_PROCESSING
#define READ_NAV_HOLD               DEFAULT_TOKEN_PROCESSING
#define READ_GLIDESLOPE_HOLD        DEFAULT_TOKEN_PROCESSING

#define READ_ELEVATOR_TRIM          DEFAULT_TOKEN_PROCESSING
#define READ_PARKING_BRAKE_STATUS   DEFAULT_TOKEN_PROCESSING

#define READ_GEAR_POSITION          DEFAULT_TOKEN_PROCESSING
#define READ_GEAR_RETRACTABLE       DEFAULT_TOKEN_PROCESSING
#define READ_RUDDERT_TRIM           DEFAULT_TOKEN_PROCESSING
#define READ_PARKING_BRAKE_STATUS   DEFAULT_TOKEN_PROCESSING




#ifdef CONTROLLER_A
// ECAM

// Radio
#define READ_COM1_FREQ              read_com1_freq
#define READ_COM1_SB_FREQ           read_com1_sb_freq
#define READ_COM2_FREQ              read_com2_freq
#define READ_COM2_SB_FREQ           read_com2_sb_freq
#define READ_NAV1_FREQ              read_nav1_freq
#define READ_NAV1_SB_FREQ           read_nav1_sb_freq
#define READ_NAV2_FREQ              read_nav2_freq
#define READ_NAV2_SB_FREQ           read_nav2_sb_freq
#define READ_TRANSPONDER_CODE       read_transponder_code
#define READ_COM1_SOUND_ACTIVE      read_com1_sound_active
#define READ_COM2_SOUND_ACTIVE      read_com2_sound_active
#define READ_NAV1_SOUND_ACTIVE      read_nav1_sound_active
#define READ_NAV2_SOUND_ACTIVE      read_nav2_sound_active

// Gear
#define READ_GEAR_POSITION          read_gear_position
#define READ_GEAR_RETRACTABLE       read_gear_retractable
#define READ_PARKING_BRAKE_STATUS   read_parking_brake_status

#endif


#ifdef CONTROLLER_B
// AP
#define READ_SPEED                  read_speed
#define READ_SPEED_ACTIVE           read_speed_active
#define READ_HEADING                read_heading
#define READ_HEADING_ACTIVE         read_heading_active
#define READ_COURSE                 read_course
#define READ_AP_STATUS              read_ap_status
#define READ_A_THR_STATUS           read_a_thr_status
#define READ_ALTITUDE               read_altitude
#define READ_VERTICAL_SPEED         read_vertical_speed
#define READ_ALTITUDE_ACTIVE        read_altitude_active
#define READ_APR_HOLD               read_apr_hold_status   
#define READ_NAV_HOLD               read_nav_hold_status
#define READ_GLIDESLOPE_HOLD        read_glideslope_hold_status

// Flight Controls
#define READ_ELEVATOR_TRIM          read_elevator_trim
#define READ_RUDDERT_TRIM           read_ruddert_trim

#endif


#ifdef CONTROLLER_C
#endif

