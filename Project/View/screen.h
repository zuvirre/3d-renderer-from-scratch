#pragma once

#include "../Observer/Observer.h"
#include <memory>
#include "../Model/frame.h"
#include <SFML/Graphics.hpp>

namespace interface {
class Screen {
    using Color = Renderer::Color;
    using Frame = Renderer::Frame;
    static constexpr uint8_t kOpaque = 255;
    static constexpr uint8_t kTransparent = 255;
    struct RGBa {
        uint8_t R, G, B;
        uint8_t a = kOpaque;
        static RGBa FromColor(Color color);
    };
public:
    using RecievePortType = observer::CObserver<const Frame>;
    Screen(int width, int height);
    bool GetEvent(sf::Event &event);
    RecievePortType *GetRecievePort();
private:
    static RGBa ColorToRGBa(const Color &color);
    void Draw(const Frame &);
    sf::Image MakeImage(const Frame &matrix);

    RecievePortType recieve_port_;
    sf::RenderWindow window_;
    sf::Texture render_texture_;
    sf::Sprite render_sprite_;
};
}
