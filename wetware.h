
#ifndef wetware_h
#define wetware_h
#include "Arduino.h"
class wetware
{
	public:
	  wetware(int L_PWM);
	  wetware(int L_S);
	  wetware(int L_R);
	  wetware(int R_PWM);
	  wetware(int R_S);
	  wetware(int R_R);
	  wetware(int ELEV_PWM);
	  wetware(int ELEV_S);
   	  wetware(int ELEV_R);
	  wetware(int SYR_PWM);
	  wetware(int SYR_S) //Arbitrary naming? I took a guess on S vs R.
	  void orient(float theta)	
	  
	  //extra things go here.
}