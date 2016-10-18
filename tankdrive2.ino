#include <SoftwareSerial.h> //Lines 1-4 concern libraries and are nessecary
#include <dfr_tank.h> 
#include "enes100.h"
DFRTank tank;

SoftwareSerial mySerial(8, 9); //RX is pin 8, TX is pin 9
enes100::RfClient<SoftwareSerial> rf(&mySerial); 
enes100::Marker marker;
#define markerNumber 115 //update this with whatever black and white marker you receive

void line() {
  rf.sendMessage("\n");
}

void getpos() { //This is the routine that is called to get the position of the tank many times throughout the code.
  do  { //This runs the builtin function that fills the .marker structure with x,y,and theta. 
    rf.sendMessage("\nThe RF failed to get the position.\n"); //If it didn't work, it will print this message and try again.
  } while ((!rf.receiveMarker(&marker, markerNumber)));
 rf.sendMessage("\nGot my position.\n"); //This will tell you if it worked or not.
}

void die() { //This is the routine that the tank executes when it gets to the water (ideally...)
  while(true) { //Wait forever
      tank.turnOffMotors(); 
      rf.sendMessage("\nSplashdown!\n"); rf.sendMessage(marker.x); rf.sendMessage(marker.y);
      delay(30000);
    }
  }
  
void orient() { //This is the routine that the tank uses to get itself pointed in the correct direction towards the water bowl.
  getpos(); //Gets its position 
  float d_x = (2.9 - marker.x); float d_y = (.35 - marker.y); //Figure out distances from position. 
  float theta_req = atan2(d_y,d_x); //Figure out required angle by using arctan. This puts out a value between -pi and pi.. Make sure this works
  float d_theta = theta_req - marker.theta;
  int dir;
  if (d_theta < 0)
  { //declare dir as -1 for case switch
    dir = -1;
  }
  else 
  { //declare dir as 1 for case switch
    dir = 1;  
  }

  switch (dir) {
    case -1:
    line(); rf.sendMessage("Turning Clockwise...");
    while(d_theta < -.15)
    {
      getpos();
      d_theta = theta_req - marker.theta;
      tank.setLeftMotorPWM(250); 
      tank.setRightMotorPWM(-250);
      delay(75);
      tank.turnOffMotors();
      delay(1500);
      line(); rf.sendMessage(d_theta);
    }
      break;
      
    case 1:
      line(); rf.sendMessage("Turning anti-clockwise...");
      while(d_theta > .15)
    {
      getpos();
      d_theta = theta_req - marker.theta;
      tank.setLeftMotorPWM(-250); 
      tank.setRightMotorPWM(250);
      delay(75);
      tank.turnOffMotors();
      delay(1500);
      line(); rf.sendMessage(d_theta);
     } 
      break;
  }
} 

void setup()
{
  mySerial.begin(9600); //this establishes serial communication with
                        //something other than serial monitor, in this
                        //case RF communication with mission control
  Serial.begin(9600); //this establishes regular serial communication
                      //through USB to student's serial monitor
  pinMode(8, INPUT); //since pin 8 is RX, it receives as an input
  pinMode(9, OUTPUT); //since pin 9 is TX, it transmits as an output
  rf.resetServer(); //Nessecary to reset connections.
  rf.sendMessage("\nWetWare is Connected\n"); //sent to mission control
  tank.init();
  getpos(); //get position
  orient(); //run orient code
}

void loop() {
  getpos(); //get position
  float d_x = (2.9 - marker.x); 
  float d_y = (.35 - marker.y); //Figure out distances from position. 
  float distance = sqrt(sq(d_x) + sq(d_y)); //Gets distance from target.
  line(); rf.sendMessage(marker.x);
  line(); rf.sendMessage(d_x); line(); rf.sendMessage(d_y); line(); rf.sendMessage(distance);
  if (distance < .15) { //Within some arbitrary threshold. You may have to play with this.
    die(); //Runs that die code.
  }
  tank.setLeftMotorPWM(250); tank.setRightMotorPWM(250); 
  delay(250); 
  tank.turnOffMotors();
  delay(500);
  orient(); 
}
