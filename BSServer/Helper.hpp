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
static unsigned long getTimeNano(){
  timeval time;
gettimeofday(&time, NULL);
unsigned long nano= (time.tv_sec * 10000000000) + (time.tv_usec );
return nano;
}
};