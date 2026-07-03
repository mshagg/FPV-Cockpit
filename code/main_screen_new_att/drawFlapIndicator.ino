void drawFlapIndicator()
{
  // 3-position flap indicator under artificial horizon
  const int x = 2;
  const int y = 186;
  const int w = 160;
  const int h = 42;

  uint16_t borderColor = TFT_WHITE;
  uint16_t inactiveBg  = spr.color565(35, 35, 35);
  uint16_t activeBg    = spr.color565(230, 220, 40);  // yellow
  uint16_t textNormal  = TFT_WHITE;
  uint16_t textActive  = TFT_BLACK;


int flapPct = map(chan6_raw, 1500, 1000, 0, 100);
flapPct = constrain(flapPct, 0, 100);

int flapPos = 0;  // 0 = UP, 1 = HALF, 2 = FULL

if (flapPct >= 75) {
  flapPos = 2;      // full flaps
} else if (flapPct >= 25) {
  flapPos = 1;      // half flaps
} else {
  flapPos = 0;      // flaps up
}

  // Background and border
  spr.fillRect(x, y, w, h, inactiveBg);
  spr.drawRect(x, y, w, h, borderColor);

  // Segment layout
   // Uneven segment layout: smaller UP segment
  const int segmentY = y + 1;
  const int segmentH = h - 2;

  const int upW = 38;
  const int halfW = (w - 2 - upW) / 2;
  const int fullW = w - 2 - upW - halfW;

  int upX   = x + 1;
  int halfX = upX + upW;
  int fullX = halfX + halfW;

  drawFlapSegmentLarge(upX,   segmentY, upW,   segmentH, "UP",   flapPos == 0, activeBg, inactiveBg, textActive, textNormal);
  drawFlapSegmentLarge(halfX, segmentY, halfW, segmentH, "1/2", flapPos == 1, activeBg, inactiveBg, textActive, textNormal);
  drawFlapSegmentLarge(fullX, segmentY, fullW, segmentH, "FULL", flapPos == 2, activeBg, inactiveBg, textActive, textNormal);
}

void drawFlapSegmentLarge(
  int x,
  int y,
  int w,
  int h,
  const char *label,
  bool active,
  uint16_t activeBg,
  uint16_t inactiveBg,
  uint16_t textActive,
  uint16_t textNormal
) {
  uint16_t bg = active ? activeBg : inactiveBg;
  uint16_t fg = active ? textActive : textNormal;

  spr.fillRect(x, y, w, h, bg);
  spr.drawRect(x, y, w, h, TFT_WHITE);

  spr.setTextFont(2);
  spr.setTextSize(2);
  spr.setTextColor(fg, bg);

  // Approximate font 4 character width: about 13 px
  int textW = strlen(label) * 13;
  int textH = 26;

  int textX = x + (w - textW) / 2;
  int textY = y + (h - textH) / 2;

  spr.setCursor(textX, textY);
  spr.print(label);
}