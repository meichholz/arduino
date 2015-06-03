#ifndef _H_LOGGER
#define _H_LOGGER

#include "Arduino.h"

 class Logger {
   public:
   void setup();
   void debug(String message);
   Logger();
   virtual ~Logger() {}; 
 };
 #endif
 
