#include "ptest.h"
#include "Cello.h"

PT_SUITE(suite_go) {
  var empty_function (var args) {
    return None;
  }

  void sleepy_function(var args) {
    int i;
    for (i = 0; i < 100000000; i++) {
    }
    //print("args is %$\n", args);
    var channel = cast(at(args, 0), Channel);
    push(channel, None);
  }

  PT_TEST(test_go_function) {
    var f = new(Function, empty_function);
    go(f, None);
  }

  PT_TEST(test_channel_creation) {
    /* create a new channel with a buffer size 4 */
    try {
      var c = new(Channel, 4);
    } catch (e) {
      PT_ASSERT(0);
    }
  }

  PT_TEST(test_push_pop) {
    var c = new(Channel, 4);
    push(c, Some);
    PT_ASSERT(pop(c));
    push(c, Some);
    PT_ASSERT(pop(c));
    push(c, Some);
    PT_ASSERT(pop(c));
  }

  PT_TEST(test_blocking) {
    var f = new(Function, sleepy_function);
    var c = new(Channel, 4);
    go(f, c);
    pop(c);
    delete(f);
    delete(c);
  }
}