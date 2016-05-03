///
/// @file		freeRam.h
/// @brief		Header
/// @details	<#details#>
/// @n	
/// @n @b		Project deviceScheduler
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Little Abakus
/// @author		Little Abakus
///
/// @date		02.05.16 14:10
/// @version	<#version#>
/// 
/// @copyright	(c) Little Abakus, 2016
/// @copyright	<#license#>
///
/// @see		ReadMe.txt for references
///

#include <malloc.h>


#ifndef freeRam_h
#define freeRam_h

// Prototypes
void freeRam ();


extern char _end;
extern "C" char *sbrk(int i);



#endif
