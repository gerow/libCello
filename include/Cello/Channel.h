/*
** == Channel ==
** A thread-safe Communication Channel
**
** + can either be buffered or unbuffered
** + buffered version implemented as a ring buffer
** + unbuffered version requires two threads to meet up
**
*/

#ifndef Channel_h
#define Channel_h

#include "Prelude.h"
#include "Type.h"

#include <pthread.h>

global var Channel;

data {
  var type;
  int buffer_size;
  int read_pos;
  int write_pos;
  var *buffer;
  pthread_mutex_t mutex;
  pthread_cond_t read_ready_cond;
  pthread_cond_t write_ready_cond;
} ChannelData;

var Channel_New(var self, va_list* args);
var Channel_Delete(var self);

void Channel_Push(var self, var obj);
var Channel_Pop(var self);

instance(Channel, New) = { sizeof(ChannelData), Channel_New, Channel_Delete }; 
instance(Channel, Push) = { Channel_Push, NULL, NULL, NULL, Channel_Pop, NULL, NULL, NULL };
#endif
