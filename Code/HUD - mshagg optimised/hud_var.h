// Define the attitude indicator screen bounds
float scale = 1.5;
float x_limit = 110 * scale; // Define the attitude indicator canvas size
float y_limit = 160 * scale; // Define the attitude indicator canvas size

float hud_scale_x = 0.78;
float hud_scale_y = 0.53;
int hud_x_limit = 128; // Define the HUD attitude indicator canvas size
int hud_y_limit = 128; // Define the HUD attitude indicator canvas size

// Define the centre coordinates of the main screen attitude indicator
float x_center = x_limit/2; 
float y_center = y_limit/2; 

float x0; // Define the aircraft centre X-coordinate
float y_0; // Aircraft centre Y-coordinate; use y_0 because y0 is already reserved.
float r0; // Distance from the attitude indicator centre to the aircraft nose centre

float alpha; // Angle between the attitude indicator normal and the horizon line
float beta; // Angle between the attitude line and the screen X-axis
float cosa; //cos(alpha)
float sina; //sin(alpha)
float cosb; //cos(beta)
float sinb; //sin(beta)
char level_char[10];

float roll = 0;
float pitch = 0;
float yaw;

// Define a 2D array to store attitude data
int att_points[7][8];

// Define a 2D array for HUD attitude data
int hud_att_points[7][8];

float xa1, xa2, xa3, xa4, ya1, ya2, ya3, ya4; // 0-degree horizon line used to calculate other attitude line coordinates
float xb1, xb2, xb3, xb4, yb1, yb2, yb3, yb4; // Define a long 0-degree horizon line
float xb1_h, xb2_h, xb3_h, xb4_h, yb1_h, yb2_h, yb3_h, yb4_h; // Define a long 0-degree horizon line for the HUD
float xb5, yb5, xb6, yb6, xb7, yb7, xb8, yb8; // Define the four point coordinates used to draw the sky box

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

const float pitch_to_pixel = level_pixel/radians(level_angle); // Pixel spacing per 10-degree horizon line
float vx, vy; // Level vector between adjacent attitude line midpoints
float center_degree; // Pitch angle value; the flight controller provides pitch in radians
int base_vect; // Nearest attitude line index: base
int draw_level_angle; // The horizontal attitude line to be drawn

//mavlink definitions
mavlink_message_t msg; // MAVLink message object
mavlink_status_t status; // MAVLink status object
uint8_t msgReceived = false; 