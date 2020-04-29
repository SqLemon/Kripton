#include <Arduino.h>
#include <PID_v1.h>

enum {MOT_D, MOT_I};
enum {RIGH, LEFT};
#define GAP_SIZE 100
#define RUN_TIME 10 //ms

//timer al que estan conectados los motores
//HardwareTimer motorTimer(n)

//setpoint, input, output
double motDSet, motDIn, motDOut;
double motISet, motIIn, motIOut;


//valores pid: soft y hard
double MHKp=4, MHKi=0.2, MHKd=1;
double MSKp=1, MSKi=0.05, MSKd=0.25;

PID MotD_PID(&motDIn, &motDOut, &motDSet, MSKp, MSKi, MSKd, DIRECT);
PID MotI_PID(&motIIn, &motIOut, &motISet, MSKp, MSKi, MSKd, DIRECT);



class Motors{
   private:
    char MOTD_A, MOTD_B , MOTI_A , MOTI_B;

   public:
    Motors(char, char, char, char);
    void init(void);
    void run(void);
    void setVel(int, int);
    void setMotVel(char, int);
    void turn90(char);
    void turn180(char);

};