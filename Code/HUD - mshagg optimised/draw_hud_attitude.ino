void draw_hud_attitude(){

  get_mavlink_data();
  cal_att_cord();

  for (int i=0; i < 7; i++){
    for (int j=0; j < 8; j++){
      if ((j % 2) == 0){
        hud_att_points[i][j] = att_points[i][j] * hud_scale_x;
      } else{
        hud_att_points[i][j] = att_points[i][j] * hud_scale_y;
      }
    }
  }
  
  xb1_h = xb1*hud_scale_x; 
  xb2_h = xb2*hud_scale_x;
  xb3_h = xb3*hud_scale_x;
  xb4_h = xb4*hud_scale_x;
  yb1_h = yb1*hud_scale_y;
  yb2_h = yb2*hud_scale_y;
  yb3_h = yb3*hud_scale_y;
  yb4_h = yb4*hud_scale_y;

  for (int i=1; i < 6; i++) {
    // Angle of the attitude line closest to the attitude indicator centre
    draw_level_angle = base_vect*level_angle+(i-3)*level_angle; // i - 3 gives attitude line offsets from -2 to 2; offset 0 is closest to the attitude indicator centre.
    
    itoa(draw_level_angle, level_char, 10);        
    // Check whether the left half of the long 0-degree horizon line is within the canvas bounds
    if ((draw_level_angle == 0) && (xb1_h>1) && (xb1_h<hud_x_limit) && (xb2_h>1) && (xb2_h<hud_x_limit) && (yb1_h>1) && (yb1_h<hud_y_limit) && (yb2_h>1)  && (yb2_h<hud_y_limit)){
      u8g3.drawLine(xb1_h, yb1_h, xb2_h, yb2_h); // Draw a long 0-degree horizon line
    } else {
        if((hud_att_points[i][0]>1) && (hud_att_points[i][0]<hud_x_limit) && (hud_att_points[i][1]>1)  && (hud_att_points[i][1]<hud_y_limit) && (hud_att_points[i][2]>1) && (hud_att_points[i][2]<hud_x_limit)  && (hud_att_points[i][3]>1) && (hud_att_points[i][3]<hud_y_limit) ) {
          u8g3.drawLine(hud_att_points[i][0], hud_att_points[i][1], hud_att_points[i][2], hud_att_points[i][3]);
        }       
      } //else

    // Check whether the right half of the long 0-degree horizon line is within the canvas bounds
    if ((draw_level_angle == 0) && (xb3_h>1) && (xb4_h>1) && (xb3_h<hud_x_limit) && (xb4_h<hud_x_limit) && (yb3_h>1) && (yb4_h>1) && (yb3_h<hud_y_limit) && (yb4_h<hud_y_limit) ){
      u8g3.drawLine(xb3_h, yb3_h, xb4_h, yb4_h); // Draw a long 0-degree horizon line
      } else {
        if((hud_att_points[i][4]>1) && (hud_att_points[i][4]<hud_x_limit) && (hud_att_points[i][5]>1)  && (hud_att_points[i][5]<hud_y_limit) && (hud_att_points[i][6]>1) && (hud_att_points[i][6]<hud_x_limit)  && (hud_att_points[i][7]>1) && (hud_att_points[i][7]<hud_y_limit) )  {          
          u8g3.drawLine(hud_att_points[i][4], hud_att_points[i][5], hud_att_points[i][6], hud_att_points[i][7]);
          
        }
      }//else  

    // Pitch angle labels
    if (draw_level_angle == 0 ){
      u8g3.setCursor((hud_att_points[i][0]+hud_att_points[i][6])/2 - 5, (hud_att_points[i][1]+hud_att_points[i][7])/2 + 5);

      u8g3.println(level_char);    
    }
    if (draw_level_angle > 0 ){
      u8g3.setCursor((hud_att_points[i][0]+hud_att_points[i][6])/2 - 10, (hud_att_points[i][1]+hud_att_points[i][7])/2 + 5);
      u8g3.println(level_char);    
    }
    if (draw_level_angle < 0 ){
      u8g3.setCursor((hud_att_points[i][0]+hud_att_points[i][6])/2 - 13, (hud_att_points[i][1]+hud_att_points[i][7])/2 + 5);
      u8g3.println(level_char);    
    }

  } // for loop

// Draw the screen centre reference point
  u8g3.setDrawColor(0);
  u8g3.drawDisc(64, 64, 5);
  u8g3.setDrawColor(1);
  u8g3.drawLine(60, 64, 68, 64);
  u8g3.drawLine(64, 60, 64, 68);

}