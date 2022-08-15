#pragma once

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

typedef struct {
  size_t length;
  const char* cString;
} String;

#define NilString (String) { 0, NULL }

String toString(const char* string);
String intToString(const int integer);
String joinStrings(const char* separator, size_t argsLength, ...);
#define concatStrings(argsLength, ...) joinStrings("", argsLength, __VA_ARGS__)
