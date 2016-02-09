#pragma once
#include <stdlib.h>
class Helper{
public:
static unsigned long getTime(){
  timeval time;
gettimeofday(&time, NULL);
unsigned long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
return millis;
}
};