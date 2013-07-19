#include "Channel.h"

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

void Channel_Push_Back(var self, var val) {
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
}

void Channel_Push_Front(var self, var val) {
}

void Channel_Push_At(var self, var val, int index) {
}

var Channel_Pop_Back(var self) {
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

  return out;
}

var Channel_Pop_Front(var self) {
  return NULL;
}

var Channel_Pop_At(var self, int index) {
  return NULL;
}
