#include <Servo.h>
#include <string.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
using namespace std;

#define ScreenWidth 128 
#define ScreenHeight 32
#define ScreenAddress 0x3C
#define OledReset -1

Adafruit_SSD1306   LedDisplay(ScreenWidth, ScreenHeight, &Wire, OledReset);

Servo BaseMotor;
Servo ArmMotor;
Servo WristMotor;
Servo HandMotor;

const int BaseMotorPin  = 2;
const int ArmMotorPin   = 3;
const int WristMotorPin = 4;
const int HandMotorPin  = 5;
const int LedPin             = 6;
const int BuzzerPin          = 7;
const int SwitchPin          = 8;
const int LeftJoystickX      = A0;
const int LeftJoystickY      = A1;
const int RightJoystickX     = A2;
const int RightJoystickY     = A3;

int BaseMotorPosition = 90;
int ArmMotorPosition = 90;
int WristMotorPosition = 90;
int HandMotorPosition = 80;

void setup() {
  Serial.begin(9600);

  if(!LedDisplay.begin(SSD1306_SWITCHCAPVCC, ScreenAddress)){ Serial.println(F("Display Address incorrect")); for(;;); }

  BaseMotor.attach(BaseMotorPin);
  ArmMotor.attach(ArmMotorPin);
  WristMotor.attach(WristMotorPin);
  HandMotor.attach(HandMotorPin);

  pinMode(LedPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(SwitchPin, INPUT_PULLUP);
}

void loop(){
  WriteText();
  ControlBaseAndArm(5);
  ControlHandAndWrist(5);
  CheckPosition(5);
}

void ControlBaseAndArm(int Duration){
  if(JoystickLeft(LeftJoystickX, LeftJoystickY) == true && BaseMotorPosition > 0){
    BaseMotor.write(--BaseMotorPosition);
    delay(Duration);
  } 
  else if(JoystickRight(LeftJoystickX, LeftJoystickY) == true && BaseMotorPosition < 180){
    BaseMotor.write(++BaseMotorPosition);
    delay(Duration);
  } 
  else if(JoystickUp(LeftJoystickX, LeftJoystickY) == true && ArmMotorPosition < 180){
    ArmMotor.write(++ArmMotorPosition);
    delay(Duration);
  } 
  else if(JoystickDown(LeftJoystickX, LeftJoystickY) == true && ArmMotorPosition > 0){
    ArmMotor.write(--ArmMotorPosition);
    delay(Duration);
  }
}

void ControlHandAndWrist(int Duration){
  if(JoystickUp(RightJoystickX, RightJoystickY) == true && HandMotorPosition > 0){
    HandMotor.write(--HandMotorPosition);
    delay(Duration);
  } 
  else if(JoystickDown(RightJoystickX, RightJoystickY) == true && HandMotorPosition < 180){
    HandMotor.write(++HandMotorPosition);
    delay(Duration);
  } 
  else if(JoystickLeft(RightJoystickX, RightJoystickY) == true && WristMotorPosition < 180){
    WristMotor.write(++WristMotorPosition);
    delay(Duration);
  } 
  else if(JoystickRight(RightJoystickX, RightJoystickY) == true && WristMotorPosition > 0){
    WristMotor.write(--WristMotorPosition);
    delay(Duration);
  }
}

void CheckPosition(int Duration){
  if((JoystickLeft(LeftJoystickX, LeftJoystickY) == true) && (BaseMotorPosition == 0)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  }
  else if((JoystickLeft(RightJoystickX, RightJoystickY) == true) && (WristMotorPosition == 180)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  }
  else if((JoystickRight(LeftJoystickX, LeftJoystickY) == true) && (BaseMotorPosition == 180)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  }
  else if((JoystickRight(RightJoystickX, RightJoystickY) == true) && (WristMotorPosition == 0)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  } 
  else if((JoystickUp(LeftJoystickX, LeftJoystickY) == true) && (ArmMotorPosition == 180)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  }
  else if((JoystickUp(RightJoystickX, RightJoystickY) == true) && (HandMotorPosition == 0)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  } 
  else if((JoystickDown(LeftJoystickX, LeftJoystickY) == true) && (ArmMotorPosition == 0)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  }
  else if((JoystickDown(RightJoystickX, RightJoystickY) == true) && (HandMotorPosition == 180)){
    digitalWrite(LedPin, HIGH); digitalWrite(BuzzerPin, HIGH);
    delay(Duration);
  }
  else{ 
    digitalWrite(LedPin, LOW); digitalWrite(BuzzerPin, LOW);
  }
}

bool JoystickUp(int JoystickX, int JoystickY){
  if((analogRead(JoystickX) > 300 && analogRead(JoystickX) < 700) && analogRead(JoystickY) < 200){ return true; }
  else return false;
}
bool JoystickDown(int JoystickX, int JoystickY){
  if((analogRead(JoystickX) > 300 && analogRead(JoystickX) < 700) && analogRead(JoystickY) > 700){ return true; }
  else return false;
}
bool JoystickLeft(int JoystickX, int JoystickY){
  if(analogRead(JoystickX) > 700 && (analogRead(JoystickY) < 700 && analogRead(JoystickY) > 300)){ return true; }
  else return false;
}
bool JoystickRight(int JoystickX, int JoystickY){
  if(analogRead(JoystickX) < 200 && (analogRead(JoystickY) < 700 && analogRead(JoystickY) > 300)){ return true; }
  else return false;
}
bool JoystickMiddle(int JoystickX, int JoystickY){
  if((analogRead(JoystickX) > 495 && analogRead(JoystickX) < 520) && (analogRead(JoystickY) > 495 && analogRead(JoystickY) < 520)){ return true; }
  else return false;
}

void GarbagePickup(int Delay){
  AdjustAngle(BaseMotor, 90); delay(Delay); AdjustAngle(ArmMotor, 120); delay(Delay); AdjustAngle(WristMotor, 129); delay(Delay); AdjustAngle(HandMotor, 76); delay(Delay);

  AdjustAngle(ArmMotor, 74); delay(Delay); AdjustAngle(HandMotor, 56); delay(Delay); AdjustAngle(BaseMotor, 156); delay(Delay);

  AdjustAngle(ArmMotor, 126); delay(Delay); AdjustAngle(WristMotor, 120); delay(Delay); AdjustAngle(HandMotor, 77); delay(Delay);

  AdjustAngle(ArmMotor, 85); delay(Delay); AdjustAngle(BaseMotor, 0); delay(Delay); AdjustAngle(HandMotor, 43); delay(Delay);
}

void AdjustAngle(Servo ServoMotor, int Angle){
  int CurrentAngle = ServoMotor.read();
  if(Angle > CurrentAngle){
    for(int i=CurrentAngle; i<=Angle; i++){
      ServoMotor.write(i);
      delay(15);
    }
  }
  else if(CurrentAngle >= Angle){
    for(int i=CurrentAngle; i>=Angle; i--){
      ServoMotor.write(i);
      delay(15);
    }
  }
}

void WriteText(void){
  LedDisplay.clearDisplay();
  LedDisplay.setTextColor(SSD1306_WHITE);
  LedDisplay.setTextSize(1);         
  LedDisplay.setCursor(0,0);         
  LedDisplay.print("B: "); BaseMotorPosition = BaseMotor.read();
  LedDisplay.print(BaseMotorPosition);
  LedDisplay.print(" A: "); ArmMotorPosition = ArmMotor.read();
  LedDisplay.println(ArmMotorPosition);
  LedDisplay.print(" W: "); WristMotorPosition = WristMotor.read();
  LedDisplay.print(WristMotorPosition);
  LedDisplay.print(" H: "); HandMotorPosition = HandMotor.read();
  LedDisplay.println(HandMotorPosition);

  if(JoystickLeft(LeftJoystickX, LeftJoystickY) == true){ LedDisplay.println("PositionL: Left"); }
  else if(JoystickRight(LeftJoystickX, LeftJoystickY) == true){ LedDisplay.println("PositionL: Right"); }
  else if(JoystickUp(LeftJoystickX, LeftJoystickY) == true){ LedDisplay.println("PositionL: Up"); }
  else if(JoystickDown(LeftJoystickX, LeftJoystickY) == true){ LedDisplay.println("PositionL: Down"); }
  else if(JoystickMiddle(LeftJoystickX, LeftJoystickY) == true){ LedDisplay.println("PositionL: Middle"); }

  if(JoystickLeft(RightJoystickX, RightJoystickY) == true){ LedDisplay.println("PositionR: Down"); }
  else if(JoystickRight(RightJoystickX, RightJoystickY) == true){ LedDisplay.println("PositionR: Up"); }
  else if(JoystickUp(RightJoystickX, RightJoystickY) == true){ LedDisplay.println("PositionR: Left"); }
  else if(JoystickDown(RightJoystickX, RightJoystickY) == true){ LedDisplay.println("PositionR: Right"); }
  else if(JoystickMiddle(RightJoystickX, RightJoystickY) == true){ LedDisplay.println("PositionR: Middle"); }

  LedDisplay.display();
}