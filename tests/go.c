#include "ptest.h"
#include "Cello.h"

PT_SUITE(suite_go) {
  var empty_function (var args) {
    return None;
  }

  PT_TEST(test_go_function) {
    var f = new(Function, empty_function);
    go(f, None);
  }
}