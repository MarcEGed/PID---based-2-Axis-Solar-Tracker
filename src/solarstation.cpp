#include <Servo.h>
#include <Arduino.h>

//photores pins
const int TL = A4;
const int TR = A6;
const int BL = A5;
const int BR = A7;

bool started = false;
unsigned long startMillis = 0;

//servos
const int servoHPin = 9;
const int servoVPin = 10;

Servo servoH;
Servo servoV;

int posH = 90;
int posV = 90;

long sampleCount = 0;
long samplesWanted = 100;

//params
const int deadzone = 40;
const int samples = 10;

//PID ctes
float Kp = 0.04;
float Ki = 0.0002;
float Kd = 0.02;

//PID variables
float integralH = 0;
float integralV = 0;

int prevErrorH = 0;
int prevErrorV = 0;

bool shouldSleep(int errorH, int errorV){
  return abs(errorH) < 10 && abs(errorV) < 10;
}

int average_Samples(int pin) {
  long total = 0;
  for(int i=0;i<samples;i++){
    total += analogRead(pin);
    delay(2);
  }
  return total / samples;
}

float computePID(int error, float& integral, int& prevError){
  if(abs(error) > deadzone){
    integral += error;
    integral = constrain(integral, -2000, 2000);   //no windup

    int derivative = error - prevError;

    float output = Kp * error + Ki * integral + Kd * derivative;
    prevError = error;

    return output;
  }
  return 0;
}

void printSensors(int tl, int tr, int bl, int br) {
  Serial.print("TL: "); Serial.print(tl);
  Serial.print(" TR: "); Serial.print(tr);
  Serial.print(" BL: "); Serial.print(bl);
  Serial.print(" BR: "); Serial.println(br);
}

void setup() {
  Serial.begin(9600);

  servoH.attach(servoHPin);
  servoV.attach(servoVPin);

  servoH.write(posH);
  servoV.write(posV);  
}

void loop() {
  int tl = average_Samples(TL);
  int tr = average_Samples(TR);
  int bl = average_Samples(BL);
  int br = average_Samples(BR);

  int left  = (tl + bl) / 2;
  int right = (tr + br) / 2;
  int top   = (tl + tr) / 2;
  int bottom = (bl + br) / 2;

  int errorH = left - right;
  int errorV = top - bottom;

  posH += computePID(errorH, integralH, prevErrorH);
  posV += computePID(errorV, integralV, prevErrorV);

  posH = constrain(posH, 5, 160);
  posV = constrain(posV, 5, 160);

  servoH.write(posH);
  servoV.write(posV);

  if (Serial.available() > 0){
    char c = Serial.read();
    if (c == 'S' || c == 's'){
      started = true;
      startMillis = millis();
      sampleCount = 0;
      Serial.println("time,posH,posV,errorH,errorV");
    }
  }

  if (started && sampleCount < sampleWanted){
    Serial.print(millis() - startMillis);
    Serial.print(",");
    Serial.print(posH);
    Serial.print(",");
    Serial.print(posV);
    Serial.print(",");
    Serial.print(errorH);
    Serial.print(",");
    Serial.println(errorV);

    sampleCount++;
    if (sampleCount >= samplesWanted){
      started = false;
    }
  }
  delay(50);
}