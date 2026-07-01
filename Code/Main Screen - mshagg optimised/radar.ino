static float radarCos[360];
static float radarSin[360];
static int radarEdgeX[360];
static int radarEdgeY[360];
static float sweepCos2 = 0;
static bool radarLookupReady = false;

int normDeg(int deg) {
  while (deg >= 360) deg -= 360;
  while (deg < 0) deg += 360;
  return deg;
}

void initRadarLookup() {
  if (radarLookupReady) return;

  for (int a = 0; a < 360; a++) {
    float rad = a * DEG_TO_RAD;

    radarCos[a] = cos(rad);
    radarSin[a] = sin(rad);

    radarEdgeX[a] = RADAR_CENTER_X + RADAR_RADIUS * radarCos[a];
    radarEdgeY[a] = RADAR_CENTER_Y + RADAR_RADIUS * radarSin[a];
  }

  float halfSweepRad = (SWEEP_WIDTH / 2.0) * DEG_TO_RAD;
  float c = cos(halfSweepRad);
  sweepCos2 = c * c;

  radarLookupReady = true;
}

void initRadar() {
initRadarLookup();  

  for (int i = 0; i < MAX_TARGETS; i++) {
    initTarget(i);
  }

  radarInitialized = true;
  scanAngle = 0;
}

void initTarget(int i) {
  initRadarLookup();

  int angle = random(0, 360);
  float r = (random(0, 2) == 0) ? RADAR_RADIUS : -RADAR_RADIUS;

  targets[i].x = r * radarCos[angle];
  targets[i].y = r * radarSin[angle];
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

bool isTargetInSweep(float tx, float ty, int scanDeg) {
  scanDeg = normDeg(scanDeg);

  float dot = tx * radarCos[scanDeg] + ty * radarSin[scanDeg];

  if (dot <= 0) {
    return false;
  }

  float dist2 = tx * tx + ty * ty;

  return (dot * dot) >= (dist2 * sweepCos2);
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
  int angleDeg = normDeg(scanAngle + i);
  int x = radarEdgeX[angleDeg];
  int y = radarEdgeY[angleDeg];

    // Fade colour based on angle distance
    uint8_t fade = 255 - abs(i) * (255 / SWEEP_WIDTH);
    uint16_t color = tft.color565(fade, fade, 0);  // Light yellow fade
    spr_mid.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, x, y, color);
  }

  //Draw and move targets only if in beam 
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (isTargetInSweep(targets[i].x, targets[i].y, scanAngle)) {
      targets[i].time_cur = millis();
      if (targets[i].time_cur - targets[i].time_pre > 500) {
        targets[i].x += targets[i].dx;
        targets[i].y += targets[i].dy;
        targets[i].time_pre = millis();
      }
      
    }
    // Control whether the target appears or disappears
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
int scanDeg = normDeg(scanAngle);
spr_mid.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y,
                 radarEdgeX[scanDeg], radarEdgeY[scanDeg],
                 TFT_YELLOW);

  scanAngle += 10;
  if (scanAngle >= 360) scanAngle -= 360;

}