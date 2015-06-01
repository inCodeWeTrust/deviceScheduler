/// 
/// @mainpage	deviceScheduler 
///
/// @details	schedule and drive stepper-, servo- and other devices
/// @n 		
/// @n 
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		little Ikarus
/// @author		___FULLUSERNAME___
/// @date		01.06.15 12:52
/// @version	<#version#>
/// 
/// @copyright	(c) little Ikarus, 2015
/// @copyright	License
///
/// @see		ReadMe.txt for references
///


///
/// @file		deviceScheduler.ino
/// @brief		Main sketch
///
/// @details	<#details#>
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		little Ikarus
/// @author		___FULLUSERNAME___
/// @date		01.06.15 12:52
/// @version	<#version#>
/// 
/// @copyright	(c) little Ikarus, 2015
/// @copyright	License
///
/// @see		ReadMe.txt for references
/// @n
///


// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430 G2 and F5529, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries


// Prototypes


// Define variables and constants


// Add setup code 
void setup() 
{

}

// Add loop code 
void loop() 
{

}
