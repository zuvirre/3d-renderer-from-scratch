#pragma once

#include "../utility.h"

namespace Renderer {
class AmbientLight {
public:
    double GetIntensity() const;
private:
    static constexpr double kDefaultIntenstity = 0.2;
    static const Color kDefaultColor;
    double intencity_ = kDefaultIntenstity;
    Color color_ = kDefaultColor;
};
}
