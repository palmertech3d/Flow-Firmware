#include "queue.h"
#include <Arduino.h>

queue::queue(){
}

bool queue::put(GcodeCommand_t item){
  if (full()) {
    return 0;
  } else {
    if (head == max_size) {
      head = 0;
    }
    commands[head] = item;
    head++;
    return 1;
  }
} // queue::put

void queue::putForce(GcodeCommand_t item){
  if (head == max_size) {
    head = 0;
  }
  commands[head] = item;
  head++;
} // queue::putForce

GcodeCommand_t queue::get(){
  GcodeCommand_t temp;

  if (!empty()) {
    if (tail == max_size) {
      tail = 0;
    }
    temp = commands[tail];
    tail++;
  } else {
    temp.letter = GCODE_LETTER_ERR;
  }

  return temp;
} // queue::get

void queue::reset(){
  tail = 0;
  head = 0;
  full_flag = 0;
} // queue::reset

bool queue::empty() const {
  return (tail == head && !full_flag);
} // queue::empty

bool queue::full() const {
  return full_flag;
} // queue::full

uint8_t queue::capacity() const {
  return max_size;
} // queue::capacity

uint8_t queue::size() const {
  return head - tail;
} // queue::size
