#include "queue.h"

queue::queue(){}

bool queue::put(gcodeCommand item){
  if (full()){
    return 0;
  }else{
    commands[head] = item;
    head++;
    return 1;
  }
}

void queue::putForce(gcodeCommand item){
  commands[head] = item;
  head++;
}

gcodeCommand queue::get(){
  gcodeCommand temp;

  if(!empty()){
    temp = commands[tail];
    tail++;
  }else{
    temp.letter = -1;
  }

  return temp;
}

void queue::reset(){
  tail = head;
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
