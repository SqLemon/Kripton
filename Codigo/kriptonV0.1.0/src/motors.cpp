#include "motors.h"

Motors::Motors(char Da, char Db, char Ia, char Ib){
  MOTD_A = Da;
  MOTD_B = Db;
  MOTI_A = Ia;
  MOTI_B = Ib;

  motDSet = 0;
  motDIn = 0;
  motDOut = 0;
  
  motISet = 0;
  motIIn = 0;
  motIOut = 0;
}

void Motors::init(void){
  pinMode(MOTD_A, OUTPUT);
  pinMode(MOTD_B, OUTPUT);
  pinMode(MOTI_A, OUTPUT);
  pinMode(MOTI_B, OUTPUT);

  //seteo frecuencia pwm
  //motorTimer.setPeriod(100)//us. frecuencia de pwm (10kHz) descomentar el hardwareTimer en .h

  //"Prendo" el PID. (MANUAL para apagarlo) 
  MotD_PID.SetMode(AUTOMATIC);
  MotI_PID.SetMode(AUTOMATIC);
  
  //Seteo el tiempo de ejecucion del loop
  MotD_PID.SetSampleTime(RUN_TIME);
  MotI_PID.SetSampleTime(RUN_TIME);

  //seteo valores maximos y minimos
  MotD_PID.SetOutputLimits(-65535, 65535); //maximo analog 
  MotI_PID.SetOutputLimits(-65535, 65535); //maximo analog 
}

void Motors::run(){
  //getear las velocidades

  double Dgap = abs(motDSet-motDIn); //distance away from setpoint
  double Igap = abs(motISet-motIIn); 
  
  //Cambio de constantes segun distancia al setpoint (soft o hard)
  //----------DERECHA----------
  if (Dgap < GAP_SIZE)
      MotD_PID.SetTunings(MSKp, MSKi, MSKd); //poca distancia al setpoint. pid menos agresivo
  else
      MotD_PID.SetTunings(MHKp, MHKi, MHKd);//

  //----------IZUQIERDA----------
  if (Igap < 10)
      MotI_PID.SetTunings(MSKp, MSKi, MSKd); //poca distancia al setpoint. pid menos agresivo
  else
      MotI_PID.SetTunings(MHKp, MHKi, MHKd);

  /////////////////////
  MotD_PID.Compute();
  MotI_PID.Compute();
  /////////////////////
  
  //Write a pines de motores
  //----------DERECHA----------
  if(motDOut >= 0){
    analogWrite(MOTD_A, motDOut);
    digitalWrite(MOTD_B, 0);
  }
  else{
    digitalWrite(MOTD_A, 0);
    analogWrite(MOTD_B, -motDOut);
  }

  //----------IZQUIERDA----------
  if(motIOut >= 0){
    analogWrite(MOTD_A, motIOut);
    digitalWrite(MOTD_B, 0);
  }
  else{
    digitalWrite(MOTD_A, 0);
    analogWrite(MOTD_B, -motIOut);
  }
}

void Motors::setVel(int velD, int velI){
  motDSet = velD;
  motISet = velI;
}

void Motors::setMotVel(char motor, int vel){
  if(motor == MOT_D) motDSet = vel;
  else if(motor == MOT_I) motISet = vel;
}

void Motors::turn90(char side){

}

void Motors::turn180(char side){

}

