//******************************************************************************************
//  File: Everything.h
//  Authors: Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  st::Everything is a static class which essentially acts as the main() routine for
//			  a ST_Anything application.
//			  -All st::Device type objects (Sensors and Executors) are managed by st::Everything.
//			  -It calls the correct functions within each object it
//			   is responsible for at the proper time.
//			  -It handles all initialization of and use of the SmarThings Shield library.
//
//			  User-definable settings which will impact the st::Everything class are stored in
//			  Constants.h.  Please edit Constants.h to adjust these settings.
//
//			  In general, this file should not need to be modified.
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-01-03  Dan & Daniel   Original Creation
//	  2015-01-10  Dan Ogorchock	 Minor improvements to support Door Control Capability
//
//
//******************************************************************************************

#ifndef ST_EVERYTHING_H
#define ST_EVERYTHING_H

#include "Arduino.h"
#include "Constants.h"
#include "Sensor.h"
#include "Executor.h"

#include "SmartThings.h"

namespace st
{

	class Everything
	{
		private:
			static Sensor* m_Sensors[Constants::MAX_SENSOR_COUNT];		//array of Sensor objects that st::Everything will keep track of
			static unsigned int m_nSensorCount;	//number of st::Sensor objetcs added to st::Everything in your sketch Setup() routine

			static Executor* m_Executors[Constants::MAX_EXECUTOR_COUNT]; //array of Executor objects that st::Everything will keep track of
			static unsigned int m_nExecutorCount;//number of st::Executor objects added to st::Everything in your sketch Setup() routine

			static SmartThingsNetworkState_t stNetworkState;

			static void updateNetworkState();	//keeps track of the current ST Shield to Hub network status
			static void updateSensors();		//simply calls update on all the sensors
			static void sendStrings();			//sends all updates from the devices in Return_String

			static unsigned long lastmillis;	//used to keep track of last time run() has output freeRam() info

			//stuff for refreshing Devices
			static unsigned long refLastMillis;	//used to keep track of last time run() has called refreshDevices()
			static void refreshDevices();		//simply calls refresh on all the Devices

			#ifdef ENABLE_SERIAL
				static void readSerial();		//reads data from Arduino IDE Serial Monitor, if enabled in Constants.h
			#endif

			static String Return_String;		//static buffer for string data queued for transfer to SmartThings Shield - prevents dynamic memory allocation heap fragmentation

		public:
			//SmartThings Object
			#ifndef DISABLE_SMARTTHINGS
				static SmartThings SmartThing;	//SmartThings Shield Library object
			#endif

			static void init();					//st::Everything initialization routine called in your sketch setup() routine
			static void initDevices();			//calls the init() routine of every object added to st::Everything in your sketch setup() routine
			static void run();					//st::Everything initialization routine called in your sketch loop() routine

			static bool sendSmartString(String &str); //sendSmartString() may edit the string reference passed to it - queues messages - preferable
			static bool sendSmartStringNow(String &str); //sendSmartStringNow() may edit the string reference passed to it - sends messages immediate - only for special curcumstances

			static Device* getDeviceByName(const String &str);	//returns pointer to Device object by name

			static bool addSensor(Sensor *sensor);		//adds a Sensor object to st::Everything's m_Sensors[] array - called in your sketch setup() routine
			static bool addExecutor(Executor *executor);//adds a Executor object to st::Everything's m_Executors[] array - called in your sketch setup() routine

			static byte bTimersPending;	//number of time critical events in progress - if > 0, do NOT perform refreshDevices() routine

			static bool debug;	//debug flag to determine if debug print statements are executed - set value in your sketch's setup() routine

			friend SmartThingsCallout_t receiveSmartString; //callback function to act on data received from SmartThings Shield - called from SmartThings Shield Library
	};
}
#endif
