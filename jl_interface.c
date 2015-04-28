#include "base.h"

void dll_load() {
  DLL_LOAD
  if (DLL_LOAD_STATUS < 0) {
    printf("error loading library\n");
    exit(EXIT_FAILURE);
  }
}

void dll_close() {
  DLL_CLOSE
}
