//
// main.cpp
// Main file
// ----------------------------------
// Developed with embedXcode+
// http://embedXcode.weebly.com
//
// Project 		deviceScheduler
//
// Created by 	little Ikarus, 01.06.15 12:52
//              ___FULLUSERNAME___
//
// Copyright 	© little Ikarus, 2015
// License 		License
//
// See 			deviceScheduler.ino and ReadMe.txt for references
//
// ----------------------------------
// DO NOT EDIT THIS FILE.
// THE SKETCH IS IN deviceScheduler.ino
// ----------------------------------
//
// Last update: Aug 19, 2014 release 174

// IDE selection
#if defined(EMBEDXCODE)

// Core library and main()
#if defined(MPIDE)
#elif defined(MICRODUINO)
#elif defined(ENERGIA)
#elif defined(LIGHTBLUE_CORE)
#elif defined(MAPLE_IDE)
#elif defined(TEENSYDUINO) || defined(CORE_TEENSY)
#elif defined(WIRING)
#elif defined(COSA)

#elif defined(ARDUINO)
// ============================================================================= Arduino specific

#if (ARDUINO < 100)
// ----------------------------------------------------------------------------- Arduino 0023 specific

#include "WProgram.h"

int main(void)
{
	init();
    
	setup();
    
	for (;;)
		loop();
    
	return 0;
}


#elif (ARDUINO < 150)
// ----------------------------------------------------------------------------- Arduino 1.0x specific

#include "Arduino.h"

int main(void)
{
	init();
    
#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
    
	return 0;
}


#else
// ----------------------------------------------------------------------------- Arduino 1.5x specific

#if defined(__ARDUINO_X86__)
// ............................................................................. Arduino 1.5x X86 architecture specific

/*
 main.cpp userspace main loop for Intel Galileo family boards
 Copyright (C) 2014 Intel Corporation
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 
 */
// Arduino hooks
#include <Arduino.h>
#include <trace.h>
#include <interrupt.h>
#include <sys/stat.h>

#define PLATFORM_NAME_PATH "/sys/devices/platform/"

/************************ Static *************************/
#define MY_TRACE_PREFIX __FILE__

/************************ Global *************************/
int main(int argc, char * argv[])
{
	char *platform_path = NULL;
	struct stat s;
	int err;
    
	// Install a signal handler
    
	// make ttyprintk at some point
	stdout = freopen("/tmp/log.txt", "w", stdout);
	if (stdout == NULL){
	    fprintf(stderr, "unable to remap stdout !\n");
	    exit(-1);
	}
	fflush(stdout);
    
	stderr = freopen("/tmp/log_er.txt", "w", stderr);
	if (stderr == NULL){
	    printf("Unable to remap stderr !\n");
	    exit(-1);
	}
	fflush(stderr);
    
	// Snapshot time counter
	if (timeInit() < 0)
		exit(-1);
    
	// debug for the user
	if (argc < 2){
		fprintf(stderr, "./sketch tty0\n");
		return -1;
	}
	printf("started with binary=%s Serial=%s\n", argv[0], argv[1]);
	fflush(stdout);
    
	// check if we're running on the correct platform
	// and refuse to run if no match
    
	platform_path = (char *)malloc(sizeof(PLATFORM_NAME_PATH) + sizeof(PLATFORM_NAME));
	sprintf(platform_path,"%s%s", PLATFORM_NAME_PATH, PLATFORM_NAME);
    
	printf("checking platform_path [%s]\n", platform_path);
	fflush(stdout);
    
	err = stat(platform_path, &s);
    
	if(err != 0) {
		fprintf(stderr, "stat failed checking for %s with error code %d\n", PLATFORM_NAME, err);
		free(platform_path);
		return -1;
	}
	if(!S_ISDIR(s.st_mode)) {
		/* exists but is no dir */
		fprintf(stderr, "Target board not a %s\n", PLATFORM_NAME);
		free(platform_path);
		return -1;
	}
    
	printf("Running on a %s platform (%s)\n", PLATFORM_NAME, platform_path);
	fflush(stdout);
    
	free(platform_path);
    
	// TODO: derive trace level and optional IP from command line
	trace_init(VARIANT_TRACE_LEVEL, 0);
	trace_target_enable(TRACE_TARGET_UART);
    
	// Call Arduino init
	init(argc, argv);
    
	// Init IRQ layer
	// Called after init() to ensure I/O permissions inherited by pthread
	interrupt_init();
    
#if defined(USBCON)
	USBDevice.attach();
#endif
    
	setup();
	for (;;) {
		loop();
		//if (serialEventRun) serialEventRun();
	}
	return 0;
}


#elif defined(__SAM3X8E__)
// ............................................................................. Arduino 1.5x SAM architecture specific

/*
 Copyright (c) 2011 Arduino.  All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#define ARDUINO_MAIN
#include "Arduino.h"

/*
 * Cortex-M3 Systick IT handler
 */
/*
 extern void SysTick_Handler( void )
 {
 // Increment tick count each ms
 TimeTick_Increment() ;
 }
 */

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
	init();
    
	delay(1);
    
#if defined(USBCON)
	USBDevice.attach();
#endif
    
	setup();
    
	for (;;)
	{
		loop();
		if (serialEventRun) serialEventRun();
	}
    
	return 0;
}

#else
// ............................................................................. Arduino 1.5x AVR architecture specific

#include "Arduino.h"

int main(void)
{
	init();
    
#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
    
	return 0;
}


#endif                                                                          // architecture
#endif                                                                          // Arduino

#else                                                                           // error
#error Platform not defined
#endif                                                                          // end IDE

// Sketch
#include "voiceOGraph.ino"


#endif                                                                          // end embedXcode

