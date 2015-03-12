//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Door Lock - Josh Villbrandt, March 2015
//*****************************************************************************

// includes
#include <SoftwareSerial.h>
#include <SmartThings.h>

// pins
#define PIN_THING_TX    2
#define PIN_THING_RX    3
#define PIN_REED_DOOR   7
#define PIN_REED_LOCK   8
#define PIN_SERVO       9
#define PIN_BUZZER      10
#define PIN_MOTION      11

// init SmartThings
SmartThingsCallout_t messageCallout;
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor

// start variables
bool isDebugEnabled = true;
int motion = LOW;
int doorOpen = HIGH;
int doorLocked = HIGH;

void setup()
{
  // setup hardware pins
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // setup debug serial port
  if (isDebugEnabled)
  {
    Serial.begin(9600);
    Serial.println("Door Lock - setup complete");
  }
}

void loop()
{
  // read in sensor values
  readSensors();

  // perform automated responses
  automatedResponses();

  // run smartthing logic
  smartthing.run();
}

void readSensors()
{
  motion = digitalRead(PIN_MOTION);
  doorOpen = digitalRead(PIN_REED_DOOR);
  doorLocked = digitalRead(PIN_REED_LOCK);
}

void lock()
{
  // todo
}

void unlock()
{
  // todo
}

void automatedResponses()
{
  // turn on RGB LED if motion is detected

}

void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  // perform actions
  if (message.equals("lock")) lock();
  else if (message.equals("unlock")) unlock();
}
