void drawNavStatusPanel() {
  const int panelX = 7;
  const int panelY = 6;
  const int panelW = 165;
  const int panelH = 148;

  const int cellW = panelW / 2;
  const int cellH = panelH / 2;

  char hdgText[8];
  char vsText[10];
  char rssiText[8];
  char fixText[8];

  int hdgValue = heading;
  if (hdgValue < 0) hdgValue += 360;
  if (hdgValue >= 360) hdgValue -= 360;

  snprintf(hdgText, sizeof(hdgText), " %03d", hdgValue);
  snprintf(vsText, sizeof(vsText), "%+.1f", climb);

  int rssiPct = rssi;
  if (rssi > 100 && rssi <= 254) {
    rssiPct = map(rssi, 0, 254, 0, 100);
  }
  rssiPct = constrain(rssiPct, 0, 100);
  snprintf(rssiText, sizeof(rssiText), "%d%%", rssiPct);

  getFixText(fix_type, fixText, sizeof(fixText));

  drawNavTile(panelX,         panelY,         cellW, cellH, " HDG",  hdgText);
  drawNavTile(panelX + cellW, panelY,         cellW, cellH, " VSP",   vsText);
  drawNavTile(panelX,         panelY + cellH, cellW, cellH, "RSSI", rssiText);
  drawNavTile(panelX + cellW, panelY + cellH, cellW, cellH, " FIX",  fixText);
}

void drawNavTile(int x, int y, int w, int h, const char *label, const char *value) {
  const int headerH = 31;

  // Outer tile border
  spr_mid.drawRect(x, y, w, h, GREEN);

  // Inverted label header
  spr_mid.fillRect(x + 1, y + 1, w - 2, headerH - 1, GREEN);

  spr_mid.setTextFont(4);
  spr_mid.setTextSize(1);

  // Label: inverted text
  spr_mid.setTextColor(BLACK, GREEN);
  spr_mid.setCursor(x + 5, y + 4);
  spr_mid.print(label);

  // Value: normal text
  spr_mid.setTextColor(GREEN, BLACK);
  spr_mid.setCursor(x + 5, y + 40);
  spr_mid.print(value);

  spr_mid.setTextFont(1);
  spr_mid.setTextSize(1);
}

void getFixText(uint8_t fix, char *buffer, size_t bufferSize) {
  switch (fix) {
    case 0:
    case 1:
      snprintf(buffer, bufferSize, " NO");
      break;
    case 2:
      snprintf(buffer, bufferSize, "2D");
      break;
    case 3:
      snprintf(buffer, bufferSize, "3D");
      break;
    case 4:
      snprintf(buffer, bufferSize, "DGPS");
      break;
    case 5:
      snprintf(buffer, bufferSize, "RTK");
      break;
    default:
      snprintf(buffer, bufferSize, "--");
      break;
  }
}