// Define the attitude indicator screen bounds
float scale = 1.5;
float x_limit = 110 * scale; // Define the attitude indicator canvas size
float y_limit = 160 * scale; // Define the attitude indicator canvas size

float hud_scale_x = 0.78; //
float hud_scale_y = 0.53;
int hud_x_limit = 128; // Define the HUD attitude indicator canvas size
int hud_y_limit = 128; // Define the HUD attitude indicator canvas size

// Define the centre coordinates of the main screen attitude indicator
float x_center = x_limit/2; 
float y_center = y_limit/2; 

// Define the centre coordinates of the HUD attitude indicator
float x_center_h; 
float y_center_h; 

float x0; // Define the aircraft centre X-coordinate
float y_0; // Define the aircraft centre Y-coordinate. y0 is already used by the system, so y_0 is used instead.
float r0; // Distance from the attitude indicator centre to the aircraft nose centre

float alpha; // Angle between the attitude indicator normal and the horizon line
float beta; // Angle between the attitude line and the screen X-axis
float cosa; //cos(alpha)
float sina; //sin(alpha)
float cosb; //cos(beta)
float sinb; //sin(beta)
float theta;
char level_char[10];

float roll = 0;
float pitch = 0.04;
float old_roll = 0;
float old_pitch = 0;
float yaw;

// Define a 2D array to store attitude data
int att_points[7][8];

// Define a 2D array to store the previous attitude data
int old_att_points[7][8];

// Define a 2D array for HUD attitude data
int hud_att_points[7][8];

float xa1, xa2, xa3, xa4, ya1, ya2, ya3, ya4; // 0-degree horizon line, same length as other pitch lines, used to calculate attitude line coordinates
float xb1, xb2, xb3, xb4, yb1, yb2, yb3, yb4; // Define a long 0-degree horizon line
float xb1_h, xb2_h, xb3_h, xb4_h, yb1_h, yb2_h, yb3_h, yb4_h; // Define a long 0-degree horizon line for the HUD
float xb5, yb5, xb6, yb6, xb7, yb7, xb8, yb8; //定义用于绘制天空方框的四个点坐标
float old_xb1, old_xb2, old_xb3, old_xb4, old_yb1, old_yb2, old_yb3, old_yb4;
float xbs1, xbs2, xbs3, xbs4, ybs1, ybs2, ybs3, ybs4; // Base level at the centre of the attitude indicator
float old_xbs1, old_xbs2, old_xbs3, old_xbs4, old_ybs1, old_ybs2, old_ybs3, old_ybs4;  // Previous base-level horizontal attitude line, i.e. the screen centre level
float xp1, xp2, xp3, xp4, yp1, yp2, yp3, yp4; // Horizontal line coordinates used in the for loop

// Horizon line lengths: r1----r2  0  r3----r4, measured in pixels from centre.
float r1 = 30 * scale;
float r2 = 15 * scale;
float r3 = 15 * scale;
float r4 = 30 * scale;

// Define a long 0-degree horizon line
float l1 = 48 * scale;
float l2 = 8 * scale;
float l3 = 8 * scale;
float l4 = 48 * scale;

float l0 = 150 * scale; // Ground-sky boundary line
const float level_pixel = 30 * scale; // Pixel spacing between each horizontal attitude line

const int level_angle = 10; // Horizontal attitude line spacing in degrees

const float pitch_to_pixel = level_pixel/radians(level_angle); // Pixel spacing for each 10-degree horizon line
float vx, vy; // Level vector between adjacent attitude line midpoints
float center_degree; // Pitch angle value; the pitch value from the flight controller is in radians
int base_vect; // base: index of the attitude line closest to the attitude indicator centre
int old_base_vect;
String level_str;
int draw_level_angle; // Target horizontal attitude line

//mavlink definitions
mavlink_message_t msg; // Define a MAVLink message object
mavlink_status_t status; // Define a MAVLink status object
uint8_t msgReceived = false; 

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF
#define GRAY     0x2A0A
unsigned long color;
uint8_t brightness_new_main; //lilygo amoled 最亮值255，对应pwm 1900 最暗值0，对应pwm 1100. 
uint8_t brightness_old_main;

uint8_t brightness_new; //lilygo amoled 最亮值255，对应pwm 1900 最暗值0，对应pwm 1100. 
uint8_t brightness_old;

// Variables and parameters used by the PNG decoder
#define MAX_IMAGE_WDITH 320 // Adjust for your images
int16_t rc;
int16_t xpos;
int16_t ypos;

// #define MAVLINK_MSG_ID_HEARTBEAT 0
uint8_t base_mode;
uint32_t custom_mode;
uint32_t flight_mode;
String flight_mode_str;
uint8_t system_status;
String system_status_str;
//#define MAVLINK_MSG_ID_AVAILABLE_MODES 435
uint8_t mode_index; /*<  The current mode index within number_modes, indexed from 1.*/

//#define MAVLINK_MSG_ID_SYS_STATUS 1
uint16_t voltage_battery = 19200; /*< [mV] Battery voltage, UINT16_MAX: Voltage not sent by autopilot*/
int16_t current_battery; /*< [cA] Battery current, -1: Current not sent by autopilot*/
int8_t battery_remaining; /*< [%] Battery energy remaining, -1: Battery remaining energy not sent by autopilot*/

//#define MAVLINK_MSG_ID_BATTERY_STATUS 147
int32_t current_consumed; /*< [mAh] Consumed charge, -1: autopilot does not provide consumption estimate*/

// #define MAVLINK_MSG_ID_GPS_RAW_INT 24
uint16_t cog; /*< [cdeg] Course over ground (NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/
uint8_t fix_type; /*<  GPS fix type.*/
uint8_t satellites_visible; /*<  Number of satellites visible. If unknown, set to UINT8_MAX*/

//#define MAVLINK_MSG_ID_DISTANCE_SENSOR 132
float current_distance; /*< [cm] Current distance reading*/
float rangefinder_distance;

//#define MAVLINK_MSG_ID_GLOBAL_POSITION_INT 33
uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot).*/
int32_t relative_alt; /*< [mm] Altitude above ground*/
uint16_t hdg; /*< [cdeg] Vehicle heading (yaw angle), 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/

//MAVLINK_MSG_ID_REQUEST_DATA_STREAM: //66
uint16_t req_message_rate;

// #define MAVLINK_MSG_ID_DATA_STREAM 67
uint16_t message_rate = 9; ///< The requested interval between two messages of this type
uint8_t stream_id = 9; ///< The ID of the requested data stream
uint8_t on_off; ///< 1 stream is enabled, 0 stream is stopped.


//#define MAVLINK_MSG_ID_VFR_HUD 74
float airspeed; /*< [m/s] Vehicle speed in form appropriate for vehicle type. For standard aircraft this is typically calibrated airspeed (CAS) or indicated airspeed (IAS) - either of which can be used by a pilot to estimate stall speed.*/
float groundspeed; /*< [m/s] Current ground speed.*/
float alt; /*< [m] Current altitude (MSL).*/
float climb; /*< [m/s] Current climb rate.*/
int16_t heading; /*< [deg] Current heading in compass units (0-360, 0=north).*/
uint16_t throttle; /*< [%] Current throttle setting (0 to 100).*/

// #define MAVLINK_MSG_ID_RC_CHANNELS_RAW 35
 uint16_t chan1_raw; ///< RC channel 1 value, in microseconds
 uint16_t chan2_raw; ///< RC channel 2 value, in microseconds
 uint16_t chan3_raw; ///< RC channel 3 value, in microseconds
 uint16_t chan4_raw; ///< RC channel 4 value, in microseconds
 uint16_t chan5_raw; ///< RC channel 5 value, in microseconds
 uint16_t chan6_raw; ///< RC channel 6 value, in microseconds
 uint16_t chan7_raw; ///< RC channel 7 value, in microseconds
 uint16_t chan8_raw; ///< RC channel 8 value, in microseconds
 
 uint8_t port; ///< Servo output port (set of 8 outputs = 1 port). Most MAVs will just use one, but this allows to encode more than 8 servos.

 uint16_t chan10_raw;

// MAVLINK_MSG_ID_RC_CHANNELS 65
 uint8_t rssi; ///< Receive signal strength indicator, 0: 0%, 255: 100%

// Time tracking variable
char time_str[6];

unsigned long Millis0 = 0;
unsigned long Millis1st = 0;
unsigned long Millis2nd = 0;
unsigned long Millis3rd = 0;
unsigned long Millis4th = 0;
unsigned long now0 = 0;
unsigned long now1 = 0;
unsigned long now2 = 0;
unsigned long now3 = 0;
unsigned long now4 = 0;

// Global variables to store latest values
int32_t current_lat = 0, current_lon = 0;
int32_t home_lat = 0, home_lon = 0;
float current_yaw = 0.0;  // in radians

// Variables used by the radar
#define RADAR_RADIUS 70
#define RADAR_CENTER_X 95
#define RADAR_CENTER_Y 90
#define MAX_TARGETS 8
#define SWEEP_WIDTH 15  // Beam width, in degrees

struct Target {
  float x, y;
  float dx, dy;
  uint16_t color;
  unsigned long time_cur = 0;
  unsigned long time_pre = 0;

};

Target targets[MAX_TARGETS];
bool radarInitialized = false;
int scanAngle = 0;

