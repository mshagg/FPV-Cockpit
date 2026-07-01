//hud数据 128x128
void push_u8g3()
{
  u8g3.clearBuffer();
  draw_hud_attitude();
  u8g3.sendBuffer();
}

