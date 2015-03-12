# SmartThings Lock

An Internet of Things door lock using an Arduino and the SmartThings platform.

## Summary

This projects creates a network-connected door lock. The device uses two reed switches to report the open/closed status of the door and the locked/unlocked status of the lock. The door's lock may be engaged and disengaged via a servo. The locking mechanism works without inhibiting the door to be locked or unlocked via standard methods (using a key or by turning the thumb-turn.)

An Arduino Uno with a SmartThings ThingShield connects to the SmartThings hub with the help of the [ST_Anything](https://github.com/DanielOgorchock/ST_Anything) Arduino library.

## Bill of Materials

| Qty. | Item | Link |
| --- | --- | --- |
| 1 | Arduino Uno | |
| 1 | SmartThings ThingShield | |

## Electrical Schematic

![screenshot](fritzing/SmartThings Lock Fritzing.jpg)

## Digital Model

![screenshot](solidworks/SmartThings Lock Solidworks.jpg)
