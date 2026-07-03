void drawClassicAttitudeIndicator()
{
  // Tune these for your centre panel
  const int cx = 82;
  const int cy = 95;
  const int radius = 76;

  const float pixelsPerDeg = 3.0f;

  uint16_t skyColor    = spr.color565(0, 75, 230);
  uint16_t groundColor = spr.color565(145, 85, 10);
   uint16_t panelGrey   = spr.color565(45, 45, 45);
  uint16_t bezelColor  = TFT_BLACK;
  uint16_t whiteColor  = TFT_WHITE;
  uint16_t yellowColor = spr.color565(230, 220, 40);

  // Grey square around the dial only
  const int panelPad = 10;
  const int panelX = cx - radius - panelPad;
  const int panelY = cy - radius - panelPad;
  const int panelW = (radius + panelPad) * 2;
  const int panelH = (radius + panelPad) * 2;

  // Black outside the instrument panel
  //spr.fillSprite(TFT_BLACK);

  // Tight grey panel behind the dial
  spr.fillRect(panelX, panelY, panelW, panelH, panelGrey);

  float rollRad = -roll;                 // reverse if roll direction is wrong
  float pitchDeg = pitch * RAD_TO_DEG;
  float pitchOffset = pitchDeg * pixelsPerDeg;

  // Limit extreme pitch so the display does not go completely wild
  pitchOffset = constrain(pitchOffset, -radius * 1.5f, radius * 1.5f);

  drawArtificialHorizonFill(cx, cy, radius, rollRad, pitchOffset, skyColor, groundColor);
  drawPitchLadder(cx, cy, radius, rollRad, pitchOffset, pixelsPerDeg, whiteColor);

  // Erase any pitch ladder pixels that escaped outside the circular face
  clearOutsideCircleInRect(cx, cy, radius - 2, panelGrey, panelX, panelY, panelW, panelH);

    // Draw panel screws
  drawPanelScrews(panelX, panelY, panelW, panelH);

  drawRollScale(cx, cy, radius, whiteColor);
  drawAircraftSymbol(cx, cy, yellowColor, bezelColor);


  // Outer bezel last
  spr.drawCircle(cx, cy, radius + 2, whiteColor);
  spr.drawCircle(cx, cy, radius + 3, whiteColor);
  spr.drawCircle(cx, cy, radius + 4, bezelColor);
}

void drawArtificialHorizonFill(
  int cx,
  int cy,
  int radius,
  float rollRad,
  float pitchOffset,
  uint16_t skyColor,
  uint16_t groundColor
) {
  float ux = cos(rollRad);
  float uy = sin(rollRad);

  // Normal vector pointing generally downward when roll = 0
  float nx = -uy;
  float ny = ux;

  for (int dy = -radius; dy <= radius; dy++) {
    int xSpan = sqrt((radius * radius) - (dy * dy));

    int xStart = cx - xSpan;
    int xEnd   = cx + xSpan;
    int y      = cy + dy;

    // Horizon equation:
    // value < 0 = sky
    // value >= 0 = ground
    if (abs(nx) < 0.0001f) {
      float value = ny * (dy - pitchOffset);
      uint16_t colour = (value < 0) ? skyColor : groundColor;
      spr.drawFastHLine(xStart, y, xEnd - xStart + 1, colour);
    } else {
      float splitX = cx - (ny * (dy - pitchOffset)) / nx;

      if (nx > 0) {
        // Sky left of split, ground right of split
        if (splitX <= xStart) {
          spr.drawFastHLine(xStart, y, xEnd - xStart + 1, groundColor);
        } else if (splitX >= xEnd) {
          spr.drawFastHLine(xStart, y, xEnd - xStart + 1, skyColor);
        } else {
          int sx = (int)splitX;
          spr.drawFastHLine(xStart, y, sx - xStart + 1, skyColor);
          spr.drawFastHLine(sx + 1, y, xEnd - sx, groundColor);
        }
      } else {
        // Ground left of split, sky right of split
        if (splitX <= xStart) {
          spr.drawFastHLine(xStart, y, xEnd - xStart + 1, skyColor);
        } else if (splitX >= xEnd) {
          spr.drawFastHLine(xStart, y, xEnd - xStart + 1, groundColor);
        } else {
          int sx = (int)splitX;
          spr.drawFastHLine(xStart, y, sx - xStart + 1, groundColor);
          spr.drawFastHLine(sx + 1, y, xEnd - sx, skyColor);
        }
      }
    }
  }

  // Draw the actual horizon line
  int lineLen = radius + 20;

  int x1 = cx - ux * lineLen + nx * pitchOffset;
  int y1 = cy - uy * lineLen + ny * pitchOffset;
  int x2 = cx + ux * lineLen + nx * pitchOffset;
  int y2 = cy + uy * lineLen + ny * pitchOffset;

  spr.drawLine(x1, y1, x2, y2, TFT_WHITE);
}

void drawPitchLadder(
  int cx,
  int cy,
  int radius,
  float rollRad,
  float pitchOffset,
  float pixelsPerDeg,
  uint16_t colour
) {
  float ux = cos(rollRad);
  float uy = sin(rollRad);

  float nx = -uy;
  float ny = ux;

  spr.setTextColor(colour);
  spr.setTextFont(2);
  spr.setTextSize(1);

  for (int deg = -30; deg <= 30; deg += 5) {
    if (deg == 0) continue;

    float lineOffset = pitchOffset - deg * pixelsPerDeg;

    // Skip lines well outside the instrument
    if (lineOffset < -radius - 20 || lineOffset > radius + 20) continue;

    int halfLen = (deg % 10 == 0) ? 26 : 14;

    int mx = cx + nx * lineOffset;
    int my = cy + ny * lineOffset;

    int x1 = mx - ux * halfLen;
    int y1 = my - uy * halfLen;
    int x2 = mx + ux * halfLen;
    int y2 = my + uy * halfLen;

    spr.drawLine(x1, y1, x2, y2, colour);

    // Add labels only on 10-degree marks
    if (deg % 10 == 0) {
      int labelValue = abs(deg);

      int lx1 = x1 - 18;
      int ly1 = y1 - 4;
      int lx2 = x2 + 6;
      int ly2 = y2 - 4;

      // Only draw labels if roughly inside the circular face
      if (pointNearCircle(cx, cy, lx1, ly1, radius)) {
        spr.setCursor(lx1, ly1);
        spr.print(labelValue);
      }

      if (pointNearCircle(cx, cy, lx2, ly2, radius)) {
        spr.setCursor(lx2, ly2);
        spr.print(labelValue);
      }
    }
  }
}

void drawRollScale(int cx, int cy, int radius, uint16_t colour)
{
  for (int deg = -60; deg <= 60; deg += 10) {
    float a = deg * DEG_TO_RAD;

    int outerR = radius - 5;
    int innerR = (deg % 30 == 0) ? radius - 18 : radius - 12;

    int x1 = cx + sin(a) * outerR;
    int y1 = cy - cos(a) * outerR;
    int x2 = cx + sin(a) * innerR;
    int y2 = cy - cos(a) * innerR;

    spr.drawLine(x1, y1, x2, y2, colour);
  }

  // Top fixed roll pointer
  spr.fillTriangle(
    cx,     cy - radius + 9,
    cx - 7, cy - radius + 23,
    cx + 7, cy - radius + 23,
    colour
  );
}

void drawAircraftSymbol(int cx, int cy, uint16_t yellowColor, uint16_t outlineColor)
{
  // Black outline first
  spr.drawLine(cx - 55, cy,     cx - 14, cy,     outlineColor);
  spr.drawLine(cx + 14, cy,     cx + 55, cy,     outlineColor);
  spr.drawLine(cx - 14, cy,     cx,      cy + 7, outlineColor);
  spr.drawLine(cx,      cy + 7, cx + 14, cy,     outlineColor);

  // Yellow fixed aircraft wings
  spr.drawLine(cx - 55, cy,     cx - 18, cy,     yellowColor);
  spr.drawLine(cx - 18, cy,     cx,      cy + 7, yellowColor);
  spr.drawLine(cx,      cy + 7, cx + 18, cy,     yellowColor);
  spr.drawLine(cx + 18, cy,     cx + 55, cy,     yellowColor);

  // Thicken the symbol slightly
  spr.drawLine(cx - 55, cy + 1, cx - 18, cy + 1, yellowColor);
  spr.drawLine(cx + 18, cy + 1, cx + 55, cy + 1, yellowColor);

  // Small centre reference
  spr.drawCircle(cx, cy, 2, yellowColor);
}

bool pointNearCircle(int cx, int cy, int x, int y, int radius)
{
  int dx = x - cx;
  int dy = y - cy;
  return (dx * dx + dy * dy) < ((radius - 8) * (radius - 8));
}

void clearOutsideCircle(int cx, int cy, int radius, uint16_t backgroundColor)
{
  int w = spr.width();
  int h = spr.height();

  for (int y = 0; y < h; y++) {
    int dy = y - cy;

    if (abs(dy) > radius) {
      // Entire row is outside the circle
      spr.drawFastHLine(0, y, w, backgroundColor);
    } else {
      int xSpan = sqrt((radius * radius) - (dy * dy));

      int leftEdge  = cx - xSpan;
      int rightEdge = cx + xSpan;

      // Clear left side outside the circle
      if (leftEdge > 0) {
        spr.drawFastHLine(0, y, leftEdge, backgroundColor);
      }

      // Clear right side outside the circle
      if (rightEdge < w - 1) {
        spr.drawFastHLine(rightEdge + 1, y, w - rightEdge - 1, backgroundColor);
      }
    }
  }
}

void clearOutsideCircleInRect(
  int cx,
  int cy,
  int radius,
  uint16_t backgroundColor,
  int rectX,
  int rectY,
  int rectW,
  int rectH
) {
  int xMin = max(0, rectX);
  int xMax = min(spr.width() - 1, rectX + rectW - 1);
  int yMin = max(0, rectY);
  int yMax = min(spr.height() - 1, rectY + rectH - 1);

  for (int y = yMin; y <= yMax; y++) {
    int dy = y - cy;

    if (abs(dy) > radius) {
      spr.drawFastHLine(xMin, y, xMax - xMin + 1, backgroundColor);
    } else {
      int xSpan = sqrt((radius * radius) - (dy * dy));

      int circleLeft  = cx - xSpan;
      int circleRight = cx + xSpan;

      // Clear left side within the grey square
      if (circleLeft > xMin) {
        spr.drawFastHLine(xMin, y, circleLeft - xMin, backgroundColor);
      }

      // Clear right side within the grey square
      if (circleRight < xMax) {
        spr.drawFastHLine(circleRight + 1, y, xMax - circleRight, backgroundColor);
      }
    }
  }
}

void drawPanelScrews(int panelX, int panelY, int panelW, int panelH)
{
  uint16_t screwOuter = spr.color565(170, 170, 170);  // silver
  uint16_t screwInner = spr.color565(80, 80, 80);     // dark centre

  const int screwR = 3;
  const int inset = 8;

  int x1 = panelX + inset;
  int x2 = panelX + panelW - inset;
  int y1 = panelY + inset;
  int y2 = panelY + panelH - inset;

  spr.fillCircle(x1, y1, screwR, screwOuter);
  spr.fillCircle(x2, y1, screwR, screwOuter);
  spr.fillCircle(x1, y2, screwR, screwOuter);
  spr.fillCircle(x2, y2, screwR, screwOuter);

  spr.fillCircle(x1, y1, 1, screwInner);
  spr.fillCircle(x2, y1, 1, screwInner);
  spr.fillCircle(x1, y2, 1, screwInner);
  spr.fillCircle(x2, y2, 1, screwInner);
}

