#include "esp_arduino_version.h"
// #if ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3,0,0)
#include <LilyGo_AMOLED.h>      //To use LilyGo AMOLED series screens, please include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
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

#include "tftespi_cn_fonts.h"
#include "tftespi_en_fonts.h"
#include "salt_45_43.h" //GPS卫星图标

// 双核运行所需handle，用于追踪执行情况
TaskHandle_t Task1;
TaskHandle_t Task2;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object, 姿态仪所用sprite
TFT_eSprite spr_meters = TFT_eSprite(&tft);
TFT_eSprite spr_meters_2 = TFT_eSprite(&tft);
TFT_eSprite spr_mid = TFT_eSprite(&tft);

LilyGo_Class amoled;

//定义最上方副显示屏（1号屏）的虚拟对象，起名u8g1。1号屏用硬件I2C协议，FPS高，用于显示电流数据。
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g1(U8G2_R0, /* reset=*/ 15, /* clock=*/ 1, /* data=*/ 2);  //15引脚是空的

//定义下方副显示屏（2号屏）的虚拟对象，起名u8g2。无法同时开启两个硬件I2C协议，用软件I2C。FPS较低。
U8G2_SSD1306_72X40_ER_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 41, /* data=*/ 40, /* reset=*/ U8X8_PIN_NONE);  

void setup() {

  amoled.begin();
  amoled.setRotation(0);
  u8g1.begin();
  u8g2.begin();
  u8g1.enableUTF8Print();
  u8g2.enableUTF8Print();

  u8g1.setFont(u8g2_font_wqy13_t_gb2312b);
  u8g2.setFont(u8g2_font_wqy16_t_gb2312b);

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

// task1code, 显示主显示屏信息
void Task1code( void * pvParameters ){
  //task 的时间不能太短，否则会触发task watchdog,导致芯片重启
  // Serial.print("Task1 begines on core ");
  // Serial.println(xPortGetCoreID());
  for(;;){
    vTaskDelay(1); //disable task watchdog

    // Serial.print("Task1 running on core ");
    // Serial.println(xPortGetCoreID());
    // Serial.print("time_1_push main:");
    // now2 = millis();
    // Serial.println(now2);
    

    push_main_screen();
    adjust_brightness_main();

    // 计算主屏幕fps0
    frameCount3 ++;
    now3 = millis();
    if (now3 - Millis3rd >= 2000){
      fps3 = frameCount3/2;
      frameCount3 = 0;   
      Millis3rd = now0;      
    }      
  } 
}

// Task2code: 显示1、2屏幕信息
void Task2code( void * pvParameters ){

  for(;;){
    vTaskDelay(1);
    // Serial.print("Task2 running on core ");
    // Serial.println(xPortGetCoreID());


    push_u8g1();
    // adjust_brightness_small();

    now1 = millis();
    


    frameCount1 ++;
    if (now1 - Millis1st >= 1000){
      fps1 = frameCount1;
      frameCount1 = 0;   
      Millis1st = now1;
      push_u8g2();
      
    }
  }
}

void loop(void) {  
  
} // void loop
