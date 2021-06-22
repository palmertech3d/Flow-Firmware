#include "queue.h"
#include <Arduino.h>

queue::queue(){}

bool queue::put(gcodeCommand item){
  if (full()){
    return 0;
  }else{
    if (head == max_size){
      head = 0;
    }
    commands[head] = item;
    head++;
    return 1;
  }
}

void queue::putForce(gcodeCommand item){

  if (head == max_size){
    head = 0;
  }
  commands[head] = item;
  head++;
}

gcodeCommand queue::get(){
  gcodeCommand temp;

  if(!empty()){
    if (tail == max_size){
      tail = 0;
    }
    temp = commands[tail];
    tail++;
  }else{
    //Serial.println("Thinks empty");
    temp.letter = -1;
  }

  return temp;
}

void queue::reset(){
  tail = 0;
  head = 0;
  full_flag = 0;
}

bool queue::empty() const{
  return (tail == head && !full_flag);
}

bool queue::full() const{
  return full_flag;
}

uint8_t queue::capacity() const{
  return max_size;
}

uint8_t queue::size() const{
  return head - tail;
}
