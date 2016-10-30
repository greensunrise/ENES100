/* This file is a location for supplemental code, i.e. functions that
 *  have to be called a lot in OSV_Code
 */




void get_pos(int mrknum)
{
  while((!rf.receiveMarker(&marker, mrknum))) {
  }
}
void line() {
  rf.sendMessage("\n");
}

void orient(float theta_req) {
  pinMode(L_S, OUTPUT); pinMode(L_R, OUTPUT); pinMode(L_PWM, OUTPUT);
  pinMode(R_S, OUTPUT); pinMode(R_R, OUTPUT); pinMode(R_PWM, OUTPUT);
  get_pos(marker_number); //Gets its position 
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
      get_pos(marker_number);
      d_theta = theta_req - marker.theta;
      turn_right();
    }
      break;
      
    case 1:
      line(); rf.sendMessage("Turning anti-clockwise...");
      while(d_theta > .15)
    {
      get_pos(marker_number);
      d_theta = theta_req - marker.theta;
      turn_left();
     } 
      break;
  }
} 

void turn_left() {
  digitalWrite(L_S, HIGH); //Is this right? lol who knows
  digitalWrite(L_R, LOW);
  analogWrite(L_PWM, HIGH);
  delay(100);
  digitalWrite(L_S, LOW);
  digitalWrite(L_R, LOW);
  analogWrite(L_PWM, LOW);
  delay(1500);
}
void turn_right() {
  digitalWrite(L_R, HIGH); //Is this right? lol who knows
  digitalWrite(L_S, LOW);
  analogWrite(L_PWM, HIGH);
  delay(100);
  digitalWrite(L_R, LOW);
  digitalWrite(L_S, LOW);
  analogWrite(L_PWM, LOW);
  delay(1500);
}

int is_board_present() { //Returns 0 if board is absent, 1 if board is present.
  #define trigPin 11
  #define echoPin 12
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  float rolling_val = 0;
  int a = 1;
  float sum = 0;
  
while (a <= 10) { //Taking avg of 10 measurements
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance <=0) { //Scan for bad values.
    //Iterate again without touching counter.
    continue;
  } 
  sum = sum + distance;
  delay(500);
  a += 1;

rolling_val = sum/10;
if (rolling_val >= 130 || rolling_val <= 0){
    return(0); //It's not there.
  }
  else {
    return(1); //It's there.
  }
}

