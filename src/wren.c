//  Created by Chance Snow on 5/16/22
//  Copyright © 2022 Snow Developments, LLC. All rights reserved.
//
// See Wren (https://wren.io)

#include <stdlib.h>
#include <assert.h>

#include "wren.h"

WrenVM* __wrenVM;

// Wren Runtime Port
double wrenGetLocalTimeZoneAdjustment(/** ms since unix epoch */ double unix_ms, bool is_utc) {
  // TODO: https://devforum.play.date/t/c-equivalent-to-playdate-gettime/7884/3
  return 0;
}
double wrenGetCurrentTime(void) {
  PlaydateAPI* pd = wrenGetUserData(__wrenVM);
  assert(pd != NULL);
  unsigned int ms;
  unsigned int sec = pd->system->getSecondsSinceEpoch(&ms);
  double time = sec * 1000.0f + ms;
  return time;
}

void writeFn(WrenVM* vm, const char* text) {
  PlaydateAPI* pd = wrenGetUserData(vm);
  assert(pd != NULL);
  pd->system->logToConsole(text);
}

// TODO: Write unit tests: https://jera.com/techinfo/jtns/jtn002

bool initWrenRuntime(PlaydateAPI* pd) {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.userData = pd;
  config.writeFn = &writeFn;
  __wrenVM = wrenNewVM(&config);
  if (__wrenVM == NULL) pd->system->error("Could not initialize Wren runtime!");

  return __wrenVM != NULL;
}

void destroyWrenRuntime() {
  wrenFreeVM(__wrenVM);
}
