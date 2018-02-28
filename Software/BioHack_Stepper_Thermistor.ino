/*
  Written By Brandon Wade 
  
  A program written using the AnalogRead and Stepper examples located in the Arduino Library and other sources(linked below) to showcase the BioHack Boards Functionality.

  This Code Currently Contains two functions:

  readTempature = read the tempature values from a thermistor
  tempatureDriver = drive a steeper motor using tempatures from a thermistor

  * Please Note some portions of this code is used from the Arduino "stepper_oneStepAtATime" and "Analog Input" examples from the arduino.
  * I just modified the example sketches as well added the steinhart equation from the links below to make the code easy to be interfaced with. 

  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
  http://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
  Steinharts Equation: http://www.thinksrs.com/downloads/PDFs/ApplicationNotes/LDC%20Note%204%20NTC%20Calculator.pdf and http://www.skyeinstruments.com/wp-content/uploads/Steinhart-Hart-Eqn-for-10k-Thermistors.pdf
*/

#include <Stepper.h>

// Intial Values read from Thermistor
int Vo;
// Resitstor Value 10k is recommended
float R1 = 10000;

float logR2;
//calculated reistance
float R2;

//Variable used to store tempature
float tempature;

//c1, c2 and c3 are from Steinharts equation to calculate tempature
float c1 = 1.009249522e-03;
float c2 = 2.378405444e-04; 
float c3 = 2.019202697e-07;

// change this to fit the number of steps per revolution
const int stepsPerRevolution = 200;  

int a = 0;


// number of steps the motor has taken
int stepCount = 0;        

//connect pin 4 to enable
int stepStart = 4;
//connect pin 7 to step
int stepGo = 7;  
// connect pin 8 to direction
int stepDirection = 8;  

bool changeDirection;

// the setup routine runs once when you press reset:
void setup() 
{
  //pinMode(stepStart,OUTPUT);
  pinMode(stepGo, OUTPUT);
  pinMode(stepDirection, OUTPUT);   
  

  changeDirection = false; //stepper will start going clockwise
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}


void loop() 
{
  //testStepperMotor(); // test the stepper motor by making it rotate in both directions
  readTempature(); // read tempatures from the thermistor
 // tempatureDriver();//updates changeDirection accordingly based on temapture 

  
 /* Currently the stepper motor doesnt work with the BioHack board     
  *  updateStepperMotor(changeDirection) is called after tempatureDriver
  *  function updates change direction accordingly                        
  */
   //updateStepperMotor(changeDirection); // tells stepperMotor what to do
  
}
/*
 * Goes up to 195 degrees in boiling in water
 *  27 degrees in ice
 */
/****************************************************/
/* Read the tempatures from a Thermistor            */
/* . Goes up to 195 degrees in boiling water        */
/* . Goes up 27 degrees in ice                      */
/***************************************************/
void readTempature()
{
  int i;
  
  // read the input on analog pin 0:
  Vo = analogRead(A0);
  
  //Steinharts Equation
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  tempature = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  
  //convert the tempature to fahreiheit
  tempature = tempature - 273.15;
  tempature = (tempature * 9.0)/ 5.0 + 32.0; 
  
  //print the values to the serial port
  Serial.print("Temperature: "); 
  Serial.print(tempature);
  Serial.println(" F"); 

  delay(1000);

}
/**************************************************************/
/** Drive the stepper motor using values from readTempature  **/
/**************************************************************/
void tempatureDriver()
{
   Serial.println(tempature);
   if(tempature < 100)
   {
     changeDirection = true; // stepper motor will go clockwise
     Serial.println("going clockwise");
   }

   else if(tempature > 100)
   {
      changeDirection = false; // stepper motor will go counter clockwise
      Serial.println("going counter clockwise");
   }
}
/********************************************************/
/* sends a bool to tell stepper motor to turn right or left*/
/*******************************************************/
void updateStepperMotor(bool isRight)
{
   /** tell stepper to go clockwise **/
   if(isRight == true)
   {
       digitalWrite(stepDirection,LOW); 
       delay(10);
       digitalWrite(stepGo,HIGH);
       delay(10);
       digitalWrite(stepGo,LOW); 
       delay(10);
   }

   /* counter clocker wise **/
   else if(isRight == false)
   {
     digitalWrite(stepDirection,HIGH);
     delay(10);
     digitalWrite(stepGo,HIGH);
     delay(10);
     digitalWrite(stepGo,LOW);
     delay(10); 
   }
   
}
/*****************************************************/
/* Causes the Steeper Motor to alternate directions */
/****************************************************/
void testStepperMotor()
{
  if(stepCount < 200)
  {
     changeDirection = true;
  }

  else if( stepCount > 200 && stepCount < 400)
  {
    changeDirection = false;
  }

  else if(stepCount > 400)
  {
     stepCount = 0;
     changeDirection = true;
  }

    stepCount++;
}
/**************************************************/
/**************************************************/
  
   

   
  







