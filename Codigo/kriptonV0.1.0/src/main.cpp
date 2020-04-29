#include <SimpleTimer.h>
#include "main.h"
#include "motors.h"

int velA, velB;
Motors motors(MOTD_A_PIN, MOTD_B_PIN, MOTI_A_PIN, MOTI_B_PIN);
SimpleTimer motorExecute;


void setup() {
  motors.init();
  motorExecute.setInterval(RUN_TIME, motors.run);
}

void loop() {
  motorExecute.run();
}
