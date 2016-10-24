#include "Arduino.h"
#include "Morse.h"

wetware::wetware(){
} //??

void wetware::getpos(int mrknum)
{
	while((!rf.receiveMarker(&marker, mrknum))) {
	}
}


void wetware::orient(float theta, float targx, float targy) {
  getpos(); //Gets its position 
  float d_x = (targx - marker.x); float d_y = (targy - marker.y); //Figure out distances from position. 
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
      //make left go forward, right backward
      delay(75);
      //stop motors
      delay(1500);
    }
      break;
      
    case 1:
      line(); rf.sendMessage("Turning anti-clockwise...");
      while(d_theta > .15)
    {
      getpos();
      d_theta = theta_req - marker.theta;
      //make left go backwards, right forwards
      delay(75);
      //stop motors
      delay(1500);
     } 
      break;
  }
} 
}

