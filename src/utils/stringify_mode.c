#include "main.h"

const char* stringify_mode(EditorMode mode) {
  switch (mode) {
    case NORMAL:
      return "-- NORMAL --";
    case INSERT:
      return "-- INSERT --";
    default:
      return "-- NORMAL --";
  }
}
