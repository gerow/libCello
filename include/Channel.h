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

void Channel_Push_Back(var self, var obj);
void Channel_Push_Front(var self, var obj);
void Channel_Push_At(var self, var obj, int i);

var Channel_Pop_Back(var self);
var Channel_Pop_Front(var self);
var Channel_Pop_At(var self, int i);

instance(Channel, New) = { sizeof(ChannelData), Channel_New, Channel_Delete }; 
instance(Channel, Push) = { Channel_Push_Back, Channel_Push_At, Channel_Push_Back, Channel_Push_Front, Channel_Pop_Back, Channel_Pop_At, Channel_Pop_Back, Channel_Pop_Front };

#endif