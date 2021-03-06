#include "Cello/Channel.h"

var Channel = methods {
  methods_begin(Channel),
  method(Channel, New),
  method(Channel, Push),
  methods_end(Channel)
};

var Channel_New(var self, va_list* args) {
  ChannelData* co = cast(self, Channel);

  co->buffer_size = va_arg(*args, int);
  co->read_pos = 0;
  co->write_pos = 0;
  co->buffer = NULL;
  pthread_mutex_init(&co->mutex, NULL);
  pthread_cond_init(&co->read_ready_cond, NULL);
  pthread_cond_init(&co->write_ready_cond, NULL);

  if (co->buffer_size) {
    co->buffer = malloc(co->buffer_size * sizeof(*co->buffer));
  }

  return self;
}

var Channel_Delete(var self) {
  ChannelData* co = cast(self, Channel);
  pthread_mutex_destroy(&co->mutex);
  pthread_cond_destroy(&co->read_ready_cond);
  pthread_cond_destroy(&co->write_ready_cond);
  if (co->buffer) {
    free(co->buffer);
  }
  return self;
}

void Channel_Push(var self, var val) {
  print("pushing into channel\n");
  ChannelData* co = cast(self, Channel);

  pthread_mutex_lock(&co->mutex);
  while (co->write_pos == -1) {
    pthread_cond_wait(&co->write_ready_cond, &co->mutex);
    pthread_mutex_lock(&co->mutex);
  }

  bool empty_before = (co->read_pos == co->write_pos);

  co->buffer[co->write_pos] = val;
  co->write_pos = (co->write_pos + 1) % co->buffer_size;
  if (co->write_pos == co->read_pos) {
    /* to signal the fact that the buffer is empty */
    co->write_pos = -1;
  }

  if (empty_before) {
    pthread_cond_signal(&co->read_ready_cond);
  }

  pthread_mutex_unlock(&co->mutex);
}

var Channel_Pop(var self) {
  ChannelData* co = cast(self, Channel);

  pthread_mutex_lock(&co->mutex);
  while (co->read_pos == co->write_pos) {
    pthread_cond_wait(&co->read_ready_cond, &co->mutex);
    pthread_mutex_lock(&co->mutex);
  }

  bool full_before = (co->write_pos == -1);

  var out = co->buffer[co->read_pos];
  co->read_pos = (co->read_pos + 1) % co->buffer_size;

  if (full_before) {
    pthread_cond_signal(&co->write_ready_cond);
  }

  pthread_mutex_unlock(&co->mutex);

  return out;
}
