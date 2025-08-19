void initRadar() {
  

  for (int i = 0; i < MAX_TARGETS; i++) {
    initTarget(i);
  }

  radarInitialized = true;
  scanAngle = 0;
}

void initTarget(int i) {
  float angle = random(0, 360) * DEG_TO_RAD;

  float r = (random(0, 2) == 0) ? RADAR_RADIUS : -RADAR_RADIUS; //控制目标出现的初始位置，只能在雷达边缘开始出现，而不是在雷达中心
  targets[i].x = r * cos(angle);
  targets[i].y = r * sin(angle);
  targets[i].dx = random(-20, 21) / 2.0;
  targets[i].dy = random(-20, 21) / 2.0;

  if (targets[i].dx == 0 && targets[i].dy == 0) {
    targets[i].dx = 0.5;
  }

  uint16_t color1 = tft.color565(0, 255, 255);   // Cyan 亮青色
  uint16_t color2 = tft.color565(255, 255, 0);   // Yellow 亮黄色
  uint16_t color3 = tft.color565(0, 128, 255);   // Blue 亮蓝色
  uint16_t colors[] = {color1, color2, color3};
  targets[i].color = colors[random(0, 3)];
}

bool isTargetInSweep(float tx, float ty, float scanDeg) {
  float angle = atan2(ty, tx) * 180 / PI;
  if (angle < 0) angle += 360;

  float diff = abs(scanDeg - angle);
  if (diff > 180) diff = 360 - diff;

  return diff <= SWEEP_WIDTH / 2;
}

void drawRadar() {
  if (!radarInitialized) {
    initRadar();
  }

  // Draw radar background
  spr_mid.drawCircle(RADAR_CENTER_X, RADAR_CENTER_Y, RADAR_RADIUS, TFT_GREEN);
  spr_mid.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y - RADAR_RADIUS,
                       RADAR_CENTER_X, RADAR_CENTER_Y + RADAR_RADIUS, TFT_DARKGREEN);
  spr_mid.drawLine(RADAR_CENTER_X - RADAR_RADIUS, RADAR_CENTER_Y,
                       RADAR_CENTER_X + RADAR_RADIUS, RADAR_CENTER_Y, TFT_DARKGREEN);

  // --- Draw fading sweep beam ---
  for (int i = -SWEEP_WIDTH / 2; i <= SWEEP_WIDTH / 2; i += 1) {
    float angleDeg = scanAngle + i;
    if (angleDeg >= 360) angleDeg -= 360;
    if (angleDeg < 0) angleDeg += 360;
    float rad = angleDeg * DEG_TO_RAD;
    int x = RADAR_CENTER_X + RADAR_RADIUS * cos(rad);
    int y = RADAR_CENTER_Y + RADAR_RADIUS * sin(rad);

    // 透明度模拟：角度越偏远颜色越淡
    uint8_t fade = 255 - abs(i) * (255 / SWEEP_WIDTH);
    uint16_t color = tft.color565(fade, fade, 0);  // 淡黄渐变
    spr_mid.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, x, y, color);
  }

  // --- Draw and move targets only if in beam ---
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (isTargetInSweep(targets[i].x, targets[i].y, scanAngle)) {
      targets[i].time_cur = millis();
      if (targets[i].time_cur - targets[i].time_pre > 500) {
        targets[i].x += targets[i].dx;
        targets[i].y += targets[i].dy;
        targets[i].time_pre = millis();
      }
      
    }
    //控制target消失或者出现
    float dist2 = targets[i].x * targets[i].x + targets[i].y * targets[i].y;
    if (abs(targets[i].x) >= RADAR_RADIUS or abs(targets[i].y) >= RADAR_RADIUS){  //如果target坐标超出园的外切正方形，就重新初始化这个target
      initTarget(i);
      continue;
    } else if (dist2 > (RADAR_RADIUS-4) * (RADAR_RADIUS-4)){ //如果target坐标接近圆形边缘，就不渲染这个target
      continue;
    }

    int drawX = RADAR_CENTER_X + targets[i].x;
    int drawY = RADAR_CENTER_Y + targets[i].y;
    spr_mid.fillCircle(drawX, drawY, 3, targets[i].color);
  }

  // Scan line (center of beam)
  float scanRad = scanAngle * DEG_TO_RAD;
  int scanX = RADAR_CENTER_X + RADAR_RADIUS * cos(scanRad);
  int scanY = RADAR_CENTER_Y + RADAR_RADIUS * sin(scanRad);
  spr_mid.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, scanX, scanY, TFT_YELLOW);

  scanAngle += 10;
  if (scanAngle >= 360) scanAngle -= 360;

}