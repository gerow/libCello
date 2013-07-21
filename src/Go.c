#include "Cello/Go.h"

#include "Cello/Function.h"
#include "Cello/Number.h"

#include <pthread.h>
#include <assert.h>

void * go_runner(var thread_obj) {
  print("inside go runner\n");

  var f = at(thread_obj, 0);
  var args = at(thread_obj, 1);

  print("succeeded in getting stuff from thread_obj\n");

  Call *icall = type_class(type_of(f), Call);
  assert(icall->call_with);

  print("grabbing call interface seemed to work\n");

  icall->call_with(f, args);

  print("call seemed to work\n");

  delete(thread_obj);
  delete(args);

  return NULL;
}

void go_with_ptr(var self, var* args) {
  int num = 0;
  while(args[num] != (var)-1) { num++; }
  //print("found %i args\n", num);
  return go_with(self, new(List, 3, num, args, 0));
}

void go_with(var f, var args){
  print("got inside go_with\n");
  //print("args is %$\n", args);
  Call* icall = type_class(type_of(f), Call);
  assert(icall->call_with);
  print("icall classification worked\n");

  pthread_t thread;
  pthread_attr_t attr;

  print("creating thred_obj\n");
  var thread_obj = new(List, 2, f, args);
  //var thread_obj = NULL;
  print("created thread obj\n");

  // create the thread as detached so it will clean up its
  // resources on its own when it exits
  pthread_attr_init(&attr);
  print("Initialized pthread_attr\n");
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  print("Set pthread to detached state\n");
  int rc = pthread_create(&thread, &attr, go_runner, thread_obj);
  print("Called pthread create\n");
  if (rc) {
    // maybe throw an error here...
  }
}