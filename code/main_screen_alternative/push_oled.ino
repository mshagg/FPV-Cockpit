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

// Auxillary Display data
void push_u8g1()
{
  u8g1.clearBuffer();
  u8g1.setFont(u8g2_font_8x13B_tf);
  u8g1.setCursor(2, 21);   
  u8g1.print("SPD ");
  u8g1.print(airspeed*3.6, 1);
  u8g1.setCursor(2, 43);   
  u8g1.print("PWR ");
  u8g1.print(double(current_battery)/100, 1);
  u8g1.sendBuffer();
}

void adjust_brightness_main(){
  {
    amoled.setBrightness(255);
     }
}

void adjust_brightness_small(){
  {
    u8g1.setContrast(255);
      
  }
}