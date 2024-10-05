#include<Wire.h>

const int photoResistor = A0; // Fotootpornik, analogni pin A0
const int frontLight = 6;       // Prednje svjetlo bicikla, PWM pin
const int brakes = 8;  // Svjetlo koje se pali prilikom kočenja
const int left = 13;  // Lijevi žmigavac
const int right = 2; // Desni žmigavac

int value;	 // Vrijednost fotootpornika (0-1023)

const int MPU_addr = 0x68;  // MPU6050 I2C adresa
int16_t AcX, AcY, AcZ;
 
int minVal  = 265;
int maxVal  = 402;
 
double x;
double y;
double z;

void setup(){
  
  // Definiranje ulaznih i izlaznih pinova
  pinMode(frontLight,  OUTPUT);  
  pinMode(brakes, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(photoResistor, INPUT);

  Wire.begin(); // Inicijalizacija komunikacije s MPU6050
  Wire.beginTransmission(MPU_addr); // Početak komunikacije
  Wire.write(0x6B); // Komunikacija s registrom 6B
  Wire.write(0); // Pisanje 0 u registar 6B
  Wire.endTransmission(true); // Kraj prijenosa

}

void loop(){

  value = analogRead(photoResistor); // Čitanje vrijednosti fotootpornika
 
  // Postavljanje vrijednosti prednjeg svjetla u ovisnosti o vrijednosti fotootpornika
  if(value > 400 && value < 500){
    analogWrite(frontLight,  255); 
  }
  else if(value >= 500 && value < 600){
    analogWrite(frontLight,  212);
  }
  else if(value >= 600 && value < 700){
    analogWrite(frontLight,  170);
  }
  else if(value >= 700 && value < 800){
    analogWrite(frontLight,  127);
  }
  else if(value >= 800 && value < 900){
    analogWrite(frontLight,  85);
  }
  else if(value >= 900 && value < 1000){
    analogWrite(frontLight,  42);
  }
  else if(value >= 1000){
    analogWrite(frontLight,  0);
  }
  
  // Čitanje vrijednosti akcelerometra
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);

  // Vrijednosti osi
  AcX = Wire.read()<<8|Wire.read(); // x-os
  AcY = Wire.read()<<8|Wire.read(); // y-os
  AcZ = Wire.read()<<8|Wire.read(); // z-os
  
  // Računanje kuteva
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);
  
  // Pretvorba radijana u stupnjeve
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
  
  // Kočenje
  if(x >= 20 && x <= 90){
    digitalWrite(brakes, HIGH);
  }
  else{
    digitalWrite(brakes, LOW);
  }

 // U ovisnosti o vrijednosti kuteva paliti lijevi ili desni žmigavac
  if(y >= 10 && y <= 100){
    digitalWrite(left, HIGH);
    digitalWrite(right, LOW);
    delay(200);
    digitalWrite(left, LOW);
  }
  else if(y >= 260 && y <= 350){
    digitalWrite(right, HIGH);
    digitalWrite(left, LOW);
    delay(200);
    digitalWrite(right, LOW);
  }
  else{
    digitalWrite(right, LOW);
    digitalWrite(left, LOW);
  }

    delay(500); 
  }
