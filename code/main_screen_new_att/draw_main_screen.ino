void draw_meters(){

  spr_meters.fillSprite(BLACK); 
  spr_meters.setTextFont(4);
  spr_meters.setTextColor(WHITE, BLACK);

  // Satellite icon
  spr_meters.pushImage(0, 10, 45, 43, salt_45_43); 
  spr_meters.setCursor(48, 15); 
  spr_meters.print(satellites_visible);

  // HDOP
  spr_meters.setTextFont(4);
  spr_meters.setTextSize(1);

  const int hdopX = 90;   // align with second column of gauges
  const int hdopY = 4;

  spr_meters.setCursor(hdopX, hdopY);
  spr_meters.print("HDOP");

  spr_meters.setTextFont(4);
  spr_meters.setCursor(hdopX, hdopY + 30);

  if (eph == 65535 || eph == 0) {
  spr_meters.print("--");
  } else {
  float hdop = eph / 100.0f;
  spr_meters.print(hdop, 1);
}

spr_meters.setTextFont(4);

  int x_start = 0;
  int y_start = 0;

  // Draw the instrument panel background: an 84x81 area containing a 78x78 filled square
  for (int j = 1; j < 3; j++){
    spr_meters.fillSmoothRoundRect(x_start,j*81+y_start, 78, 78, 6, GRAY, TFT_BLACK);
    spr_meters.fillSmoothRoundRect(x_start+84,j*81+y_start, 78, 78, 6, GRAY, TFT_BLACK);
  }  

  // System status text box
  spr_meters.fillSmoothRoundRect(x_start+84+3, 2*81+y_start+5, 72, 40, 6, TFT_BLACK, GRAY);

  //Ground Speed Dial
  spr_meters.setCursor(x_start+10, 1*81+y_start+30); spr_meters.print(groundspeed*3.6, 0); //Gspeed to km/h
  spr_meters.drawSmoothArc(x_start+38, 1*81+y_start+40, 36, 33, 60, 300, BLACK, BLACK, true);
  spr_meters.drawSmoothArc(x_start+38, 1*81+y_start+40, 36, 33, 60, int32_t(60+groundspeed*240/150+1), YELLOW, BLACK, true); //Max Speed 150km/h

  //Altitude Dial
  if (alt > 100) {alt = 100;}
  if (alt < 0) {alt = 0;}
  spr_meters.setCursor(x_start+10, 2*81+y_start+30); spr_meters.print(alt, 0); 
  spr_meters.drawSmoothArc(x_start+38, 2*81+y_start+40, 36, 33, 60, 300, BLACK, BLACK, true);
  spr_meters.drawSmoothArc(x_start+38, 2*81+y_start+40, 36, 33, 60, int32_t(60+alt*240/100+1), YELLOW, BLACK, true); //Altitude capped at 100m

  //Voltage Dial
  if (voltage_battery < 19200) {voltage_battery = 19200;}
  spr_meters.setCursor(x_start+10+84, 1*81+y_start+30); spr_meters.print(double(voltage_battery)/6000, 1); //6s电池，Per-cell voltage for 6S
  spr_meters.drawSmoothArc(x_start+38+84, 1*81+y_start+40, 36, 33, 60, 300, BLACK, BLACK, true);
  spr_meters.drawSmoothArc(x_start+38+84, 1*81+y_start+40, 36, 33, 60, int32_t(60+(voltage_battery-19200)*240/6000+1), YELLOW, BLACK, true);// Max volts 25.2v

  //Text for dials
  spr_meters.setTextFont(4);     
  spr_meters.drawString("GSPD",x_start+12, 1*81+y_start+55); // Calculate coordinates starting from the top-left corner
  spr_meters.drawString("ALT",x_start+12, 2*81+y_start+55); // Calculate coordinates starting from the top-left corner
  spr_meters.drawString("VOLT",x_start+12+84, 1*81+y_start+55); // Calculate coordinates starting from the top-left corner
  spr_meters.drawString("SYS",x_start+12+84, 2*81+y_start+55); // Calculate coordinates starting from the top-left corner
  spr_meters.drawString(system_status_str, x_start+84+3, 2*81+y_start+15); //System status
}

void draw_mid(){
  spr_mid.setTextFont(4);
  spr_mid.fillSprite(BLACK); 
  spr_mid.setSwapBytes(true); // Fix incorrect colours when using pushImage

  //Throttle bar
  spr_mid.drawRect(71, 185, 100, 20, GREEN);
  spr_mid.fillRect(71, 185, throttle, 20, GREEN);
  
  //Battery bar
  spr_mid.drawRect(71, 215, 100, 20,GREEN);
  spr_mid.fillRect(71+current_consumed*100/4000, 215, 100-current_consumed*100/4000, 20, GREEN);

  spr_mid.setTextColor(WHITE, BLACK);
  spr_mid.setTextFont(4);
  spr_mid.drawString("THR",13,185); //从左上角开始计算坐标
  spr_mid.drawString("BAT",13,215); //从左上角开始计算坐标
  
  drawNavStatusPanel();
  drawFlapIndicator();
}

