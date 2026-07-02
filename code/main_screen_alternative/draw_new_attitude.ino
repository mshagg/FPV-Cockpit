void draw_new_attitude(){

  // Erase the previous attitude line
  spr.fillRect(1,1,x_limit-1,y_limit-1,GREEN);  
  spr.fillTriangle(xb5,yb5,xb6,yb6,xb7,yb7,BLUE);
  spr.fillTriangle(xb8,yb8,xb6,yb6,xb7,yb7,BLUE);
  spr.setTextFont(1);
  spr.setTextSize(2);
    
  // Draw new attitude lines for i = 0 to 6

  spr.drawRect(0,0,x_limit,y_limit,YELLOW);// Draw boxes around the attitude lines
  spr.fillTriangle(x_center, y_center,x_center-35,y_center+9,x_center+35,y_center+9, RED); // Draw the aircraft marker
  
  for (int i=1; i < 6; i++) {
    // Angle of the attitude line closest to the attitude indicator centre
    draw_level_angle = base_vect*level_angle+(i-3)*level_angle; // i - 3 gives attitude line offsets -2 to 2; offset 0 is closest to the attitude indicator centre.
    if (draw_level_angle < 0){
      color = BLACK;      
    } else{
      color = WHITE;
    }
    
    itoa(draw_level_angle, level_char, 10);        
    // Check left half of long 0-degree horizon line is within canvas bounds
    if ((draw_level_angle == 0) && (xb1>1) && (xb1<x_limit) && (xb2>1) && (xb2<x_limit) && (yb1>1) && (yb1<y_limit) && (yb2>1)  && (yb2<y_limit)){
      spr.drawWideLine(xb1, yb1, xb2, yb2, 4, color); // Draw a long 0-degree horizon line
    } else {
        if((att_points[i][0]>1) && (att_points[i][0]<x_limit) && (att_points[i][1]>1)  && (att_points[i][1]<y_limit) && (att_points[i][2]>1) && (att_points[i][2]<x_limit)  && (att_points[i][3]>1) && (att_points[i][3]<y_limit) ) {
          spr.drawWideLine(att_points[i][0], att_points[i][1], att_points[i][2], att_points[i][3], 4, color);
        }       
      }

    // Check right half of long 0-degree horizon line is within canvas bounds
    if ((draw_level_angle == 0) && (xb3>1) && (xb4>1) && (xb3<x_limit) && (xb4<x_limit) && (yb3>1) && (yb4>1) && (yb3<y_limit) && (yb4<y_limit) ){
      spr.drawWideLine(xb3, yb3, xb4, yb4, 4, color); // Draw a long 0-degree horizon line
      } else {
        if((att_points[i][4]>1) && (att_points[i][4]<x_limit) && (att_points[i][5]>1)  && (att_points[i][5]<y_limit) && (att_points[i][6]>1) && (att_points[i][6]<x_limit)  && (att_points[i][7]>1) && (att_points[i][7]<y_limit) )  {          
          spr.drawWideLine(att_points[i][4], att_points[i][5], att_points[i][6], att_points[i][7], 4, color);
        }
      }  

    //Pitch Angle Labels
    if (draw_level_angle == 0 ){
      spr.setCursor((att_points[i][0]+att_points[i][6])/2 - 5, (att_points[i][1]+att_points[i][7])/2 - 10);
      spr.setTextColor(color);
      spr.println(level_char);    
    }
    if (draw_level_angle > 0 ){
      spr.setCursor((att_points[i][0]+att_points[i][6])/2 - 10, (att_points[i][1]+att_points[i][7])/2 - 10);
      spr.setTextColor(color);
      spr.println(level_char);    
    }
    if (draw_level_angle < 0 ){
      spr.setCursor((att_points[i][0]+att_points[i][6])/2 - 15, (att_points[i][1]+att_points[i][7])/2 - 10);
      spr.setTextColor(color);
      spr.println(level_char);    
    }

  } 
   
}


