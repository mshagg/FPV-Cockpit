void cal_att_cord(){

// Calculate pitch-adjusted r0 length
  r0 = pitch*pitch_to_pixel;
  alpha = HALF_PI + roll;
  beta = HALF_PI + alpha;
  cosa = cos(alpha);
  sina = sin(alpha);
  cosb = cos(beta);
  sinb = sin(beta);
    
  x0 = r0*cosa + x_center; // X-coordinate of the midpoint of the 0-degree attitude line
  y_0 = r0*sina + y_center; // Y-coordinate of the midpoint of the 0-degree attitude line
  vx = level_pixel*cosa;
  vy = level_pixel*sina;

  center_degree = (pitch)/PI*180; // Convert pitch from radians to degrees
  base_vect = round(center_degree/level_angle); // Number of attitude line intervals from x0/y_0 to the attitude indicator centre 
  // Coordinates of the 0-degree attitude line
  xa1 = x0+cosb*r1;
  ya1 = y_0+sinb*r1;
  xa2 = x0+cosb*r2;
  ya2 = y_0+sinb*r2;  
  xa3 = x0-cosb*r3;
  ya3 = y_0-sinb*r3;
  xa4 = x0-cosb*r4;
  ya4 = y_0-sinb*r4;

  // Calculate centre attitude line coordinates: att_points[3]
  att_points[3][0] = xa1-vx*base_vect;
  att_points[3][1] = ya1-vy*base_vect;
  att_points[3][2] = xa2-vx*base_vect;
  att_points[3][3] = ya2-vy*base_vect;
  att_points[3][4] = xa3-vx*base_vect;
  att_points[3][5] = ya3-vy*base_vect;
  att_points[3][6] = xa4-vx*base_vect;
  att_points[3][7] = ya4-vy*base_vect;
  
  // Calculate the coordinates of the other attitude lines based on the centre attitude line coordinates
  for (int i=0; i < 7; i++) {      
  int j = i-3;// j = distance from the centre attitude line. i = attitude line index: highest pitch line is 0, lowest is 6, centre line is 3.
  att_points[i][0] = att_points[3][0]-j*vx;   att_points[i][1] = att_points[3][1]-j*vy;     
  att_points[i][2] = att_points[3][2]-j*vx;   att_points[i][3] = att_points[3][3]-j*vy;  
  att_points[i][4] = att_points[3][4]-j*vx;   att_points[i][5] = att_points[3][5]-j*vy;
  att_points[i][6] = att_points[3][6]-j*vx;   att_points[i][7] = att_points[3][7]-j*vy;
  }  
 
  // Coordinates of the long 0-degree horizon line
  xb1 = x0+cosb*l1;
  yb1 = y_0+sinb*l1;
  xb2 = x0+cosb*l2;
  yb2 = y_0+sinb*l2;  
  xb3 = x0-cosb*l3;
  yb3 = y_0-sinb*l3;
  xb4 = x0-cosb*l4;  
  yb4 = y_0-sinb*l4;  
  
  // Sky box coordinates: b5, b6, b7, b8, with edges b5->b7 and b6->b8
  xb5 = x0+cosb*l0;
  yb5 = y_0+sinb*l0;
  xb6 = x0-cosb*l0;
  yb6 = y_0-sinb*l0;    
  xb7 = x0+cosb*l0-9*vx;
  yb7 = y_0+sinb*l0-9*vy;
  xb8 = x0-cosb*l0-9*vx;
  yb8 = y_0-sinb*l0-9*vy;  
 
}