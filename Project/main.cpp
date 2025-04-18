#include "app.h"
#include "except.h"
#include <iostream>

int main() {
    try {
        Application::Application app;
        app.Run();
    } catch(...) {
        except::HandleException();
    }   
    return 0;
}
