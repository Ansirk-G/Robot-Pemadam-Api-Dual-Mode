#include <Servo.h>
#include <AFMotor.h>

#define Slow 130
#define Medium 180
#define Fast 250
#define Flame_kanan A4
#define Flame_kiri A3
#define Flame_depan A2

int trigPin = A1;
int echoPin = A0;
String readString;
bool AutoMode = true;

Servo myservo;
AF_DCMotor M1(1, MOTOR12_1KHZ);
AF_DCMotor M4(4, MOTOR34_1KHZ);
AF_DCMotor pompa(2, MOTOR12_1KHZ);

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(10);
  pompa.setSpeed(Fast);
  M1.setSpeed(0);
  M4.setSpeed(0);
}

void loop() {
  while(Serial.available()>0){
    M1.run(RELEASE);
    M4.run(RELEASE);
    AutoMode = false;
    char c = Serial.read();
    readString += c;
  }
  if (readString.length() > 0){
    Serial.println(readString);
    
    if (readString == "auto"){
      AutoMode = true;
    }
    if (readString == "bluetooth"){
      AutoMode = false;
    }
  }
  
  if (AutoMode == true){
    float distance = getDistance();
    int output = fuzzyLogic(distance);
    motorControl(output);
        int Api1 = analogRead(Flame_kanan);
        int Api2 = analogRead(Flame_kiri);
        int Api3 = analogRead(Flame_depan);
        if (Api3 <= 100 && Api3 > 50){
          delay(50);
          berhenti();
          M1.setSpeed(150);
          M4.setSpeed(150);
          delay(50);
          maju();
          Serial.print("Api Terdeteksi");
          Serial.println(Api3);
          delay(1000);
        }
        else if (Api2 <= 100 && Api2 > 50){
          delay(50);
          berhenti();
          M1.setSpeed(150);
          M4.setSpeed(150);
          delay(50);
          kiri();
          Serial.print("Api Terdeteksi");
          Serial.println(Api2);
          delay(1000);
        }
        else if (Api1 <= 100 && Api1 > 50){
          delay(50);
          berhenti();
          M1.setSpeed(150);
          M4.setSpeed(150);
          delay(50);          
          kanan();
          Serial.print("Api Terdeteksi");
          Serial.println(Api1);
          delay(1000);
        }
        else if (Api1 <= 50 || Api2 <=50 || Api3 <= 50) {
          delay(50);
          M1.run(RELEASE);
          M4.run(RELEASE);
          pompa.setSpeed(255);
          pompa.run(FORWARD);
          myservo.write(20);
          delay(800);
          myservo.write(160);
          delay(800);
          myservo.write(100);
          delay(800);          
          Serial.print("Api Terdeteksi");
          Serial.println(Api1);
          delay(1000);
          Serial.print("Api Terdeteksi");
          Serial.println(Api2);
          delay(1000);
          Serial.print("Api Terdeteksi");
          Serial.println(Api3);
          delay(1000);
        }
        else {
          Serial.print("Api Tidak Terdeteksi");
          pompaOff();
          delay(1000);
        }
  } 
  else if (AutoMode == false){
    AutoMode = false;
    Bluetooth(); 
    delay(50);
  }
}

//fuzzifikasi
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance = (duration / 2) * 0.0343;
  return distance;
}

//rule evaluation
int fuzzyLogic(float distance) {
  int output = 0;
  if (distance <= 15) {
    output = 0; // stop
  }
  else if (distance > 15 && distance <= 25) {
    output = 1; // near
  } else if (distance > 25 && distance <= 35) {
    output = 2; // medium
  } else if (distance > 35) {
    output = 3; // far
  } 
  return output;
}

//defuzzifikasi
void motorControl(int output) {
  int speed = 0;
  switch (output) {
    case 0: // slow
       M1.setSpeed(170);
       M4.setSpeed(170);
       M1.run(RELEASE);
       M4.run(RELEASE); // set speed to a stop value
       delay(500);
       M1.run(FORWARD);
       M4.run(BACKWARD);
       delay(500);
       break;
    case 1: // slow
       speed = 50; // set speed to a low value
       break;
    case 2: // medium
       speed = 100; // set speed to a medium value
       break;
    case 3: // fast
       speed = 150; // set speed to a high value
       break;
    }
   M1.setSpeed(speed);
   M4.setSpeed(speed);
   M1.run(FORWARD);
   M4.run(FORWARD);
}

void maju(){
  M1.run(FORWARD);
  M4.run(FORWARD);
}
void mundur(){
  M1.run(BACKWARD);
  M4.run(BACKWARD);
}
void kanan(){
  M1.run(FORWARD);
  M4.run(BACKWARD);
}
void kiri(){
  M1.run(BACKWARD);
  M4.run(FORWARD);
}
void berhenti(){
  M1.run(RELEASE);
  M4.run(RELEASE);
}
void pompaOn(){
  pompa.run(FORWARD);
}
void pompaOff(){
  pompa.run(RELEASE);
}

void Bluetooth(){
  while(Serial.available()>0){
    AutoMode = false;
    delay(50);
    char c = Serial.read();
    readString += c;
  }
  if (readString.length() > 0){
    Serial.println(readString);
    if (readString == "auto"){
      AutoMode = true;
    }
    if (readString == "bluetooth"){
      AutoMode = false;
    }
    if (readString == "pelan"){
      M1.setSpeed(Slow);
      M4.setSpeed(Slow);
      delay(50);
    }
    if (readString == "medium"){
      M1.setSpeed(Medium);
      M4.setSpeed(Medium);
      delay(50);
    }
    if (readString == "cepat"){
      M1.setSpeed(Fast);
      M4.setSpeed(Fast);
      delay(50);
    }
    if (readString == "F"){
      maju();
      delay(50);
    }
    if (readString == "B"){
      mundur();
      delay(50);
    }
    if (readString == "R"){
      kanan();
      delay(50);
    }
    if (readString == "L"){
      kiri();
      delay(50);
    }
    if (readString == "S"){
      berhenti();
      delay(50);
    }
    if (readString == "WATER ON"){
      pompaOn();
      delay(50);
    }
    if (readString == "WATER OFF"){
      pompaOff();
      delay(50);
    }
    if (readString == "move"){
      myservo.write(20);
      delay(800);
      myservo.write(160);
      delay(800);
      myservo.write(100);
      delay(800);
    }
     if (readString == "tidur"){
      berhenti();
      M1.setSpeed(0);
      M4.setSpeed(0);
      delay(2000);
    }
    if (readString == "bangun"){
      M1.setSpeed(Slow);
      M4.setSpeed(Slow);
      delay(50);
    }
    readString="";
  }  
}
