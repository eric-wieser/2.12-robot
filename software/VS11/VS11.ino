#include <Servo.h>
//global declarations
#define SERVO1 4        //define a pin for servo1
#define SERVO2 5        // define pin for servo2
Servo myservo1;        // initialize  servo1
Servo myservo2;        // initialize servo2

// values that sets servo in different positions
const int neutral = 1500;  // rest position of servo
const int forward = 2000;  // forward rotation of servo
const int backward = 1000;  // backward rotation of servo
  
//--- Function: Setup ()
void setup()
{	
  pinMode (SERVO1, OUTPUT);	// want servo1 pin to be an output
  pinMode(SERVO2, OUTPUT);      // want servo2 pin to be output
  myservo1.attach(SERVO1);	// attach pin to the servo1 
  myservo1.writeMicroseconds(neutral);  // set servo1 to mid-point
  myservo2.attach(SERVO2);      // attach pin to servo2
  myservo2.writeMicroseconds(neutral);  // set servo2 to mid-point
}
//--- Function: loop ()
void loop()
{
  myservo1.writeMicroseconds(forward); // set servo1 to forward pulse
  myservo2.writeMicroseconds(backward); // set servo2 to forward pulse
                                    //(this servo has opposite values)
  delay(6000);		// Pauses the program for 6sec
  myservo1.writeMicroseconds(neutral);  // set servo1 to mid-point
  delay(1400);		// Pauses the program for 14sec
  myservo1.writeMicroseconds(forward);  // set servo1 to backward
  delay(12000);         // Pause the program for 12sec
  while(1) {            // keep program running but not doing anything
    myservo1.writeMicroseconds(neutral);
    myservo2.writeMicroseconds(neutral);
  }                                        
}

