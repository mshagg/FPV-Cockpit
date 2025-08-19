void push_main_screen()
{ //main screen resolution 536x240
  get_mavlink_data();
  cal_att_cord();
  draw_new_attitude();
  amoled.pushColors(186, 0, 165, 240, (uint16_t*)spr.getPointer());

  now0 = millis();
    if (now0 - Millis0 >= 500){
      draw_meters();
      amoled.pushColors(0, 0, 171, 240, (uint16_t*)spr_meters.getPointer());
      Millis0 = now0;
    }      

    if (now0 - Millis2nd >= 25){
      draw_mid();
      amoled.pushColors(365, 0, 171, 240, (uint16_t*)spr_mid.getPointer());  
      Millis2nd = now0;  
    }      
}

// 上方小屏数据 64x48
void push_u8g1()
{
  u8g1.clearBuffer();
  u8g1.setFont(u8g2_font_wqy13_t_gb2312b);
  u8g1.setCursor(2, 30);   
  u8g1.print("空速 ");
  u8g1.print(airspeed*3.6, 0);
  // u8g1.print("RC10: ");
  // u8g1.print(chan10_raw);
  u8g1.setCursor(2, 47);   
  u8g1.print("动力 ");
  u8g1.print(double(current_battery)/100, 1);

  // u8g1.print("时间");
  // u8g1.print(time_str);

  // u8g1.print("fps: ");
  // u8g1.print(fps1);


  
  u8g1.sendBuffer();
}

//下方小屏数据 72X40
void push_u8g2()
{
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15); 
  u8g2.print("飞行模式:");
  u8g2.setCursor(0, 35);
  u8g2.print(flight_mode_str);
  u8g2.sendBuffer();
}


void adjust_brightness_main(){
  brightness_new_main = (chan10_raw-1000)*255/1000; //only the integral part is stored and the fractional part of the number is lost
  if (abs(brightness_new_main - brightness_old_main) > 20){
    amoled.setBrightness(brightness_new_main);
    brightness_old_main = brightness_new_main;
  }
}

void adjust_brightness_small(){
  brightness_new = (chan10_raw-1000)*255/1000; //only the integral part is stored and the fractional part of the number is lost
  if (abs(brightness_new - brightness_old) > 8){
    u8g1.setContrast(brightness_new);
    u8g2.setContrast(brightness_new/4);
    brightness_old = brightness_new;
  }
}