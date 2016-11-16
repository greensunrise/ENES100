

/* This file is a location for supplemental code, i.e. functions that
 *  have to be called a lot in OSV_Code
 */

void get_pos(int mrknum) 
{
  while (!rf.receiveMarker(&marker, mrknum)) { 
    rf.sendMessage("\nThe RF failed to get the position.\n");
    delay(250);
  }
 rf.sendMessage("\nGot my position.\n"); 
}

void line() {
  rf.sendMessage("\n");
}

void turn_motors_off() {
  digitalWrite(LEFT_FORWARD, LOW); 
  digitalWrite(LEFT_BACKWARD, LOW);
  analogWrite(L_PWM, 0);
    
  digitalWrite(RIGHT_FORWARD, LOW); 
  digitalWrite(RIGHT_BACKWARD, LOW);
  analogWrite(R_PWM, 0);
  delay(300);
}

void orient(float theta_req) {
  get_pos(marker_number); //Gets its position 
  float d_theta = theta_req - marker.theta;
  int dir;
  if (d_theta < -.15)
  { //declare dir as -1 for case switch
    dir = -1;
  }
  else if(d_theta > .15)
  { //declare dir as 1 for case switch
    dir = 1;  
  }
  else 
  {
    dir = 0;
  }

  switch (dir) {
    case -1:
    line(); rf.sendMessage("Turning Clockwise, d_theta is"); rf.sendMessage(d_theta);
    while(d_theta < -.15)
    {
      get_pos(marker_number);
      d_theta = theta_req - marker.theta;
      line(); rf.sendMessage(d_theta);
      turn_left();
      go_forward();
      if(d_theta > .15) {
        continue;
      }
      get_pos(marker_number);
      turn_left();
      go_backwards();
    }
      break;
      
    case 1:
      line(); rf.sendMessage("Turning anti-clockwise, d_theta is"); rf.sendMessage(d_theta);
      while(d_theta > .15)
    {
      get_pos(marker_number);
      d_theta = theta_req - marker.theta;
      line(); rf.sendMessage(d_theta);
      turn_right();
      go_forward();
      if(d_theta > .15) {
        continue;
      }
      turn_right();
      go_backwards();
     } 
      break;
    case 0:
      break;
  }
} 

void turn_left() {
  digitalWrite(LEFT_FORWARD, HIGH); 
  digitalWrite(LEFT_BACKWARD, LOW);
  analogWrite(L_PWM, 200);

  digitalWrite(RIGHT_BACKWARD, HIGH); 
  digitalWrite(RIGHT_FORWARD, LOW);
  analogWrite(R_PWM, 200);
  
  delay(100);
  
  turn_motors_off();
  
  delay(10);
}
void turn_right() {
  digitalWrite(LEFT_FORWARD, LOW); 
  digitalWrite(LEFT_BACKWARD, HIGH);
  analogWrite(L_PWM, 200);

  digitalWrite(RIGHT_FORWARD, HIGH); 
  digitalWrite(RIGHT_BACKWARD, LOW);
  analogWrite(R_PWM, 200);
  
  delay(100);
  
  turn_motors_off();
  
  delay(10);
}
void go_forward() {
  digitalWrite(LEFT_FORWARD, HIGH); 
  digitalWrite(LEFT_BACKWARD, LOW);
  analogWrite(L_PWM, 150);

  digitalWrite(RIGHT_FORWARD, HIGH); 
  digitalWrite(RIGHT_BACKWARD, LOW);
  analogWrite(R_PWM, 150);
  
  delay(250);
  
  turn_motors_off();
  
  delay(10);
}
void go_backwards() {
  digitalWrite(LEFT_FORWARD, LOW); 
  digitalWrite(LEFT_BACKWARD, HIGH);
  analogWrite(L_PWM, 255);

  digitalWrite(RIGHT_FORWARD, LOW); 
  digitalWrite(RIGHT_BACKWARD, HIGH);
  analogWrite(R_PWM, 255);
  
  delay(250);
  turn_motors_off();
  delay(10);
}

bool is_board_present() { //Returns false if board is absent, true if board is present.

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
}
rolling_val = sum/10;
if (rolling_val >= 130 || rolling_val <= 0){
    return(false); //It's not there.
  }
  else {
    return(true); //It's there.
  }
}


void goto_y(float y, float theta) {
   get_pos(marker_number);
   float d_y = (y - marker.y);
   rf.sendMessage(d_y);
  while(abs(d_y) > .1) {
    rf.sendMessage(d_y); line();
    go_forward();
    orient(theta);
    get_pos(marker_number);
    d_y = (y - marker.y);
  } 
}

void goto_x(float x, float theta) {
   get_pos(marker_number);
   orient(theta);
   float d_x = (x - marker.x);
  while(abs(d_x) > .1) {
    rf.sendMessage(d_x); line();
    go_forward();
    orient(theta);
    get_pos(marker_number);
    d_x = (x - marker.x);
  } 
}

void salinity() {


int i = 0;
int sum = 0;
while(i < 10) {
  sum += analogRead(A0); 
}
float salinity = sum/10;

if (salinity>740)
{
  line();
  rf.sendMessage("Saltwater"); 
  delay(100);
  
}
if (salinity>650 && salinity<740)
{
  line();
  rf.sendMessage("Freshwater");
  delay(100);
}
else
{
  line();
  rf.sendMessage("No water present. BZZZRP!");
}
}

float water_depth() {
int echoPin = 16;
int trigPin = 17;                          
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(9600);
  double duration, distance; 
  float distanceReadings[50];
  float distanceFromWater;
  int index;
  float depth;    
  float sum = 0;                              
  for(index = 0; index < 50; index++)
    {
       digitalWrite(trigPin, LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin, HIGH);
       delayMicroseconds(10);
       digitalWrite(trigPin, LOW); 
       duration = pulseIn(echoPin, HIGH);
       distance = duration/ 2.9/ 2;
       distanceReadings[index] = distance;
       Serial.println(distanceReadings[index]);
       sum = sum + distanceReadings[index];
       delay(50); 
    }
    distanceFromWater = (sum / 50); 
    depth = 76.5 - distanceFromWater; 
    return(depth);   
}

