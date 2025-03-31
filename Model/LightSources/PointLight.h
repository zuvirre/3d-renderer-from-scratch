#pragma once

#include "utility.h"

namespace Renderer {

class PointLight {
public:
    double GetIntencity(double distance) const;
    Color GetColor() const;
    void SetLight(double c_attenuation, double l_attenuation, double q_attenuation);
    void Switch();
private:
    static constexpr double DefaultIntencity = 1;
    static constexpr double DefaultConstantAttenuation = 1;
    static constexpr double DefaultLinearAttenuation = 0.4;
    static constexpr double DefaultQuadraticAttenuation = 0.02;
    static const Color DefaultColor;

    double initial_intencity_ = DefaultIntencity;
    double constant_attenuation_ = DefaultConstantAttenuation;
    double linear_attenuation_ = DefaultLinearAttenuation;
    double quad_attenuation_ = DefaultQuadraticAttenuation;
    Color color_ = DefaultColor;
    bool is_on_flag_ = true;
};
}
