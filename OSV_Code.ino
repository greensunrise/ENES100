/* OSV Code- Rev. 1
 *  Noah Eckman, Oct 27 2016
 */
#define marker_number 115
//Define Pin Numbers
#define L_PWM 6
#define L_S 4
#define L_R 7

#define R_PWM 10
#define R_S 8
#define R_R 9

#define ELEV_PWM 3
#define ELEV_S 0
#define ELEV_R 1

#define SYR_PWM 5
#define SYR_S 2

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
  rf.resetServer();
  rf.sendMessage("\nWetWare is Connected\n"); //sent to mission control
  theta_req = atan((2.9 - marker.x), (.35 - marker.y)); //From a rando orientation, go to the mission objective site
  orient(theta_req);
  int board = is_board_present();
}

void loop() {
  
}
