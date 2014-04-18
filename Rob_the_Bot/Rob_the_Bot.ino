#include <QTRSensors.h>
#include <Servo.h> 
#include <DistanceGP2Y0A21YK.h>
// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white 
// paper and sliding the sensor across it.  It prints the sensor values to the serial 
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).


#define NUM_SENSORS   4     // number of QTR sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2
#define NUM_ANALOG    5     //number of sharp analog sensors


// sensors 0 through 1 are connected to digital pins 3 through 4, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
 
boolean out_front_right = false; 
boolean out_front_left = false; 
boolean out_back_right = false; 
boolean out_back_left = false; 

Servo motor_right;
Servo motor_left;
Servo servo;
int servo_down = 161;
int servo_up = 40;



//Sharp analog distance sensors
char front_left = A0; //connected to pin A0
char front_right = A1M;
char side_left = A2;
char side_right = A3;
char back = A4;

DistanceGP2Y0A21YK dist(unsigned char[] {front_left, front_right, side_left, side_right, back};
int distance[NUM_ANALOGA];


void setup()
{
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  motor_right.attach(11);
  motor_left.attach(10);
  Serial.begin(9600)
  dist.begin(A0);
  dist.begin(A1);
  dist.begin(A2);
  dist.begin(A3);
  dist.begin(A4);
}



void loop()
{
  delay(2000);
  servo.write(servo_down);
  delay(1500);
  
  motor_right.write(100);
  motor_left.write(100);
  delay(5000);
  motor_right.write(75);
  motor_left.write(115);
  
  while(true){
  // read raw sensor values
  qtrrc.read(sensorValues);

  //will make sure that all distance are equal to 0 again
  for(int i = 0, i<5, i++){
   distance[i] = 0;
  }
   //reads the distances in centimeter
  dist.getDistanceCentimeter(distance);
  
  
  //tactics for different distance values  
  if(distance[0] > 0 and distance[1] > 0 and sensorValues > 1000){
  motor_right.write(0);
  motor_left.write(180);
  delay(300);
  }
  else if(distance[0] > 0 and sensorValues > 1000){
  motor_right.write(145);
  motor_left.write(145);
  delay(300);
  motor_right.write(180);
  motor_left.write(0);
  }
   else if(distance[1] > 0 and sensorValues > 1000){
  motor_right.write(45);
  motor_left.write(45);
  delay(300); 
  motor_right.write(180);
  motor_left.write(0);
  }
    else if(distance[2] > 0 and sensorValues > 1000){
  motor_right.write(180);
  motor_left.write(180);  
  delay(300); 
  motor_right.write(180);
  motor_left.write(0);
  }
   else if(distance[3] > 0 and sensorValues > 1000){
  motor_right.write(0);
  motor_left.write(0); 
  delay(300);
  motor_right.write(180);
  motor_left.write(0);
  
  }
  else if (distance[4] > 0 and sensorValues > 1000){
   motor_right.write(180);
  motor_left.write(0);
  delay(300)
  motor_right.write(180);
  motor_left.write(180);    
  }
  
  // print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
  // 1023 means minimum reflectance
  
    if(sensorValues[0] < 1000){
      out_front_left = true;   
    }
    if(sensorValues[1] < 1000){
      out_front_right = true;   
    }
    if(sensorValues[2] < 1000){
      out_back_left = true;   
    }
    if(sensorValues[3] < 1000){
      out_back_right = true;   
    }
    
  
  if(out_front_left){
    motor_right.write(100);
    motor_left.write(40);
    delay(300);
    out_front_left = false;
  }
   else if(out_front_right){
    motor_right.write(120);
    motor_left.write(80);
    delay(300);
    out_front_right = false;
  }
     else if(out_back_left){
    motor_right.write(80);
    motor_left.write(100);
    delay(300);
    out_back_left = false;
  }
     else if(out_back_right){
    motor_right.write(80);
    motor_left.write(100);
    delay(300);
    out_back_right = false;
  }
  
  else{
  motor_right.write(75);
  motor_left.write(113);
  }
  

  }
}
