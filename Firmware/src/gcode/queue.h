#ifndef QUEUEH
#define QUEUEH

#include "parser.h"

class queue{
public:
  // Default constructor
  // Initializes with default max_size of 10
  queue();

  // Puts a command in the queue
  // Returns true if queue was not empty and item put,
  // false if queue was full and item could not be put
  bool put(gcodeCommand item);

  // Puts a command in the queue
  // If queue is full, overwrites previous data
  void putForce(gcodeCommand item);

  // Gets the next command in the queue
  gcodeCommand get();

  // Resets the queue to the empty state
  void reset();

  // Returns true if queue is empty; false if not
  bool empty() const;

  // Returns true if queue is full; false if not
  bool full() const;

  // Returns the max capacity of the queue
  uint8_t capacity() const;

  // Returns the current number of elements in the queue
  uint8_t size() const;


private:
  uint8_t head = 0;
  uint8_t tail = 0;
  static const int max_size = 10;
  gcodeCommand commands[max_size];
  bool full_flag = 0;

};

#endif // QUEUEH
