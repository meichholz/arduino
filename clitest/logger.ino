#include "logger.h"

 void Logger::debug(String  message) {
   Serial.print("DEBUG:");
   Serial.println(message);
 }
 
 void Logger::setup() {
 }

Logger::Logger() {
  this->setup();
}


