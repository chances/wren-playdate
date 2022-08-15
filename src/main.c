//  Created by Chance Snow on 8/14/22
//  Copyright © 2022 Snow Developments, LLC. All rights reserved.

// #include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "pd_api.h"
#include "wren.h"

Script* entryPoint = NULL;

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

    // Load Wren entry point
    entryPoint = malloc(sizeof(Script));
    *entryPoint = loadScript("main", "main.wren");

    // TODO: Call `eventHandler` in main Wren module
  }

  return 0;
}

static int update(void* userdata) {
  PlaydateAPI* pd = userdata;

  // TODO: Call `update` in main Wren module and refactor out these draw calls

  pd->graphics->clear(kColorWhite);
  pd->system->drawFPS(0,0);

  return 1;
}
