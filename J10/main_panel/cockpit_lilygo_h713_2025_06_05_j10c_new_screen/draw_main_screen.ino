void draw_meters(){

  spr_meters.fillSprite(BLACK); 
  spr_meters.loadFont(Final_Frontier_28);
  spr_meters.setTextColor(WHITE, BLACK);

  //卫星图标
  spr_meters.pushImage(0, 10, 45, 43, salt_45_43); 
  spr_meters.setCursor(48, 15); 
  spr_meters.print(satellites_visible);

  //时间
  sprintf(time_str, "%02d:%02d", time_boot_ms/1000/60, time_boot_ms/1000%60);
  spr_meters.drawString(time_str, 90, 15);

  //fps3 主屏幕fps
  spr_meters.setCursor(48, 50);
  spr_meters.print("fps ");
  spr_meters.print(fps3); 
  spr_meters.print("/");
  spr_meters.print(fps1);  //39/17

  int x_start = 0;
  int y_start = 0;

  //画出仪表框背景， 宽高84*81的空间，里面放置78x78的方形填充
  for (int j = 1; j < 3; j++){
    spr_meters.fillSmoothRoundRect(x_start,j*81+y_start, 78, 78, 6, GRAY, TFT_BLACK);
    spr_meters.fillSmoothRoundRect(x_start+84,j*81+y_start, 78, 78, 6, GRAY, TFT_BLACK);
  }  

  //系统状态文字框
  spr_meters.fillSmoothRoundRect(x_start+84+3, 2*81+y_start+5, 72, 40, 6, TFT_PURPLE, GRAY);

  //地速
  spr_meters.setCursor(x_start+10, 1*81+y_start+30); spr_meters.print(groundspeed*3.6, 0); //地速换算成km/h
  spr_meters.drawSmoothArc(x_start+38, 1*81+y_start+40, 36, 33, 60, 300, BLACK, BLACK, true);
  spr_meters.drawSmoothArc(x_start+38, 1*81+y_start+40, 36, 33, 60, int32_t(60+groundspeed*240/150+1), YELLOW, BLACK, true); //预计速度最高不超过150km/h

  //高度
  if (alt > 100) {alt = 100;}
  if (alt < 0) {alt = 0;}
  spr_meters.setCursor(x_start+10, 2*81+y_start+30); spr_meters.print(alt, 0); 
  spr_meters.drawSmoothArc(x_start+38, 2*81+y_start+40, 36, 33, 60, 300, BLACK, BLACK, true);
  spr_meters.drawSmoothArc(x_start+38, 2*81+y_start+40, 36, 33, 60, int32_t(60+alt*240/100+1), YELLOW, BLACK, true); //高度显示最多100m

  //电压
  if (voltage_battery < 19200) {voltage_battery = 19200;}
  spr_meters.setCursor(x_start+10+84, 1*81+y_start+30); spr_meters.print(double(voltage_battery)/6000, 1); //6s电池，计算每节电池电压
  spr_meters.drawSmoothArc(x_start+38+84, 1*81+y_start+40, 36, 33, 60, 300, BLACK, BLACK, true);
  spr_meters.drawSmoothArc(x_start+38+84, 1*81+y_start+40, 36, 33, 60, int32_t(60+(voltage_battery-19200)*240/6000+1), YELLOW, BLACK, true);//最高电压按6s电池25.2V预估

  //集中输出中文文字
  spr_meters.loadFont(cn_ht_26);     
  spr_meters.drawString("地速",x_start+12, 1*81+y_start+55); //从左上角开始计算坐标
  spr_meters.drawString("高度",x_start+12, 2*81+y_start+55); //从左上角开始计算坐标
  spr_meters.drawString("电压",x_start+12+84, 1*81+y_start+55); //从左上角开始计算坐标  
  spr_meters.drawString("系统",x_start+12+84, 2*81+y_start+55); //从左上角开始计算坐标  
  spr_meters.drawString(system_status_str, x_start+84+3, 2*81+y_start+15); //系统状态
  spr_meters.unloadFont();
}

void draw_mid(){
  spr_mid.loadFont(Final_Frontier_28);
  spr_mid.fillSprite(BLACK); 
  spr_mid.setSwapBytes(true); //纠正pushimage颜色不对的问题

  //油门状态条
  spr_mid.drawRect(71, 185, 100, 20, GREEN);
  spr_mid.fillRect(71, 185, throttle, 20, GREEN);
  
  //current_consumed 电量状态条，4000mah为100%
  spr_mid.drawRect(71, 215, 100, 20,GREEN);
  spr_mid.fillRect(71+current_consumed*100/4000, 215, 100-current_consumed*100/4000, 20, GREEN);

  spr_mid.setTextColor(WHITE, BLACK);
  spr_mid.loadFont(cn_ht_26);
  spr_mid.drawString("油门",13,185); //从左上角开始计算坐标
  spr_mid.drawString("电量",13,215); //从左上角开始计算坐标

  spr_mid.unloadFont();

  //雷达图
  drawRadar();
}

