#include "Go.h"

#include "Function.h"

#include <pthread.h>

void go_runner(var thread_obj) {
  var f = at(0, thread_obj);
  var args = at(1, thread_obj);

  call(f, args);

  delete(thread_obj);
}

void go(var f, var args){
  pthread_t thread;
  pthread_attr_t attr;

  var thread_obj = new(List, 2, f, args);

  // create the thread as detached so it will clean up its
  // resources on its own when it exits
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  int rc = pthread_create(&thread, &attr, go_runner, thread_obj);
  if (rc) {
    // maybe throw an error here...
  }
}