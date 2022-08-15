//  Created by Chance Snow on 8/14/22
//  Copyright © 2022 Snow Developments, LLC. All rights reserved.
//
// See Wren (https://wren.io)

#include <stdlib.h>
#include <assert.h>

#include "wren.h"

WrenVM* __wrenVM;
ScriptResult interpret(Script script);

#define wrenGetPlaydateAPI(vm) (assert(vm != NULL), wrenGetUserData(vm))

// Wren Runtime Port
double wrenGetLocalTimeZoneAdjustment(/** ms since unix epoch */ double unix_ms, bool is_utc) {
  // TODO: https://devforum.play.date/t/c-equivalent-to-playdate-gettime/7884/3
  return 0;
}
double wrenGetCurrentTime(void) {
  PlaydateAPI* pd = wrenGetPlaydateAPI(__wrenVM);
  unsigned int ms;
  unsigned int sec = pd->system->getSecondsSinceEpoch(&ms);
  double time = sec * 1000.0f + ms;
  return time;
}

void writeFn(WrenVM* vm, const char* text) {
  PlaydateAPI* pd = wrenGetPlaydateAPI(__wrenVM);
  pd->system->logToConsole(text);
}
void errorFn(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message) {
  PlaydateAPI* pd = wrenGetPlaydateAPI(vm);
  pd->system->logToConsole("Error executing module `%s`:\n\t%s:%d %s", module, module, line, message);
}

// TODO: Write unit tests: https://jera.com/techinfo/jtns/jtn002

bool initWrenRuntime(PlaydateAPI* pd) {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.userData = pd;
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;
  __wrenVM = wrenNewVM(&config);
  if (__wrenVM == NULL) pd->system->error("Could not initialize Wren runtime!");

  return __wrenVM != NULL;
}

void destroyWrenRuntime() {
  wrenFreeVM(__wrenVM);
}

Script loadScript(char* module, char* file) {
  PlaydateAPI* pd = wrenGetPlaydateAPI(__wrenVM);
  pd->system->logToConsole("Loading Wren module `%s` from file %s", module, file);

  FileStat entryPointStats;
  if(pd->file->stat(file, &entryPointStats) != 0)
    pd->system->error("Could not stat module `%s` from file %s:\n\t%s", module, file, pd->file->geterr());
  Script script = { entryPointStats.size + 1, toString(module), toString(file), NULL };
  if (script.size == 0) return script;
  script.contents = malloc(script.size);
  SDFile* entryPointFile = pd->file->open(file, kFileRead);
  if (pd->file->read(entryPointFile, script.contents, entryPointStats.size) != entryPointStats.size)
    pd->system->error("Could not read module `%s` from file %s:\n\t%s", module, file, pd->file->geterr());
  if(pd->file->close(entryPointFile) != 0)
    pd->system->error("Could not close file %s:\n\t%s", file, pd->file->geterr());
  pd->system->logToConsole("Loaded Wren module `%s` from file %s", module, file);

  ScriptResult result = interpret(script);
  if (!result.success)
    pd->system->error("Could not initialize module `%s` from file %s", module, file);
  return script;
}

ScriptResult interpret(Script script) {
  WrenInterpretResult status = wrenInterpret(__wrenVM, script.module.cString, script.contents);
  ScriptResult result;
  result.success = status == WREN_RESULT_SUCCESS;
  result.isCompilerError = status == WREN_RESULT_COMPILE_ERROR;
  result.isRuntimeError = status == WREN_RESULT_RUNTIME_ERROR;
  return result;
}
