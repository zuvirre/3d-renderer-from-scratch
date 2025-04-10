#include "controller.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <iostream>

namespace interface {
Controller::Controller(Engine* model) : model_(model) {
}

bool Controller::HandleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            return true;
        case sf::Event::KeyPressed:
            return HandleKeyPressedEvent(event.key);
            break;
        case sf::Event::KeyReleased:
            HandleKeyReleasedEvent(event.key);
            break;
        default:
            break;
    }
    return false;
}
void Controller::HandleStallEvent() {
    HandleKeys();
}
bool Controller::HandleKeyPressedEvent(const sf::Event::KeyEvent& event) {
    switch (event.code) {

        case sf::Keyboard::BackSpace:
            return true;
        case sf::Keyboard::Up:
            pressed_keys_.push_back(sf::Keyboard::Up);
            break;
        case sf::Keyboard::Down:
            pressed_keys_.push_back(sf::Keyboard::Down);
            break;
        case sf::Keyboard::Right:
            pressed_keys_.push_back(sf::Keyboard::Right);
            break;
        case sf::Keyboard::Left:
            pressed_keys_.push_back(sf::Keyboard::Left);
            break;
        case sf::Keyboard::Space:
            pressed_keys_.push_back(sf::Keyboard::Space);
            break;
        case sf::Keyboard::Z:
            pressed_keys_.push_back(sf::Keyboard::Z);
            break;
        case sf::Keyboard::W:
            pressed_keys_.push_back(sf::Keyboard::W);
            break;
        case sf::Keyboard::S:
            pressed_keys_.push_back(sf::Keyboard::S);
            break;
        case sf::Keyboard::D:
            pressed_keys_.push_back(sf::Keyboard::D);
            break;
        case sf::Keyboard::A:
            pressed_keys_.push_back(sf::Keyboard::A);
            break;
        case sf::Keyboard::L:
            model_->SwitchLightingModel();
            model_->Update();
            break;
        case sf::Keyboard::O:
            model_->SwitchObject();
            break;
        case sf::Keyboard::C:
            model_->SwitchCamera();
            break;
        case sf::Keyboard::P:
            model_->SwitchLight();
            break;
        case sf::Keyboard::E:
            model_->AddPointLight();
            model_->Update();
            break;
        case sf::Keyboard::Q:
            double c_attenuation, l_attenuation, q_attenuation;
            std::cout << "Enter light parameters" << std::endl;
            std::cout << "Constant attenuation:" << std::endl;
            std::cin >> c_attenuation;
            std::cout << "Linear attenuation:" << std::endl;
            std::cin >> l_attenuation;
            std::cout << "Quadratic attenuation:" << std::endl;
            std::cin >> q_attenuation;
            model_->ChangeSelectedLight(c_attenuation, l_attenuation, q_attenuation);
            model_->Update();
            break;
        case sf::Keyboard::I:
            model_->ToggleSelectedLight();
            model_->Update();
            break;
        case sf::Keyboard::Num1:
            model_->ToggleLightMarkers();
            model_->Update();
            break;
        default:
            break;
    }
    return false;
}
void Controller::HandleKeyReleasedEvent(const sf::Event::KeyEvent& event) {
    auto it = pressed_keys_.begin();
    while (it != pressed_keys_.end()) {
        if (*it == event.code) {
            pressed_keys_.erase(it);
            break;
        }
        ++it;
    }
}
void Controller::HandleKeys() {
    for (const auto& key : pressed_keys_) {
        switch (key) {
            case sf::Keyboard::Up:
                model_->TiltUp(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Down:
                model_->TiltDown(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Right:
                model_->TiltRight(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Left:
                model_->TiltLeft(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Space:
                model_->MoveUp(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::Z:
                model_->MoveDown(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::W:
                model_->MoveForward(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::S:
                model_->MoveBackward(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::D:
                model_->MoveRight(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::A:
                model_->MoveLeft(kDefaultMove, model_->GetSelected());
                break;
            default:
                break;
        }
    }
    if (!pressed_keys_.empty()) {
        model_->Update();
    }
}
void Controller::InitEnviroment() {
    std::cout << "Enter number of 3D enviroment:" << std::endl;
    std::cout << "1. Spheres showcase" << std::endl;
    std::cout << "2. Custom enviroment" << std::endl;
    int input;
    std::cin >> input;
    switch (input) {
        case 1:
            model_->InitializeSphereEnv();
            return;
        case 2:
            model_->InitializeUserEnv();
            return;
        default:
            std::cout << "Invalid enviroment number" << std::endl;
            InitEnviroment();
            return;
    }
}
}
