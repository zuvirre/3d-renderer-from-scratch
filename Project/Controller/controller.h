#pragma once

#include "../Observer/Observer.h"
#include "../Model/frame.h"
#include "../Model/engine.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace interface {
class Controller {
    using Engine = kernel::Engine;

public:
    Controller(Engine* model);
    bool HandleEvent(const sf::Event&);    
    void InitEnviroment();
    void HandleStallEvent();
    
private:
    bool HandleKeyPressedEvent(const sf::Event::KeyEvent&);
    void HandleKeyReleasedEvent(const sf::Event::KeyEvent&);
    void HandleKeys();
    static constexpr double kDefaultRotation = 0.02;
    static constexpr double kDefaultMove = 0.1;
    std::list<sf::Keyboard::Key> pressed_keys_;
    Engine* model_;    
};
}
