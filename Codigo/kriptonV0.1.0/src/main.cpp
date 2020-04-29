#include <SimpleTimer.h>
#include "main.h"
#include "motors.h"

int velA, velB;
Motors motors(MOTD_DIR, MOTD_PWM, MOTI_DIR, MOTI_PWM);
SimpleTimer motorExecute;



void setup() {
  motors.init();
  motorExecute.setInterval(RUN_TIME, motorRun);
}

void loop() {
  int mode = menu();
}

void motorRun(){  
  motors.run();
}

int menu(void){

}

int readOptions(){
  if(digitalRead(BTN1)){
    while(digitalRead(BTN1))
    return 0;
  }

  if(digitalRead(BTN2)){
    while(digitalRead(BTN2))
    return 0;
  }

  if(digitalRead(BTN3)){
    while(digitalRead(BTN3))
    return 0;
  }
}