#include "esp_arduino_version.h"
#include <LilyGo_AMOLED.h>      //To use LilyGo AMOLED series screens, please include <LilyGo_AMOLED.h>
//#include <LV_Helper.h>
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

#include "cockpit_var.h"

#include "salt_45_43.h" //GPS Icon

// Required for dual core operation
TaskHandle_t Task1;
TaskHandle_t Task2;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object, 姿态仪所用sprite
TFT_eSprite spr_meters = TFT_eSprite(&tft);
TFT_eSprite spr_meters_2 = TFT_eSprite(&tft);
TFT_eSprite spr_mid = TFT_eSprite(&tft);

LilyGo_Class amoled;

// Define Aux display as u8g1。Uses hardware I2C for higher FPS
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g1(U8G2_R0, /* reset=*/ 15, /* clock=*/ 1, /* data=*/ 2);

void setup() {

  amoled.begin();
  amoled.setRotation(0);
  u8g1.begin();
  u8g1.enableUTF8Print();
  u8g1.setFont(u8g2_font_8x13B_tf);
  Serial.begin(115200);
  spr.createSprite(165, 240);  //attitude indicator
  spr_meters.createSprite(171, 240);
  spr_mid.createSprite(171, 240);
  randomSeed(millis());

xTaskCreatePinnedToCore(
                  Task1code,   /* Task function. */
                  "Task1",     /* name of task. */
                  10000,       /* Stack size of task */
                  NULL,        /* parameter of the task */
                  1,           /* priority of the task */
                  &Task1,      /* Task handle to keep track of created task */
                  0);          /* pin task to core 0 */     


  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
}

// task1code - updates main display
void Task1code( void * pvParameters ){
   for(;;){
    vTaskDelay(1); //disable task watchdog
    push_main_screen();

   static unsigned long lastBrightnessCheck = 0;
if (millis() - lastBrightnessCheck >= 250) {
  adjust_brightness_main();
  lastBrightnessCheck = millis();
}
     
    }      
  } 

// Task2code - updates auxillary display
void Task2code( void * pvParameters ){

  for(;;){
    vTaskDelay(1);
    push_u8g1();
               
    }
  }

void loop(void) {  
  
} // void loop
