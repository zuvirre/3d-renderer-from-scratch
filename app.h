#pragma once
#include "Model/engine.h"
#include "Controller/controller.h"
#include "View/screen.h"

namespace Application {
void HandleException();

class Application {
    using Engine = kernel::Engine;
    using Screen = interface::Screen;
    using Controller = interface::Controller;
public:
    Application();
    void Run();
private:
    static constexpr int DefWidth = 1500;
    static constexpr int DefHeight =1500;

    Engine model_;
    Screen view_;
    Controller controller_;
};
}
