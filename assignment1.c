#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	sv->state = 0;
	sv->shock_activated = 0;
	sv->temp_activated = 0;
	sv->track_activated = 0;
	sv->lastButtonState = HIGH;
	sv->lastDebounce = 0;
	sv->debounceDelay = 50;
	sv->count = 0;
}

void init_sensors(SharedVariable* sv) {
	pinMode(PIN_BUTTON, INPUT);
	pinMode(PIN_YELLOW, OUTPUT);
	pinMode(PIN_TEMP, INPUT);
	pinMode(PIN_TRACK, INPUT);
	pinMode(PIN_SHOCK, INPUT);
	pinMode(PIN_RED, OUTPUT);
	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_BLUE, OUTPUT);
	pinMode(PIN_ALED, OUTPUT);
	pinMode(PIN_BUZZER, OUTPUT);
}

void body_button(SharedVariable* sv) {
	int var = digitalRead(PIN_BUTTON);
	sv->buttonState = var;

	//if(var != sv->lastButtonState) {
	 // sv->lastDebounce = millis();
	  
	  if(sv->buttonState == HIGH && sv->lastButtonState == LOW) {
		 //printf("buttonPressed\n");
		  int warning = sv->shock_activated || sv->temp_activated || sv->track_activated;
		  if(sv->state == 0){
			  sv->state = 1;
		  } else if (!warning){
			  sv->state = 0;
		  } else {
			  sv->count++;
			  if(sv->count == 2){
				  sv->state = 0;
				  sv->count = 0;
			  }
		  }		
	  }
	//}

	//if((millis() - sv->lastDebounce) > sv->debounceDelay) {
	//  sv->buttonState = var;
	//}
	
	sv->lastButtonState = var;

}

void body_twocolor(SharedVariable* sv) {
	if(sv->state == 0) {
	  digitalWrite(PIN_YELLOW, LOW);
	} else if(sv->state == 1) {
	  digitalWrite(PIN_YELLOW, HIGH);
	}
}

void body_temp(SharedVariable* sv) {
	if((sv->state == 1) && digitalRead(PIN_TEMP) == HIGH) {
		sv->temp_activated = 1;
		//printf("TEMPERATURE EMERGENCY!!\n");
	} else if((sv->state == 0) || digitalRead(PIN_TEMP) == LOW) {
		sv->temp_activated = 0;
	}
}

void body_track(SharedVariable* sv) {
	if((sv->state == 1) && digitalRead(PIN_TRACK) == LOW) {
		sv->track_activated = 1;
		//printf("TRACKING ACTIVATED!\n");
	} else if(sv->state == 0) {
		sv->track_activated = 0;
	}
}

void body_shock(SharedVariable* sv) {
	if((sv->state == 1) && digitalRead(PIN_SHOCK) == LOW) {
		sv->shock_activated = 1;
		//printf("SHOCK ACTIVATED!\n");
	} else if ((sv->state == 0)){
		sv->shock_activated = 0;
	}
}

void body_rgbcolor(SharedVariable* sv) {

	//handle all different situations
	//figure out the different rgb settings
	if(sv->state == 0) {
		digitalWrite(PIN_BLUE, HIGH);
		digitalWrite(PIN_GREEN, LOW);
		digitalWrite(PIN_RED, LOW);
  } else if(sv->state == 1) {

	  if(sv->track_activated != 1 && sv->shock_activated == 1) {
		  digitalWrite(PIN_RED, HIGH);
  	  digitalWrite(PIN_GREEN, LOW);
  	  digitalWrite(PIN_BLUE, LOW);
	  } else if(sv->shock_activated != 1 && sv->track_activated == 1) {
  	  digitalWrite(PIN_RED, HIGH);
  	  digitalWrite(PIN_GREEN, LOW);
  	  digitalWrite(PIN_BLUE, HIGH);
	  } else if(sv->shock_activated != 1 && sv->track_activated != 1) {
  	  digitalWrite(PIN_RED, LOW);
  	  digitalWrite(PIN_GREEN, HIGH);
  	  digitalWrite(PIN_BLUE, LOW);
	  }  
  }
	
}

void body_aled(SharedVariable* sv) {
	if(sv->state == 1) {
		
		int warning = sv->shock_activated || sv->track_activated;
    if(sv->temp_activated == 1 && !warning) {
		  digitalWrite(PIN_ALED, LOW);
		} else if(sv->temp_activated == 0) {
		  digitalWrite(PIN_ALED, HIGH);
		}
	} else if (sv->state == 0) {
		digitalWrite(PIN_ALED, HIGH);
	}
}

void body_buzzer(SharedVariable* sv) {
	if(sv->state == 1) {
		int warning2 = sv->shock_activated || sv->track_activated;
		if(sv->temp_activated == 1 && !warning2) {
		  digitalWrite(PIN_BUZZER, HIGH);
		  //printf("BUZZER ACTIVATED!\n");
	
		} 
		else if(sv->temp_activated == 0) {
		  digitalWrite(PIN_BUZZER, LOW);
		}
	}
}
