#include "except.h"

namespace except {
void HandleException() {
  try {
    throw;
  } catch(std::exception& e) {
    std::cerr << "Application failed with exception:" << e.what();
  } catch(...) {
    std::cerr << "Application failed with unknown exception";
  }
}
}
