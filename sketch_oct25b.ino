// you can chage delay timings for a little bit different result
#include<Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
hd44780_I2Cexp lcd; //initializing lcd

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; 

int minVal=265; int maxVal=402;

String phrases[] = {"It is certain", "Certainly", "Without a doubt", "You may rely on it", "As I see it, yes", "Most likely", "Outlook good", "Yes", "My reply is no", "Very doubtful xD", "No", "No chance", "No way)", "Cannot predictt",
"I doubt it", "Ask again", "Not sure"}; // response phrases 

int numberofphrases = 17;

String output = "";

double x; 
double y;
double z;

double x1;
double y1;
double z1;

void setup(){ 
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status)
  {
    status = -status;
    
    hd44780::fatalError(status);
  }
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Ask the ball");
delay(2000);
lcd.setCursor(0, 1);
lcd.print("...and shake it");
}

void loop(){
//getting the initial position
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);

x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

delay(500);

//getting the updated position
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int x1Ang = map(AcX,minVal,maxVal,-90,90);
int y1Ang = map(AcY,minVal,maxVal,-90,90);
int z1Ang = map(AcZ,minVal,maxVal,-90,90);

x1= RAD_TO_DEG * (atan2(-y1Ang, -z1Ang)+PI);
y1= RAD_TO_DEG * (atan2(-x1Ang, -z1Ang)+PI);
z1= RAD_TO_DEG * (atan2(-y1Ang, -x1Ang)+PI);

if (x1 - x > 30 || y1 - y > 30 || z1- z > 30){ // comparing initial position with the new one
  
//you can also have some fun with the angles depending on how hard you want to shake it

output = phrases[random(numberofphrases)];

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("The ball said:");
lcd.setCursor(0, 1);
lcd.print(output);
delay(3000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Ask again");
lcd.setCursor(0, 1);
lcd.print("..go for it..");
delay(1000);
}
delay(1000);
}
