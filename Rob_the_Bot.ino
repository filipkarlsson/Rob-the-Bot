#include <DistanceGP2Y0A21YK.h>
#include <QTRSensors.h>
#include <Servo.h> 


//Edge sensors------------

#define NUM_SENSORS   4     // number of sensors used
#define TIMEOUT       1500  // waits for 1500 microseconds for sensor outputs to go low
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

int fast_forward = 180; 
int fast_backward = 0;
int forward = 140;
int backward = 40;
int still = 90;

int react_dist = 50;
int back_delay = 350;

Servo servo;
int servo_down = 2200; 
int servo_up = 1100;


void front_check();
void back_check();
void turn_180();
void turn_left();
void turn_right();

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
  
  servo.writeMicroseconds(servo_up); //Set servo to upright position

  while(!digitalRead(start_pin)) //wait for start signal
  {
      //read sensors while waiting
      d_left = left.getDistanceCentimeter();
      d_front_left = front_left.getDistanceCentimeter();
      d_front_right = front_right.getDistanceCentimeter();
      d_right = right.getDistanceCentimeter();
      d_back = back.getDistanceCentimeter();
  }
  
  servo.writeMicroseconds(servo_down);

  
    turn_180();
  
  
   while(digitalRead(start_pin)){ //main loop, run until no start signal

        
    
     //------------ opponent---------

     //read distance sensors
      d_left = left.getDistanceCentimeter();
      d_front_left = front_left.getDistanceCentimeter();
      d_front_right = front_right.getDistanceCentimeter();
      d_right = right.getDistanceCentimeter();
      d_back = back.getDistanceCentimeter();
      


 
     if((d_front_left < react_dist) && (d_front_right < react_dist)){  //If opponent is right infront go forward as long as opponent is in right in front
       while((d_front_left < react_dist) && (d_front_right < react_dist)){
        motor_right.write(fast_forward);
        motor_left.write(fast_forward);
   
        d_front_left = front_left.getDistanceCentimeter();
        d_front_right = front_right.getDistanceCentimeter();

        front_check();
        back_check();

       }
     }
     //_______________
     
     
     else if((d_front_left < react_dist) && (d_front_right > react_dist)){ //opponenet is left-front
           
           motor_right.write(fast_forward);
           motor_left.write(forward-10);
           while(d_front_left < d_front_right){
                 d_front_left = front_left.getDistanceCentimeter();
                 d_front_right = front_right.getDistanceCentimeter();
                 front_check();

           }
           
         motor_right.write(forward);
         motor_left.write(forward);
           
     }
     //-------------------------------------------------------
      else if((d_front_right < react_dist) && (d_front_left > react_dist)){ //opponenet is right-front
           
           motor_left.write(fast_forward);
           motor_right.write(forward-10);
           while(d_front_right < d_front_left){
                 d_front_left = front_left.getDistanceCentimeter();
                 d_front_right = front_right.getDistanceCentimeter();
                 front_check();

           }
           
         motor_right.write(forward);
         motor_left.write(forward);
           
     }
     
     //-------------------------------------------------------
     
     
      else if(d_left < react_dist){ //opponenet is to the left
          
        turn_left();
           }
          
     
     //____________________________________________
     
     
     
     
     else if(d_right < react_dist){ //opponenet is to the right
          turn_right();
     }
     
    //-------------------------------------------------------------------
    
    
   else if(d_back < react_dist){ //opponenet is behind
 
         turn_180();
     }
     
     else{   //opponent is not detcted, turn until otherwise
        motor_right.write(backward);
        motor_left.write(fast_forward);
        while((d_left > react_dist) && (d_back > react_dist) && (d_right > react_dist) && (d_front_left > react_dist) && (d_front_right > react_dist)){
          d_left = left.getDistanceCentimeter();
          d_front_left = front_left.getDistanceCentimeter();
          d_front_right = front_right.getDistanceCentimeter();
          d_right = right.getDistanceCentimeter();
          d_back = back.getDistanceCentimeter();
          front_check();

        }
        motor_right.write(still);
        motor_left.write(still);
        delay(10);
     }
     

  }
  motor_right.write(still); //when no start signal stop, not neccesary but nice while testing
  motor_left.write(still);
  
}





void front_check(){

//check for white line in front
      qtrrc.read(sensorValues);

      if(sensorValues[0] < 1000){ //if over the line front-left go back for 350 ms
          motor_right.write(backward);
          motor_left.write(fast_backward);
          delay(back_delay);
          back_check();
        }
        else if(sensorValues[1] < 1000){
          motor_right.write(fast_backward);
          motor_left.write(backward);
          delay(back_delay);

          back_check();

        }

        return;
}

void back_check(){

//check for white line in front
      qtrrc.read(sensorValues);
        /*if((sensorValues[2] < 1000) && (sensorValues[3] < 1000)){
          motor_right.write(backward+20);
          motor_left.write(fast_backward);
          delay(back_delay);

        }

        else*/ if(sensorValues[2] < 1000){ //if over the line back-left go back for 350 ms
          turn_right();
          motor_right.write(forward);
          motor_left.write(forward);
          delay(back_delay);
        }
        else if(sensorValues[3] < 1000){
          turn_left();
          motor_right.write(forward);
          motor_left.write(forward);
         delay(back_delay);
        }

        return;
}
   

void turn_180(){
  motor_right.write(180);
  motor_left.write(0);

  delay(250);
  motor_left.write(forward);
  motor_right.write(backward);
  return;
}  

void turn_right(){
  motor_right.write(0);
  motor_left.write(180);

  delay(135);
  motor_left.write(forward);
  motor_right.write(forward);
  return;
} 

void turn_left(){
  motor_right.write(180);
  motor_left.write(0);

  delay(135);
  motor_left.write(forward);
  motor_right.write(forward);
  return;
} 
