#include "app.h"
#include <iostream>
namespace Application {

Application::Application()
    : model_(DefWidth, DefHeight),
      view_(DefWidth, DefHeight),
      controller_(&model_) {
    model_.Subscribe(view_.GetRecievePort());
    controller_.InitEnviroment();
}
void Application::Run() {
    bool closed = false;
    sf::Event event;
    while (!closed) {
        if (view_.GetEvent(event)) {

            closed = controller_.HandleEvent(event);
            if (closed) {
                break;
            }
        } else {
            controller_.HandleStallEvent();
        }
    }
}

}
