extern void pfd_connector_setup();
extern void pfd_connector_tick();


extern void pfd_read_airspeed(char token);
extern void pfd_read_ground_speed(char token);
extern void pfd_read_altitude(char token);
extern void pfd_read_heading(char token);
extern void pfd_read_ground_clearance(char token);
extern void pfd_read_aircraft_on_ground(char token);
extern void pfd_read_ils_loc_offset(char token);
extern void pfd_read_ils_gs_offset(char token);
extern void pfd_read_reference_speed(char token);
extern void pfd_read_speed_hold_active(char token);
extern void pfd_read_reference_altitude(char token);
extern void pfd_read_altitude_hold_active(char token);
extern void pfd_read_pitch(char token);
extern void pfd_read_roll(char token);
extern void pfd_read_altimeter_setting(char token);
extern void pfd_read_overspeed_warning(char token);
extern void pfd_read_stall_warning(char token);
extern void pfd_read_g_force(char token);
extern void pfd_read_angle_of_attack(char token);
extern void pfd_read_toga_active(char token);
