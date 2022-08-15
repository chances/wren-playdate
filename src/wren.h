//  Created by Chance Snow on 8/14/22
//  Copyright © 2022 Snow Developments, LLC. All rights reserved.
#pragma once

#include <stdbool.h>
#include <wren.h>
#include <pd_api.h>

#include "lib/string.h"

typedef struct {
  size_t size;
  String module;
  String file;
  const char* contents;
} Script;

typedef struct {
  WrenErrorType type;
  String module;
  int line;
  String message;
} ScriptError;

typedef struct {
  bool success;
  bool isCompilerError;
  bool isRuntimeError;
} ScriptResult;

bool initWrenRuntime(PlaydateAPI* pd);
void destroyWrenRuntime(void);
Script loadScript(char* module, char* file);
