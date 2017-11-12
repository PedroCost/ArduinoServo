
#include <Servo.h>

int sensorPin = A0;       // Pin para potentiometer
// Pins to motor
int motorPin1 = 2, motorPin2 = 3, motorPin3 = 4, motorPin4 = 6, motorPin5 = 7, motorPin6 = 8;

int ledO1 = 14;        					// Pin led/output 1 -- Bitalino O1 > Arduino 14
int ledO2 = 15;        					// Pin led/output 2 -- Bitalino O2 > Arduino 15

int ledO1Value = 0, ledO2Value = 0;     // Valor do Led (0,1)
float sensorValue = 0;    				// Valor do PWM do bitalino

Servo servo1, servo2, servo3, servo4, servo5, servo6;

int last_ledO1Value = 0, last_ledO2Value = 0, last_sensorValue = 0;

int erroMaximo = 20, erroMedida = 10, diferencaPotencia = 120, ultimaPotencia = 0;

int polegarAtivo = 0, indicadorAtivo = 0, medioAtivo = 0, anelarAtivo = 0, minimoAtivo = 0, rotacaoAtivo = 0;

// initialize the serial communication:
void setup() {
  Serial.begin(115200);
  pinMode(ledO1, INPUT);
  pinMode(ledO2, INPUT);
  servo1.attach(motorPin1);
  servo2.attach(motorPin2);
  servo3.attach(motorPin3);
  servo4.attach(motorPin4);
  servo5.attach(motorPin5);
  servo6.attach(motorPin6);
}

// read the value from the sensor:
void loop() {
  sensorValue = analogRead(sensorPin) / 1024.0 * 360; // Valores de (0 , 250)
  ledO1Value = digitalRead(ledO1); // Valores 0 e 1
  ledO2Value = digitalRead(ledO2); // Valores 0 e 1
  Serial.println(sensorValue); 		// DEBUG

  if(ultimaPotencia - sensorValue > 10 || ultimaPotencia - sensorValue < 10){
    polegarAtivo = 0;
    indicadorAtivo = 0;
    anelarAtivo = 0;
    minimoAtivo = 0;
    medioAtivo = 0;
    rotacaoAtivo = 0;
	  delay(30);
  }
  
  
  // DEDO MEDIO
  if (ledO2Value > 0 && ledO1Value > 0 && sensorValue < diferencaPotencia && medioAtivo == 0) {
	  controlerServo(servo3, sensorValue);
	   medioAtivo = 1;
	  desativa(3);
  }
  
  // DEDO ROTACAO
  else if (ledO2Value > 0 && ledO1Value > 0 && rotacaoAtivo == 0) {
	controlerServo(servo6, sensorValue - diferencaPotencia);
	rotacaoAtivo = 1;
	desativa(6);
  }
  
  // DEDO POLEGAR & INDICADOR
  else if (ledO1Value > 0 && ledO2Value == 0) {
    if(sensorValue < diferencaPotencia && polegarAtivo == 0){
		controlerServo(servo1, sensorValue);
		polegarAtivo = 1;
		desativa(1);
    }
    else if(indicadorAtivo == 0){
		controlerServo(servo2, sensorValue - diferencaPotencia);
		indicadorAtivo = 1;
		desativa(2);
    }
  }
  
  // DEDO MINIMO & ANELAR
  else if (ledO2Value > 0 && ledO1Value == 0) {
    if(sensorValue < diferencaPotencia && minimoAtivo == 0){
    controlerServo(servo5, sensorValue);
    minimoAtivo = 1;
    desativa(5);
    }
    else if(anelarAtivo == 0){
	  controlerServo(servo4, sensorValue - diferencaPotencia);
    anelarAtivo = 1;
    desativa(4);
    }
  }
  
  
  ultimaPotencia = sensorValue;
  // desativaDedos(ledO1Value, ledO2Value, sensorValue);
  delay(10);
}

void controlerServo(Servo servo, int angulo){
  if(angulo < 10){
    servo.write(5);
  }
  else if(angulo > 20-erroMedida && angulo < 20+erroMedida){
    servo.write(36);
  } 
  else if(angulo > 40-erroMedida && angulo < 40+erroMedida){
    servo.write(72);
  }
  else if(angulo > 60-erroMedida && angulo < 60+erroMedida){
    servo.write(108);
  }
  else if(angulo > 80-erroMedida && angulo < 80+erroMedida){
    servo.write(144);
  }
  else if(angulo > 100-erroMedida && angulo < 100+erroMedida){
    servo.write(165);
  }
  delay(10);
}

void desativaDedos(int led1, int led2, int valorSensor){
  if(led1 == 0){
    polegarAtivo = 0;
    indicadorAtivo = 0;
    medioAtivo = 0;
	rotacaoAtivo = 0;
  } 
  if(led2 == 0){
    anelarAtivo = 0;
    minimoAtivo = 0;
    medioAtivo = 0;
	rotacaoAtivo = 0;
  }
}

void desativa(int dedo){
	if(dedo != 1)
		polegarAtivo = 0;
	if(dedo != 2)
		indicadorAtivo = 0;
	if(dedo != 3)
		medioAtivo = 0;
	if(dedo != 4)
		anelarAtivo = 0;
	if(dedo != 5)
		minimoAtivo = 0;
	if(dedo != 6)
		rotacaoAtivo = 0;
}


