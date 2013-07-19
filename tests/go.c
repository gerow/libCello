#include "ptest.h"
#include "Cello.h"

PT_SUITE(suite_go) {
  var empty_function (var args) {
    return None;
  }

  void sleepy_function(var channel) {
    while(1) {
      
    }
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

  PT_TEST(test_blocking) {
    try {
      var c = new(Channel, 4);
      var f = new(Function, sleepy_function);
      var args = new(List, 1, c);
      go(f, args);
      pop(c);
    } catch (e) {
      PT_ASSERT(0);
    }
  }
}