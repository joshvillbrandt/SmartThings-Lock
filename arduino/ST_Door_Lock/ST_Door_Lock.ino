//******************************************************************************************
//  File: ST_Door_Lock.ino
//  Authors: Josh Villbrandt
//
//  Change History:
//
//    Date        Who              What
//    ----        ---              ----
//    2015-03-13  Josh Villbrandt  Inital script]
//
//******************************************************************************************

//******************************************************************************************
// SmartThings Library for Arduino Shield
//******************************************************************************************
#include <SoftwareSerial.h> //Arduino UNO/Leonardo uses SoftwareSerial for the SmartThings Library
#include <SmartThings.h>    //Library to provide API to the SmartThings Shield
#include <dht.h>            //DHT Temperature and Humidity Library
#include <RCSwitch.h>       //Library to provide support for RCSwitch devices
//******************************************************************************************
// ST_Anything Library
//******************************************************************************************
#include <Constants.h>       //Constants.h is designed to be modified by the end user to adjust behavior of the ST_Anything library
#include <Device.h>          //Generic Device Class, inherited by Sensor and Executor classes
#include <Sensor.h>          //Generic Sensor Class, typically provides data to ST Cloud (e.g. Temperature, Motion, etc...)
#include <InterruptSensor.h> //Generic Interrupt "Sensor" Class, waits for change of state on digital input
#include <PollingSensor.h>   //Generic Polling "Sensor" Class, polls Arduino pins periodically
#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications

#include <IS_Motion.h>       //Implements an Interrupt Sensor (IS) to detect motion via a PIR sensor
#include <IS_Contact.h>      //Implements an Interrupt Sensor (IS) to monitor the status of a digital input pin
#include <IS_DoorControl.h> //Implements an Interrupt Sensor (IS) and Executor to monitor the status of a digital input pin and control a digital output pin

// pins
#define PIN_REED_DOOR     7
#define PIN_REED_LOCK     8
#define PIN_SERVO         9
#define PIN_BUZZER        10
#define PIN_MOTION        11

// buzzer
#define BUZZER_HIGH       294 // NOTE_D4
#define BUZZER_LOW        147 // NOTE_D3
#define BUZZER_DURATION   125 // ms

// servo
#include <Servo.h>
Servo servo;
#define SERVO_NEUTRAL     73  // deg
#define SERVO_LOCK        140 // deg
#define SERVO_UNLOCK      30  // deg
#define SERVO_DELAY       500 // ms

// timeouts
#define MOTION_TIMEOUT    1000 // ms
#define UNLOCK_TIMEOUT    5000 // ms
#define CLOSE_TIMEOUT     3000 // ms

// sensors
st::IS_Contact sensor1("door", PIN_REED_DOOR, LOW, true);
st::IS_Contact sensor2("lock", PIN_REED_LOCK, LOW, true);
st::IS_Motion sensor3("motion", PIN_MOTION, HIGH, false);
//st::EX_Switch executor1("sampleEX", PIN_sampleEX, LOW, true);

// state variables
int motionRaw = LOW;
int motion = LOW;
int doorClosedRaw = LOW;
int doorClosed = LOW;
int doorLockedRaw = HIGH;
int doorLocked = HIGH;
long motionTriggered = 0;
long doorUnlockTriggered = 0;
long doorCloseTriggered = 0;

void setup()
{
//  st::Everything::debug=true;
//  st::Executor::debug=true;
//  st::Device::debug=true;
//  st::PollingSensor::debug=true;
//  st::InterruptSensor::debug=true;
//  st::Everything::init();
//
//  st::Everything::addSensor(&sensor1);
//  st::Everything::addSensor(&sensor2);
//  st::Everything::addSensor(&sensor3);
  //st::Everything::addExecutor(&executor1);
  
  pinMode(PIN_BUZZER, OUTPUT);
  servo.attach(PIN_SERVO);
  servo.write(SERVO_NEUTRAL);
  
  // init ST_Anything
//  st::Everything::SmartThing.shieldSetLED(1, 0.5, 0); // orange
//  st::Everything::initDevices();

    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
}

void lock() {
  Serial.println("Locking...");
  
  // lock the door
  servo.write(SERVO_LOCK);
  tone(PIN_BUZZER, BUZZER_LOW);
  delay(BUZZER_DURATION);
  tone(PIN_BUZZER, BUZZER_HIGH, BUZZER_DURATION);
  delay(SERVO_DELAY - BUZZER_DURATION);
  
  // return servo to neutral position
  servo.write(SERVO_NEUTRAL);
  
  // this acts as a nice timeout before doing other actions
  delay(4 * SERVO_DELAY);
}

void unlock() {
  Serial.println("Unlocking...");
  
  // unlock the door
  servo.write(SERVO_UNLOCK);
  tone(PIN_BUZZER, BUZZER_HIGH);
  delay(BUZZER_DURATION);
  tone(PIN_BUZZER, BUZZER_LOW, BUZZER_DURATION);
  delay(SERVO_DELAY - BUZZER_DURATION);
  
  // return servo to neutral position
  servo.write(SERVO_NEUTRAL);
  
  // this acts as a nice timeout before doing other actions
  delay(4 * SERVO_DELAY);
}

void readSensors() {
  motionRaw = digitalRead(PIN_MOTION);
  doorClosedRaw = !digitalRead(PIN_REED_DOOR);
  doorLockedRaw = !digitalRead(PIN_REED_LOCK);
  
  // PIR timeout is too long - use rising edge of motionRaw to trigger motion,
  // but then motion back to LOW before motionRaw is at LOW
  if(motion == LOW && motionRaw == HIGH && motionTriggered == 0)
  {
    motion = HIGH;
    motionTriggered = millis();
  }
  if(motion == HIGH && millis() > (motionTriggered + MOTION_TIMEOUT))
  {
    motion = LOW;
  }
  if(motionRaw == LOW) {
    motionTriggered = 0;
  }
  
  // debounce lock state so that the lock does not fight us when we unlock the
  // door from outside when no motion is present (motion sensor is on the inside)
  if(doorLockedRaw == LOW)
  {
    // stagger door unlock propagation
    if(doorLocked == HIGH)
    {
      if(doorUnlockTriggered == 0)
      {
        doorUnlockTriggered = millis();
      }
      else if(millis() > (doorUnlockTriggered + UNLOCK_TIMEOUT))
      {
        doorLocked = LOW;
      }
    }
  }
  else
  {
    // immediately notify that door is locked
    doorLocked = HIGH;
    doorUnlockTriggered = 0;
  }
  
  // don't try to lock the door while the door is still closing and give
  // the person a chance to re-open the door after closing before locking
  if(doorClosedRaw == HIGH)
  {
    // stagger door close propagation
    if(doorClosed == LOW)
    {
      if(doorCloseTriggered == 0)
      {
        doorCloseTriggered = millis();
      }
      else if(millis() > (doorCloseTriggered + CLOSE_TIMEOUT))
      {
        doorClosed = HIGH;
      }
    }
  }
  else
  {
    // immediately notify that door is locked
    doorClosed = LOW;
    doorCloseTriggered = 0;
  }
  
  
  // debug
  Serial.print("motion: ");
  Serial.print(motion);
  Serial.print(", doorClosed: ");
  Serial.print(doorClosed);
  Serial.print(", doorLocked: ");
  Serial.print(doorLocked);
  Serial.println();
}

void loop()
{
//  st::Everything::run();
  
  // read in sensor values
  readSensors();
  
  // predefined automated responses
  if(motion) {
//    st::Everything::SmartThing.shieldSetLED(0, 1, 1); // cyan
    
    // unlock the door if motion is detected
    if(doorClosed && doorLocked) {
      unlock();
    }
  }
  else {
    
    // lock the door when no motion is detected
    if(doorClosed && !doorLocked) {
      lock();
    }
  }
}
