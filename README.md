# SmartThings Lock

An Internet of Things door lock using an Arduino and the SmartThings platform.

## Summary

This projects creates a network-connected door lock. The device uses two reed switches to report the open/closed status of the door and the locked/unlocked status of the lock. The door's lock may be engaged and disengaged via a servo. The locking mechanism works without inhibiting the door to be locked or unlocked via standard methods (using a key or by turning the thumb-turn.)

An Arduino Uno with a SmartThings ThingShield connects to the SmartThings hub with the help of the [ST_Anything](https://github.com/DanielOgorchock/ST_Anything) Arduino library.

## Bill of Materials

| Qty. | Item | Link |
| --- | --- | --- |
| 1 | Arduino Uno | http://www.amazon.com/IEIK-Board-ATmega328P-Cable-Arduino/dp/B00P2FX9WY/ref=sr_1_2?ie=UTF8&qid=1426149161&sr=8-2&keywords=arduino+uno |
| 1 | Arduino DC Power Adapter | http://www.amazon.com/Power-Adapter-Arduino-2-Flat-Pin-100CM/dp/B00CP1QLSC/ref=sr_1_1?ie=UTF8&qid=1426149355&sr=8-1&keywords=arduino+power+adapter |
| 1 | SmartThings ThingShield | https://shop.smartthings.com/#!/products/smartthings-shield-arduino |
| 1 | Adafruit PIR Motion Sensor | http://www.amazon.com/gp/product/B00JOZTAC6/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1 |
| 2 | Magnetic Reed Switches | http://www.amazon.com/gp/product/B00HR8CT8E/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1 |
| 1 | Standard Servo | http://www.amazon.com/Hitec-31311S-HS-311-Standard-Universal/dp/B0006O3WVE/ref=sr_1_2?ie=UTF8&qid=1426149512&sr=8-2&keywords=standard+servo |
| 1 | Piezo-electric buzzer | http://www.amazon.com/Terminals-Electronic-Continuous-Sound-Buzzer/dp/B00B0Q4KKO/ref=sr_1_8?ie=UTF8&qid=1426149482&sr=8-8&keywords=piezo+buzzer |
| 1 | 3D Printed SmartThings Lock Set | n/a |
| 1 | Wire Close Hanger | n/a |

## Electrical Schematic

![screenshot](fritzing/SmartThings%20Lock%20Fritzing.jpg)

## Digital Model

![screenshot](solidworks/SmartThings%20Lock%20Solidworks.jpg)
