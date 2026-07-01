// mavlink msg https://github.com/mavlink/c_library_v1/tree/master/common

void get_mavlink_data(){
  while (Serial.available() > 0) {
    // Avoid Serial output before Serial.read, as it can interfere with incoming serial data.
    uint8_t result = Serial.read(); // Read incoming serial data, assuming data is already available
    msgReceived = mavlink_parse_char(MAVLINK_COMM_1, result, &msg, &status);
    if (msgReceived) {
         switch (msg.msgid) {  
                
        case MAVLINK_MSG_ID_ATTITUDE:  // #30
          {
            mavlink_attitude_t attitude;
            mavlink_msg_attitude_decode(&msg, &attitude);
            pitch = attitude.pitch;
            roll = -attitude.roll;
            yaw = attitude.yaw;   
            break;                     
          }
                  
      }//switch (message.msgid)         
    }//if (msgReceived)    
  }//while (Serial.available() > 0)
}