#include "except.h"

namespace except {
void react() {
  try {
    throw;
  } catch(std::exception& e) {
    // обрабатываешь известные исключения
  } catch(...) {
    // обрабатываешь незивестные исключения
  }
}
}