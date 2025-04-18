#include "screen.h"
#include <cmath>

namespace interface {
Screen::RGBa Screen::RGBa::FromColor(Color color) {
    RGBa result;
    result.R = color.GetRedInt();
    result.G = color.GetGreenInt();
    result.B = color.GetBlueInt();
    return result;
}
Screen::Screen(int width, int height)
    : recieve_port_(
          RecievePortType::doNothing, [this](const Frame& frame) { Draw(frame); },
          RecievePortType::doNothing),
      window_(sf::VideoMode(width, height), "Renderer") {
    window_.setKeyRepeatEnabled(false);
}

void Screen::Draw(const Frame& frame) {
    render_texture_.loadFromImage(MakeImage(frame));
    render_sprite_.setTexture(render_texture_);
    window_.draw(render_sprite_);
    window_.display();
}
sf::Image Screen::MakeImage(const Frame& frame) {
    sf::Image image;
    image.create(frame.GetWidth(), frame.GetHeight());
    for (int i = 0; i < frame.GetWidth(); ++i) {
        for (int j = 0; j < frame.GetHeight(); ++j) {
            RGBa color = RGBa::FromColor(frame.GetPixel(j, i));
            image.setPixel(i, j, sf::Color(color.R, color.G, color.B, color.a));
        }
    }
    return image;
}
bool Screen::GetEvent(sf::Event& event) {
    return window_.pollEvent(event);
}
Screen::RecievePortType* Screen::GetRecievePort() {
    return &recieve_port_;
}
}
