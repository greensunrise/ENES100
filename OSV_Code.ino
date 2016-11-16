/* OSV Code- Rev. 1
 *  Noah Eckman, Oct 27 2016
 */

#define marker_number 104
//Define Pin Numbers
#define L_PWM 6

#define RIGHT_FORWARD 7 
#define RIGHT_BACKWARD 4 //Forward

#define R_PWM 10

#define LEFT_BACKWARD 8
#define LEFT_FORWARD 9 

#define ELEV_PWM 3
#define ELEV_S 0
#define ELEV_R 1

#define SYR_PWM 5
#define SYR_S 2

#define trigPin 11
#define echoPin 12

//Include Libraries
#include "enes100.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(18, 19); 
enes100::RfClient<SoftwareSerial> rf(&mySerial); 
enes100::Marker marker;


void setup() {
  mySerial.begin(9600); 
  pinMode(18, INPUT); 
  pinMode(19, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT); pinMode(LEFT_BACKWARD, OUTPUT); pinMode(L_PWM, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT); pinMode(RIGHT_BACKWARD, OUTPUT); pinMode(R_PWM, OUTPUT);
  rf.resetServer();
 
  rf.sendMessage("\nInitiated RF Contact \n"); //sent to mission control
  get_pos(marker_number);
  rf.sendMessage("marker_x, marker_y, marker_theta are..."); line(); 
  rf.sendMessage(marker.x); line(); rf.sendMessage(marker.y); line(); rf.sendMessage(marker.theta); line();



  if(marker.y > .4) {
    rf.sendMessage("Greater than y = .4, orienting to -pi/2...");
    orient((-PI/2)); line(); 
    rf.sendMessage("Going to y = .4");
    goto_y(.4, (-PI/2));
  }
  else {
    orient((PI/2));
    goto_y(.4, (PI/2)); 
    orient((-PI/2));
  }
  
  if(is_board_present()) {
    rf.sendMessage("Board is present");
    orient((PI/2)); 
    goto_y(1.7, (PI/2));
    orient(0);
    goto_x(1.2, 0);
    orient(-PI/2);
    goto_y(.4, (-PI/2));
    orient(0);
  }
rf.sendMessage("Board is absent");
goto_x(2.9, 0);

//lower elevator
float depth = water_depth();
line();
rf.sendMessage(depth);
salinity();
//collect water
}




void loop() {
  go_forward();
}

