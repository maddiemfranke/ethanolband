#include <AccelStepper.h>
#include <MultiStepper.h>


#include <AccelStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 9, 8);//    rotate
AccelStepper stepper2(AccelStepper::DRIVER, 7, 6);//  vertical
AccelStepper stepper3(AccelStepper::DRIVER, 11, 10);//  rack&Pinion

//PUMPS
const int blue =2;
const int orange =3;
const int green =4;
const int purple =5;

char command='a';
bool read_diode=0;
int trans_diode_pos[5]={38,76,114,152};
void setup()
{  Serial.begin(9600);
   stepper1.setMaxSpeed(200);
   stepper1.setSpeed(100);
   stepper2.setMaxSpeed(200);
   stepper2.setSpeed(100);  
   stepper3.setMaxSpeed(200);
   stepper3.setSpeed(100); 
   Serial.println("let's go\n");

   pinMode(blue, OUTPUT);
   pinMode(orange, OUTPUT);
   pinMode(green, OUTPUT);
   pinMode(purple, OUTPUT);
}

void loop()
{  
  if (Serial.available()>0){
    command = Serial.read();
    switch(command){
      case 'u':
        vertical(-500);
        Serial.println("up 100");
        break;
      case 'd':
        vertical(500);
        Serial.println("down 100");
        break;
      case 'r':
        Serial.println("running diodes...");
        trans_read();
        break;
      case 'l':
        Serial.println("next LED");
        rotate(8);
        break;
      case 'p':
        Serial.println("pump");
        run_pump(blue,1000);
        break;
    }
  }  
}

void vertical(int steps){
  stepper2.move(steps);
  stepper2.setSpeed(100);
  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) 
      {
        stepper1.runSpeedToPosition();
        stepper2.runSpeedToPosition();
      }
}
void rotate(int steps){
  stepper2.move(-steps);
  stepper1.move(steps);
  stepper2.setSpeed(50);
  stepper1.setSpeed(50);
  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) 
      {
        stepper1.runSpeedToPosition();
        stepper2.runSpeedToPosition();
      }
}
void trans_read(){
  
  for(int diode=0; diode<5; diode++){
    stepper3.moveTo(trans_diode_pos[diode]);
    stepper3.setSpeed(100);
    while (stepper3.distanceToGo() != 0){
        stepper3.runSpeedToPosition();
    }
    Serial.println(trans_diode_pos[diode]);
    delay(800);
    
  }
}

void onestep(){
  int i=0;
  bool stop=false;
  while( stop != true){
     if (Serial.available()>0){
        if (Serial.read()=='s'){
        stop=true;
        }
     }
     stepper3.move(1);
     while (stepper3.distanceToGo() != 0){
        stepper3.runSpeedToPosition();
      }
      i++;
      Serial.println(i);
      delay(500);
  }
}

void run_pump(int pump, int timeOn){
  //pumps run at 100 ml/min or 1.6667 ml/sec
  //timeOn in milliseconds
  digitalWrite(pump,HIGH);
  delay(timeOn);
  digitalWrite(pump,LOW);
  
}
  

