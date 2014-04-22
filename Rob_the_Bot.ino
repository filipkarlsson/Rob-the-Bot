#include <DistanceGP2Y0A21YK.h>
#include <QTRSensors.h>
#include <Servo.h> 


//Edge sensors------------

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white 
// paper and sliding the sensor across it.  It prints the sensor values to the serial 
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).


#define NUM_SENSORS   4     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2

// sensors 0 through 1 are connected to digital pins 3 through 4, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
 
boolean out_front_right = false; 
boolean out_front_left = false; 
boolean out_back_right = false; 
boolean out_back_left = false; 

//-----------------------

//  Analog distance sensors----
DistanceGP2Y0A21YK left;
DistanceGP2Y0A21YK front_left;
DistanceGP2Y0A21YK front_right;
DistanceGP2Y0A21YK right;
DistanceGP2Y0A21YK back;

int d_left, d_front_left, d_front_right, d_right, d_back;


// Start module ------

int start_pin = 7;


//---------------------

Servo motor_right;
Servo motor_left;

int forward = 110; 
int backward = 70;
int still = 90;

Servo servo;
int servo_down = 161;
int servo_up = 40;

void setup()
{
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  motor_right.attach(11);
  motor_left.attach(10);
  
  pinMode(start_pin, INPUT);
  
  left.begin(A0);
  front_left.begin(A1);
  front_right.begin(A2);
  right.begin(A3);
  back.begin(A4);
    
}


void loop()
{
  while(!digitalRead(start_pin))
  {
    delay(1);
  }
  
  servo.write(servo_down);
  delay(1500);
  
  motor_right.write(forward);
  motor_left.write(backward);
  delay(4000);
  motor_right.write(forward);
  motor_left.write(forward);
  
  while(true){
  // read raw sensor values
      qtrrc.read(sensorValues);   
    
      // sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
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
        
        
  //read distance sensors
      d_left = left.getDistanceCentimeter();
      d_front_left = front_left.getDistanceCentimeter();
      d_front_right = front_right.getDistanceCentimeter();
      d_right = right.getDistanceCentimeter();
      d_back = back.getDistanceCentimeter();
      
  //--------------------------------------------------------------------

  
      
  // Action :D --------------------
      //----- edge ----
      if(out_front_left){
        motor_right.write(backward);
        motor_left.write(backward - 10); //faster
        delay(300);
        out_front_left = false;
      }
       else if(out_front_right){
        motor_right.write(backward - 10); //faster
        motor_left.write(backward);
        delay(300);
        out_front_right = false;
      }
         else if(out_back_left){
        motor_right.write(forward);
        motor_left.write(forward);
        //delay(300);
        out_back_left = false;
      }
         else if(out_back_right){
        motor_right.write(forward);
        motor_left.write(forward);
        //delay(300);
        out_back_right = false;
      }
      
      else{
      motor_right.write(forward);
      motor_left.write(forward);
      }
      
      
      
     //------------ opponent---------
 
     if((d_front_left < 70) && (d_right < 70)){  //If opponent is right infront go forward as long as opponent is in right in front
       while((d_front_left < 70) && (d_right < 70)){
         motor_right.write(forward);
         motor_left.write(forward);
       }
     }
     
     if(d_left < 70){
     motor_right.write(forward);
     motor_left.write(backward);
     delay(500);
     }
     
     if(d_right < 70){
     motor_right.write(backward);
     motor_left.write(forward);
     delay(500);
     }
     
     if(d_back < 70){
     motor_right.write(forward);
     motor_left.write(backward);
     delay(1000);
     }
     

  }
}
