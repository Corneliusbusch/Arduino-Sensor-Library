#include "Arduino.h"
#include "Sensor.h"

Sensor::Sensor(int attachedTo, int prev, int thres) {
  pin = attachedTo;
  prevStates = (int*) realloc(prevStates, prev * sizeof(int));
  threshold = thres;
}

void Sensor::setBaseVal(int newVal){
  baseVal = newVal;
}

bool Sensor::isTriggeredValue(int newVal){

  //Serial.println("start triggered");
  // enqueue new value
  enqueuePrevStates(newVal);


  // check how many were triggered from prevStates
  int counter;
  int len = sizeof(prevStates) / sizeof(prevStates[0]);
  if(noReadings < len){
    counter = noReadings;
  }else{
    counter = len;
  }

  int trigg = 0;
  for(int i = 0; i < counter; i++){
    int diff = abs(baseVal - prevStates[i]);
    if(diff > threshold){
      trigg++;
    }
  }


  // if number triggered larger than not triggered, then accepted
  if(trigg > counter/2){
    return(true);
  } else{
    return(false);
  }

}

  void Sensor::enqueuePrevStates(int value){

     int len = sizeof(prevStates) / sizeof(prevStates[0]);
     //Serial.println(len);
     int last;
     if(noReadings >= len){
        int discard = dequeuePrevStates();
        last = len-1;
     } else{
        last = noReadings;
     }
     prevStates[last] = value;
     noReadings++;
  }

int Sensor::dequeuePrevStates(){
   int len = sizeof(prevStates) / sizeof(prevStates[0]);
   int first = prevStates[0];
   for(int i = 1; i < len; i++){
    prevStates[i-1] = prevStates[i];
   }
   prevStates[len-1] = 0;
}

void Sensor::printSensor(){
  Serial.print("pin: " + String(pin) + " base: " + String(baseVal) + " readings: " + String(noReadings));
}