#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <AccelStepper.h>


#define SERVOMIN  110 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  485 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//setting up stepper motor values
int stepPin = 2;
int dirPin = 3;
int stepPin2 = 4;
int dirPin2 = 5;
int stepsPerRev = 200;
AccelStepper RightStepper(1,stepPin,dirPin);
AccelStepper LeftStepper(1,stepPin2,dirPin2);


                  //0   1   2  3    4   5    6    7    8   9   10  11         
int minServo[12] = {0,  180,45, 0, 0,  135, 0,   180, 45,  0,  0,  135};
int maxServo[12] = {80 ,95, 115,90 ,75,90,  100, 60,  110,100,110,95};

String moves[] = {"R","U","L\'","U\'"}; //Array of moves for the robot to execute
int rightGrip = 0; //position of right wrist
int rightGripTravel = 100; //Distance for rightGripping as a percentage of max rightGrip

int leftGrip = 0; //position of right wrist
int leftGripTravel = 100; //Distance for rightGripping as a percentage of max rightGrip
int movesSize = sizeof(moves)/sizeof(moves[0]); //calculating number of moves to execute


/*0 right index 1
 *1 right index 2 
 *2 right middle 1 
 *3 right ring 1
 *4 right ring 2
 *5 right thumb 1
 *6 left index 1
 *7 left index 2 
 *8 left middle 1 
 *9 left ring 1
 *10 left ring 2
 *11 left thumb 1
 */ 

void setup() {
  Serial.begin(9600);

  digitalWrite(4,LOW);//MS1
  digitalWrite(5,LOW);//MS2
  RightStepper.setMaxSpeed(2000);
  RightStepper.setAcceleration(2000);
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
}

void loop() {
//setting grip 
for (int i = 0;i<12;i++){
     fingerTravel(i,0);
     }
   delay(1000);
   
// Executing moves in array:
while(true){
  for (int i = 0;i<movesSize;i++){
    execute(moves[i]);
    delay(500);
  }
}
  delay(1000);
  exit(0);
}


//-----FUNCTION DEFINITIONS-----//

//Converts servo signal to degrees
 int angleToPulse(float ang){
 int pulse = map(ang,0,180,SERVOMIN,SERVOMAX);
 return pulse;
}

//Travel to a certain position relative to min and max position. 0 = min position, 100 = max position
float fingerTravel(int servoNumber, float portion){
  float travel = map(portion,0,100,angleToPulse(minServo[servoNumber]),angleToPulse(maxServo[servoNumber]));
  pwm.setPWM(servoNumber, 0, travel);
  return travel;
}

//Sets position of right wrist. -50 = thumb on bottom, 50 = thumb on top
void setRightGrip(int rightGripNumber){
  if(rightGripNumber == rightGrip){
    delay(100);
     fingerTravel(2,rightGripTravel);  
     fingerTravel(5,rightGripTravel);
    return;
  }
  else if(rightGripNumber < -50 || rightGripNumber > 50){
    Serial.println("Grip went out of bounds");
    return;
  }
  else{
     for (int i = 0;i<6;i++){
     fingerTravel(i,0);
     }
     delay(500);
  }
   RightStepper.moveTo(rightGripNumber);
   while (RightStepper.distanceToGo() != 0) {
   RightStepper.run();
   }
   rightGrip = rightGripNumber; //refer to rightGrip later
   delay(100);
     fingerTravel(2,rightGripTravel);  
     fingerTravel(5,rightGripTravel);
   delay(100);
}


void setRightGripR(int rightGripNumber){
  if(rightGripNumber == rightGrip){
    delay(100);
     fingerTravel(0,70);
     fingerTravel(1,0);   
     fingerTravel(3,70);
     fingerTravel(4,0); 
     fingerTravel(5,70);
     delay(500);
     fingerTravel(2,0); 
     delay(1000);
    return;
  }
  else if(rightGripNumber < -50 || rightGripNumber > 50){
    Serial.println("Grip went out of bounds");
    return;
  }
  else{
     for (int i = 0;i<6;i++){
     fingerTravel(i,0);
     }
     delay(500);
  }
   RightStepper.moveTo(rightGripNumber);
   while (RightStepper.distanceToGo() != 0) {
   RightStepper.run();
   }
   rightGrip = rightGripNumber; //refer to rightGrip later
   delay(100);
     fingerTravel(0,70);
     fingerTravel(1,0);   
     fingerTravel(3,70);
     fingerTravel(4,0); 
     fingerTravel(5,70);
     delay(500);
     fingerTravel(2,0); 
   delay(500);
}

void setRightGripL(int leftGripNumber){
  if(leftGripNumber == leftGrip){
    delay(100);
     fingerTravel(6,70);
     fingerTravel(7,0);   
     fingerTravel(9,70);
     fingerTravel(10,0); 
     fingerTravel(11,70);
     delay(500);
     fingerTravel(8,0); 
     delay(1000);
    return;
  }
  else if(leftGripNumber < -50 || leftGripNumber > 50){
    Serial.println("Grip went out of bounds");
    return;
  }
  else{
     for (int i = 0;i<6;i++){
     fingerTravel(i,0);
     }
     delay(500);
  }
   LeftStepper.moveTo(leftGripNumber);
   while (LeftStepper.distanceToGo() != 0) {
   LeftStepper.run();
   }
   leftGrip = leftGripNumber; //refer to rightGrip later
    delay(100);
     fingerTravel(6,70);
     fingerTravel(7,0);   
     fingerTravel(9,70);
     fingerTravel(10,0); 
     fingerTravel(11,70);
     delay(500);
     fingerTravel(8,0); 
     delay(500);
}

//Sets position of left wrist. -50 = thumb on bottom, 50 = thumb on top
void setLeftGrip(int leftGripNumber){
  if(leftGripNumber == leftGrip){
     delay(100);
     fingerTravel(8,leftGripTravel);  
     fingerTravel(11,leftGripTravel);
    return;
  }
  else if(leftGripNumber < -50 || leftGripNumber > 50){
    Serial.println("Grip went out of bounds");
    return;
  }
  else{
     for (int i = 0;i<6;i++){
     fingerTravel(i,0);
     }
     delay(100);
  }
   LeftStepper.moveTo(leftGripNumber);
   while (RightStepper.distanceToGo() != 0) {
   LeftStepper.run();
   }
   leftGrip = leftGripNumber; //refer to rightGrip later
   delay(100);
     //fingerTravel(6,leftGripTravel);
     //fingerTravel(7,leftGripTravel);
     fingerTravel(8,leftGripTravel);  
     //fingerTravel(9,leftGripTravel);
     //fingerTravel(10,leftGripTravel); 
   delay(100);
 
}
//Executes turns. Input is an array of strings which list the moves
void execute(String turn) {
 if (turn == "U"){
    delay(200);
    setRightGrip(0);
    setLeftGrip(0);
    delay(500);
    fingerTravel(0, 100);
    fingerTravel(1, 100);
    delay(500);
    fingerTravel(0, 0);
    delay(300);
    fingerTravel(1, 0);
  }
 else if (turn == "D"){
    delay(200);
    setRightGrip(0);
    setLeftGrip(0);
    delay(500);
    fingerTravel(6, 100);
    fingerTravel(7, 100);
    delay(500);
    fingerTravel(6, 0);
    delay(300);
    fingerTravel(7, 0);
 }

 else if (turn == "R"){
   setRightGripR(0);
   setLeftGrip(0);
   delay(1000);
   rightGrip = rightGrip+50;
   RightStepper.moveTo(rightGrip);
   while (RightStepper.distanceToGo() != 0) {
   RightStepper.run();
   }
 }

 else if (turn == "L"){
   setRightGripL(0);
   setLeftGrip(0);
   delay(1000);
   leftGrip = leftGrip+50;
   LeftStepper.moveTo(leftGrip);
   while (LeftStepper.distanceToGo() != 0) {
   LeftStepper.run();
   }
 }
 
 else if (turn == "F"){
    delay(200);
    setRightGrip(-50);
    setLeftGrip(0);
    delay(500);
    fingerTravel(9, 100);
    fingerTravel(10, 100);
    delay(500);
    fingerTravel(9, 0);
    delay(300);
    fingerTravel(10, 0);
 }

 else if (turn == "B"){
    delay(200);
    setRightGrip(0);
    setLeftGrip(50);
    delay(500);
    fingerTravel(6, 100);
    fingerTravel(7, 100);
    delay(500);
    fingerTravel(6, 0);
    delay(300);
    fingerTravel(7, 0);
 }
  

 else if (turn == "U\'"){
    delay(200);
    setRightGrip(0);
    setLeftGrip(0);
    delay(500);
    fingerTravel(9, 100);
    fingerTravel(10, 100);
    delay(500);
    fingerTravel(9, 0);
    delay(300);
    fingerTravel(10, 0);
 }

 else if (turn == "D\'"){
    delay(200);
    setRightGrip(0);
    setLeftGrip(0);
    delay(500);
    fingerTravel(3, 100);
    fingerTravel(4, 100);
    delay(500);
    fingerTravel(3, 0);
    delay(300);
    fingerTravel(4, 0);
 }

 else if (turn == "R\'"){
   setRightGripR(0);
   rightGrip = rightGrip-50;
   RightStepper.moveTo(rightGrip);
   while (RightStepper.distanceToGo() != 0) {
   RightStepper.run();
   }
 }

 else if (turn == "L\'"){
   setRightGripL(0);
   setLeftGrip(0);
   delay(1000);
   leftGrip = leftGrip-50;
   LeftStepper.moveTo(leftGrip);
   while (LeftStepper.distanceToGo() != 0) {
   LeftStepper.run();
 }
 }
 
 else if (turn == "F\'"){
    delay(200);
    setRightGrip(0);
    setLeftGrip(50);
    delay(500);
    fingerTravel(9, 100);
    fingerTravel(10, 100);
    delay(500);
    fingerTravel(9, 0);
    delay(300);
    fingerTravel(10, 0);
 }
    
 else if (turn == "B\'"){
    delay(200);
    setLeftGrip(0);
    delay(500);
    fingerTravel(3, 100);
    fingerTravel(4, 100);
    delay(500);
    fingerTravel(3, 0);
    delay(300);
    fingerTravel(4, 0);
 }
 else {
  Serial.print("invalid move");
  }
}
