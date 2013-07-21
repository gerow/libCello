/*
** == Go ==
** 
** Go Style Routines
**
*/

#include "Prelude.h"
#include "Type.h"
#include "List.h"

#ifndef ChelloGo_h
#define ChelloGo_h

#define go(x, ...) go_with_ptr(x, (var[]){ __VA_ARGS__, (var)-1 })
void go_with_ptr(var f, var*);
void go_with(var f, var args);

#endif