//  Created by Chance Snow on 5/16/22
//  Copyright © 2022 Snow Developments, LLC. All rights reserved.

// #include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "pd_api.h"
#include "wren.h"

// TODO: Evaluate https://github.com/ericlewis/swift-playdate
// TODO: Evaluate https://github.com/McDevon/taxman-engine

static int update(void* userdata);

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg) {
  if ( event == kEventInit ) {
    // Don't activate the Lua runtime
    pd->system->setUpdateCallback(update, pd);
    // Aim for 20 FPS
    pd->display->setRefreshRate(20);

    assert(initWrenRuntime(pd));
  }

  return 0;
}

static int update(void* userdata) {
  PlaydateAPI* pd = userdata;
  pd->graphics->clear(kColorWhite);

  // TODO: Draw game
  // See https://github.com/sayhiben/awesome-playdate

  pd->system->drawFPS(0,0);

  return 1;
}
