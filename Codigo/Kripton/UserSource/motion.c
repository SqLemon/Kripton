#include "main.h"
#include "params.h"

#define ENCODER_TICKS_PER_METER 8000L // Cantidad de ticks de encoder por metro (numero placeholder, cambiar por el real)

float currentSpeedX = 0;
float currentSpeedW = 0;
int targetSpeedX = 0;
int targetSpeedW = 0;
int encoderFeedbackX = 0;
int encoderFeedbackW = 0;
float errorX = 0;
float errorW = 0;
float oldErrorX = 0;
float oldErrorW = 0;
int pwmX = 0;
int pwmW = 0;

uint32_t encoderChange;
uint32_t leftEncoderOld;
uint32_t rightEncoderOld;

// PD para recta
const float KP_X = 2;
const float KD_X = 4;
const float KP_W = 1;
const float KD_W = 12;

// PD para curva
const float KP_W1 = 1; // used for T1 and T3 in curve turn
const float KD_W1 = 26;
const float KP_W2 = 1; //used for T2 in curve turn
const float KD_W2 = 36;

// Aceleración y desaceleración máxima
const float accelX = 600; // m/s^2
const float decelX = 600;
const float accelW = 1; // m/s^2
const float decelW = 1;

// Constantes de velocidades obtenidas de parámetros, expresados en m/s (o mm/ms, equivalente).
// Todas duplicadas ya que se expresan las velocidades en la suma de los dos lados para evitar promedios.
const int moveSpeed = speed_to_counts(MOVE_SPEED * 2);
const int turnSpeed = speed_to_counts(TURN_SPEED * 2);
const int returnSpeed = speed_to_counts(RETURN_SPEED * 2);
const int stopSpeed = speed_to_counts(STOP_SPEED * 2);
const int maxSpeed = speed_to_counts(MAX_SPEED * 2);


// Macro que convierte velocidades de m/s (equivalente a mm/ms) a ticks de encoder por milisegundo
inline int mps_to_encpms(int mps){
	return (mps * ENCODER_TICKS_PER_METER);
}


// Macro que convierte velocidades de ticks de encoder por milisegundo a m/s (equivalente a mm/ms)
inline int encpms_to_mps(int encpms){
	return (encpms / ENCODER_TICKS_PER_METER);
}


// Esta función lee los encoders y actualiza los datos de sus estados
// (distancia recorrida en el último milisegundo y en general)
void updateEncoderStatus(void){
	uint32_t leftEncoder = TIM2->CNT; // Lee valor de encoder derecho del timer 2
	uint32_t rightEncoder = TIM3->CNT; // Lee valor de encoder derecho del timer 3

	uint32_t leftEncoderChange = leftEncoder - leftEncoderOld;
	uint32_t rightEncoderChange = rightEncoder - rightEncoderOld;
	encoderChange = (leftEncoderChange + rightEncoderChange) / 2; // Promedio de encoders para calcular velocidad general en eje X

	leftEncoderOld = leftEncoder;
	rightEncoderOld = rightEncoder;

	leftEncoderCount += leftEncoderChange;
	rightEncoderCount += rightEncoderChange;
	encoderCount =  (leftEncoderCount + rightEncoderCount) / 2; // Conteo de pasos generales para odometría en eje X

	remainingDistance -= encoderChange; // Actualizo distancia restante de avance
}


void updateCurrentSpeed(void){
	/* Velocidad en el eje X (adelante/atrás) */
	if(currentSpeedX < targetSpeedX) { // Fase de aceleración
		currentSpeedX += (float)(mps_to_encpms(accelX * 2)); // La velocidad X es el doble ya que después se compara con la suma de los cambios (saltea división para promedio)
		if(currentSpeedX > targetSpeedX) currentSpeedX = targetSpeedX; // Limita la velocidad al target
	} else if(currentSpeedX > targetSpeedX) { // Fase de desaceleración
		currentSpeedX -= (float)(mps_to_encpms(decelX * 2));
		if(currentSpeedX < targetSpeedX) currentSpeedX = targetSpeedX;
	}

	/* Velocidad en el eje W (rotacional) */
	if(currentSpeedW < targetSpeedW){ // Fase de aceleración
		currentSpeedW += accelW;
		if(currentSpeedW > targetSpeedW) currentSpeedW = targetSpeedW;
	} else if(currentSpeedW > targetSpeedW){ // Fase de desaceleración
		currentSpeedW += decelW;
		if(currentSpeedW < targetSpeedW) currentSpeedW = targetSpeedW;
	}
}


// Función que hace PD para el PWM de los motores junto a los datos ya calculados de encoder y el perfil de velocidad
// (después se le puede agregar el gyro y los sensores de costado). Aplica a rectas.
void calculateMotorPWM(void){
	// int gyroFeedback;
	// int sensorFeedback; //fijarse el signo del error (para qué lado sube y para qué lado baja)

	encoderFeedbackX = rightEncoderChange + leftEncoderChange; // Feedback en el eje X de los encoders
	encoderFeedbackW = rightEncoderChange - leftEncoderChange; // Feedback en el eje W (rotacional) de los encoders

	// gyroFeedback = aSpeed / gyroFeedbackRatio; // TODO: Investigar gyroFeedbackRatio en la clase de curve turn
	// sensorFeedback = sensorError / aScale; // Escala para que no sean valores enormes

	int rotationalFeedback = encoderFeedbackW;
	// if(useGyro) rotationalFeedback += gyroFeedback;
	// if(useSensor) rotationalFeedback += sensorFeedback;

	errorX += currentSpeedX - encoderFeedbackX;
	errorW += currentSpeedW - rotationalFeedback;

	pwmX = KP_X * errorX + KD_X * (errorX - oldErrorX);
	pwmW = KP_W * errorW + KD_W * (errorW - oldErrorW);

	oldErrorX = errorX;
	oldErrorW = errorW;

	leftPWM = pwmX - pwmW;
	rightPWM = pwmX + pwmW;

	setLeftPWM(leftPWM);
	setRightPWM(rightPWM);
}


// Función que calcula la desaceleración requerida, para llegar de la velocidad curSpeed
// a la velocidad endSpeed en cierta distancia.
int requiredDeceleration(int32_t distance, int16_t curSpeed, int16_t endSpeed){
	if(curSpeed < 0) curSpeed = -curSpeed;
	if(endSpeed < 0) endSpeed = -endSpeed;
	if(dist <= 0) dist = 1; // Prevenir la división por 0

	return abs( encpms_to_mps( (double)(curSpeed*curSpeed - endSpeed*endSpeed)/distance/4/2 ) );
	// Cosas medio mágicas pero básicamente aplico la fórmula de física básica 2aS=Vt^2-V0^2
	// despejada a a=(Vt^2-V0^2)/2S y agregando una división por 4 (=2^2) porque las velocidades
	// son la suma de la velocidad de cada lado (para evitar promedios). El cuadrado se distribuye.
}


// Función básica para conseguir error de los sensores de costado, se puede mejorar
void updateSensorError(void){
	if(leftSensor > leftMiddleValue && rightSensor < rightMiddleValue)
		sensorError = leftMiddleValue - leftSensor;
	else if(leftSensor < leftMiddleValue && rightSensor > rightMiddleValue)
		sensorError = rightSensor - rightMiddleValue;
	else
		sensorError = 0;
}


// Nombre autodescriptivo
void resetSpeedProfile(void){
	useSensor = false;
	useGyro = false;

	pidInputX = 0;
	pidInputW = 0;
	curSpeedX = 0;
	curSpeedW = 0;
	targetSpeedX = 0;
	targetSpeedW = 0;
	posErrorX = 0;
	posErrorW = 0;
	oldPosErrorX = 0;
	oldPosErrorW = 0;
	leftEncoderOld = 0;
	rightEncoderOld = 0;
	leftEncoder = 0;
	rightEncoder = 0;
	leftEncoderCount = 0;
	rightEncoderCount = 0;
	encoderCount = 0;
	oldEncoderCount = 0;
	leftBaseSpeed = 0;
	rightBaseSpeed = 0;

	TIM2->CNT = 0;
	TIM3->CNT = 0;
}


void moveX(int distance, int speed, int exitSpeed){
	useSensor = false;
	useGyro = false;

	targetSpeedW = 0;
	targetSpeedX = speed;
	remainingDistance = distance;

	do {
		// Confirmar si necesitamos desacelerar ya o hay tiempo para seguir acelerando
		if(requiredDeceleration(remainingDistance, currentSpeedX, exitSpeed) >= decelX)
			targetSpeedX = exitSpeed;
		else
			targetSpeedX = speed;

		// Se podría detectar acá las columnas para alinear el error de eje X
	} while((encoderCount - oldEncoderCount) < distance);

	oldEncoderCount = encoderCount;
}
