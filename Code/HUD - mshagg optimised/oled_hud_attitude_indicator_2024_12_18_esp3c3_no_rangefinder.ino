#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <MAVLink.h>
#include "TFT_eSPI.h"
#include <math.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>

#endif

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>  //hardware I2C need this library
#endif

#include "hud_var.h"


//Define u8g3 display for HUD
U8G2_SSD1327_WS_128X128_F_4W_HW_SPI u8g3(U8G2_R0, /* cs=*/ 7, /* dc=*/ 2, /* reset=*/ 3); // Hardware SPI definition for faster refresh, SDA = 6, SCK = 4

void setup() {

  u8g3.begin();
  u8g3.sendF("ca", 0xa0, 0x41); // Mirror the display content
  u8g3.setContrast(255);
  u8g3.enableUTF8Print();
  u8g3.setFont(u8g2_font_8x13B_tf);
  Serial.begin(115200);
}

void loop(void) {  
  push_u8g3();
} // void loop
