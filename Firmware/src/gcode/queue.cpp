#include "queue.h"
#include <Arduino.h>

GcodeQueue::GcodeQueue(){
}

bool GcodeQueue::put(GcodeCommand_t item){
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
} // GcodeQueue::put

void GcodeQueue::putForce(GcodeCommand_t item){
  if (head == max_size) {
    head = 0;
  }
  commands[head] = item;
  head++;
} // GcodeQueue::putForce

GcodeCommand_t GcodeQueue::get(){
  GcodeCommand_t temp_cmd;

  if (!empty()) {
    if (tail == max_size) {
      tail = 0;
    }
    temp_cmd = commands[tail];
    tail++;
  } else {
    temp_cmd.letter = GCODE_LETTER_ERR;
  }

  return temp_cmd;
} // GcodeQueue::get

void GcodeQueue::reset(){
  tail = 0;
  head = 0;
  full_flag = 0;
} // GcodeQueue::reset

bool GcodeQueue::empty() const {
  return (tail == head && !full_flag);
} // GcodeQueue::empty

bool GcodeQueue::full() const {
  return full_flag;
} // GcodeQueue::full

uint8_t GcodeQueue::capacity() const {
  return max_size;
} // GcodeQueue::capacity

uint8_t GcodeQueue::size() const {
  return head - tail;
} // GcodeQueue::size
