#ifndef QUEUEH
#define QUEUEH

#include "parser.h"

class queue{
public:
  // Default constructor
  // Initializes with default max_size of 10
  queue();

  // Alternative constructor
  // Initializes with custom maxiumum size
  queue(int max);

  // Puts a command in the queue
  void put(gcodeCommand item);

  // Gets the next command in the queue
  gcodeCommand get();

  // Resets the queue to the empty state
  void reset();

  // Returns true if queue is empty; false if not
  bool empty() const;

  // Returns true if queue is full; false if not
  bool full() const;

  // Returns the max capacity of the queue
  int capacity() const;

  // Returns the current number of elements in the queue
  int size() const;


private:
  gcodeCommand* head = 0;
  gcodeCommand* tail = 0;
  const int max_size;
  bool full_flag = 0;

};

#endif // QUEUEH
