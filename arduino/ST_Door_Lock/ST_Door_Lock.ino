//******************************************************************************************
//  File: ST_Anything_Doors.ino
//  Authors: Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  This Arduino Sketch, along with the ST_Anything library and the revised SmartThings
//            library, demonstrates the ability of one Arduino + SmartThings Shield to
//            implement a multi input/output custom device for integration into SmartThings.
//            The ST_Anything library takes care of all of the work to schedule device updates
//            as well as all communications with the SmartThings Shield.
//
//            ST_Anything_Doors implements the following:
//              - 2 x Door Control devices (used as Garage Doors)
//              - 4 x Contact Sensor devices (used to monitor magnetic door sensors)
//              - 1 x Motion device (used to detect motion in the garage)
//              - 1 x Temperature/Humidity device (unsed to monitor temp & humidity in the garage)
//
//            During the development of this re-usable library, it became apparent that the
//            Arduino UNO R3's very limited 2K of SRAM was very limiting in the number of
//            devices that could be implemented simultaneously.  A tremendous amount of effort
//            has gone into reducing the SRAM usage, including siginificant improvements to
//            the SmartThings Arduino library.  The SmartThings library was also modified to
//            include support for using Hardware Serial port(s) on the UNO, MEGA, and Leonardo.
//            During testing, it was determined that the Hardware Serial ports provide much
//            better performance and reliability versus the SoftwareSerial library.  Also, the
//            MEGA 2560's 8K of SRAM is well worth the few extra dollars to save your sanity
//            versus always running out of SRAM on the UNO R3.  The MEGA 2560 also has 4 Hardware
//            serial ports (i.e. UARTS) which makes it very easy to use Hardware Serial instead
//            of SoftwareSerial, while still being able to see debug data on the USB serial
//            console port (pins 0 & 1).
//
//            Note: We did not have a Leonardo for testing, but did fully test on UNO R3 and
//            MEGA 2560 using both SoftwareSerial and Hardware Serial communications to the
//            Thing Shield.
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-01-03  Dan & Daniel   Original Creation
//    2015-01-07  Dan Ogorchock  Modified for Door Monitoring and Garage Door Control
//
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
#define PIN_REED_DOOR                7
#define PIN_REED_LOCK                8
#define PIN_SERVO                    9
#define PIN_BUZZER                   10
#define PIN_MOTION                   11

// sensors
st::IS_Contact sensor1("door", PIN_REED_DOOR, LOW, true);
st::IS_Contact sensor2("lock", PIN_REED_LOCK, LOW, true);
st::IS_Motion sensor3("motion", PIN_MOTION, HIGH, false);
//st::EX_Switch executor1("sampleEX", PIN_sampleEX, LOW, true);

void setup()
{
  st::Everything::debug=true;
  st::Executor::debug=true;
  st::Device::debug=true;
  st::PollingSensor::debug=true;
  st::InterruptSensor::debug=true;
  st::Everything::init();

  st::Everything::addSensor(&sensor1);
  st::Everything::addSensor(&sensor2);
  st::Everything::addSensor(&sensor3);
  //st::Everything::addExecutor(&executor1);
  st::Everything::initDevices();
}

void loop()
{
  st::Everything::run();
  
  // temporarily read in the motion sensor here too
  int motion = digitalRead(PIN_MOTION);
  if(motion) {
    st::Everything::SmartThing.shieldSetLED(0, 1, 1); // cyan
  }
  else {
    st::Everything::SmartThing.shieldSetLED(0, 0, 0); // off
  }
}
