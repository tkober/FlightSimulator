
extern void (*token_processing)(char token);
extern int position_to_write;

extern void serial_setup();

extern int read_initialization();
extern int process_initialization_data();

extern void read_serial();
extern void store_token(char token, char destination[], int size, int *updated);


#define DEFAULT_TOKEN_PROCESSING      determine_extraction_type

#define READ_COM1_FREQ                DEFAULT_TOKEN_PROCESSING
#define READ_COM1_SB_FREQ             DEFAULT_TOKEN_PROCESSING
#define READ_COM2_FREQ                DEFAULT_TOKEN_PROCESSING
#define READ_COM2_SB_FREQ             DEFAULT_TOKEN_PROCESSING
#define READ_NAV1_FREQ                DEFAULT_TOKEN_PROCESSING
#define READ_NAV1_SB_FREQ             DEFAULT_TOKEN_PROCESSING
#define READ_NAV2_FREQ                DEFAULT_TOKEN_PROCESSING
#define READ_NAV2_SB_FREQ             DEFAULT_TOKEN_PROCESSING
#define READ_TRANSPONDER_CODE         DEFAULT_TOKEN_PROCESSING
#define READ_COM1_SOUND_ACTIVE        DEFAULT_TOKEN_PROCESSING
#define READ_COM2_SOUND_ACTIVE        DEFAULT_TOKEN_PROCESSING
#define READ_NAV1_SOUND_ACTIVE        DEFAULT_TOKEN_PROCESSING
#define READ_NAV2_SOUND_ACTIVE        DEFAULT_TOKEN_PROCESSING
#define READ_REFERENCE_SPEED          DEFAULT_TOKEN_PROCESSING
#define READ_SPEED_HOLD_ACTIVE        DEFAULT_TOKEN_PROCESSING
#define READ_REFERENCE_HEADING        DEFAULT_TOKEN_PROCESSING
#define READ_HEADING_HOLD_ACTIVE      DEFAULT_TOKEN_PROCESSING
#define READ_COURSE                   DEFAULT_TOKEN_PROCESSING
#define READ_AP_STATUS                DEFAULT_TOKEN_PROCESSING
#define READ_A_THR_STATUS             DEFAULT_TOKEN_PROCESSING
#define READ_REFERENCE_ALTITUDE       DEFAULT_TOKEN_PROCESSING
#define READ_REFERENCE_VERTICAL_SPEED DEFAULT_TOKEN_PROCESSING
#define READ_ALTITUDE_HOLD_ACTIVE     DEFAULT_TOKEN_PROCESSING
#define READ_APR_HOLD                 DEFAULT_TOKEN_PROCESSING
#define READ_NAV_HOLD                 DEFAULT_TOKEN_PROCESSING
#define READ_GLIDESLOPE_HOLD          DEFAULT_TOKEN_PROCESSING

#define READ_ELEVATOR_TRIM            DEFAULT_TOKEN_PROCESSING
#define READ_SPOILERS_ARMED           DEFAULT_TOKEN_PROCESSING
#define READ_PARKING_BRAKE_STATUS     DEFAULT_TOKEN_PROCESSING

#define READ_GEAR_POSITION            DEFAULT_TOKEN_PROCESSING
#define READ_GEAR_RETRACTABLE         DEFAULT_TOKEN_PROCESSING
#define READ_RUDDERT_TRIM             DEFAULT_TOKEN_PROCESSING
#define READ_PARKING_BRAKE_STATUS     DEFAULT_TOKEN_PROCESSING

#define READ_AIRSPEED                 DEFAULT_TOKEN_PROCESSING
#define READ_GROUND_SPEED             DEFAULT_TOKEN_PROCESSING
#define READ_ALTITUDE                 DEFAULT_TOKEN_PROCESSING
#define READ_HEADING                  DEFAULT_TOKEN_PROCESSING
#define READ_GROUND_CLEARANCE         DEFAULT_TOKEN_PROCESSING
#define READ_AIRCRAFT_ON_GROUND       DEFAULT_TOKEN_PROCESSING
#define READ_ILS_LOC_OFFSET           DEFAULT_TOKEN_PROCESSING
#define READ_ILS_GS_OFFSET            DEFAULT_TOKEN_PROCESSING
#define READ_PITCH                    DEFAULT_TOKEN_PROCESSING
#define READ_ROLL                     DEFAULT_TOKEN_PROCESSING
#define READ_ALTIMETER_SETTING        DEFAULT_TOKEN_PROCESSING
#define READ_OVERSPEED_WARNING        DEFAULT_TOKEN_PROCESSING
#define READ_STALL_WARNING            DEFAULT_TOKEN_PROCESSING
#define READ_G_FORCE                  DEFAULT_TOKEN_PROCESSING
#define READ_ANGLE_OF_ATTACK          DEFAULT_TOKEN_PROCESSING
#define READ_TOGA                     DEFAULT_TOKEN_PROCESSING



#ifdef CONTROLLER_A
// ECAM

// Radio
#define READ_COM1_FREQ                read_com1_freq
#define READ_COM1_SB_FREQ             read_com1_sb_freq
#define READ_COM2_FREQ                read_com2_freq
#define READ_COM2_SB_FREQ             read_com2_sb_freq
#define READ_NAV1_FREQ                read_nav1_freq
#define READ_NAV1_SB_FREQ             read_nav1_sb_freq
#define READ_NAV2_FREQ                read_nav2_freq
#define READ_NAV2_SB_FREQ             read_nav2_sb_freq
#define READ_TRANSPONDER_CODE         read_transponder_code
#define READ_COM1_SOUND_ACTIVE        read_com1_sound_active
#define READ_COM2_SOUND_ACTIVE        read_com2_sound_active
#define READ_NAV1_SOUND_ACTIVE        read_nav1_sound_active
#define READ_NAV2_SOUND_ACTIVE        read_nav2_sound_active

// Gear
#define READ_GEAR_POSITION            read_gear_position
#define READ_GEAR_RETRACTABLE         read_gear_retractable
#define READ_PARKING_BRAKE_STATUS     read_parking_brake_status

#endif


#ifdef CONTROLLER_B
// AP
#define READ_REFERENCE_SPEED          read_reference_speed
#define READ_SPEED_HOLD_ACTIVE        read_speed_hold_active
#define READ_REFERENCE_HEADING        read_reference_heading
#define READ_HEADING_HOLD_ACTIVE      read_heading_hold_active
#define READ_COURSE                   read_course
#define READ_AP_STATUS                read_ap_status
#define READ_A_THR_STATUS             read_a_thr_status
#define READ_REFERENCE_ALTITUDE       read_reference_altitude
#define READ_REFERENCE_VERTICAL_SPEED read_reference_vertical_speed
#define READ_ALTITUDE_HOLD_ACTIVE     read_altitude_hold_active
#define READ_APR_HOLD                 read_apr_hold_status
#define READ_NAV_HOLD                 read_nav_hold_status
#define READ_GLIDESLOPE_HOLD          read_glideslope_hold_status

// Flight Controls
#define READ_ELEVATOR_TRIM            read_elevator_trim
#define READ_RUDDERT_TRIM             read_ruddert_trim
#define READ_SPOILERS_ARMED           controls_read_spoilers_armed

#endif


#ifdef CONTROLLER_C

#define READ_AIRSPEED                 pfd_read_airspeed
#define READ_GROUND_SPEED             pfd_read_ground_speed
#define READ_ALTITUDE                 pfd_read_altitude
#define READ_HEADING                  pfd_read_heading
#define READ_GROUND_CLEARANCE         pfd_read_ground_clearance
#define READ_AIRCRAFT_ON_GROUND       pfd_read_aircraft_on_ground
#define READ_ILS_LOC_OFFSET           pfd_read_ils_loc_offset
#define READ_ILS_GS_OFFSET            pfd_read_ils_gs_offset
#define READ_REFERENCE_SPEED          pfd_read_reference_speed
#define READ_SPEED_HOLD_ACTIVE        pfd_read_speed_hold_active
#define READ_REFERENCE_ALTITUDE       pfd_read_reference_altitude
#define READ_ALTITUDE_HOLD_ACTIVE     pfd_read_altitude_hold_active
#define READ_PITCH                    pfd_read_pitch
#define READ_ROLL                     pfd_read_roll
#define READ_ALTIMETER_SETTING        pfd_read_altimeter_setting
#define READ_OVERSPEED_WARNING        pfd_read_overspeed_warning
#define READ_STALL_WARNING            pfd_read_stall_warning
#define READ_G_FORCE                  pfd_read_g_force
#define READ_ANGLE_OF_ATTACK          pfd_read_angle_of_attack
#define READ_TOGA                     pfd_read_toga_active

#endif
