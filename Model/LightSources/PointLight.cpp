#include "PointLight.h"

namespace Renderer {
const Color PointLight::DefaultColor{1,1,1};
double PointLight::GetIntencity(double distance) const {
    return initial_intencity_ * (1 / (constant_attenuation_ + linear_attenuation_ * distance + 
                                      quad_attenuation_ * distance * distance));
}

Color PointLight::GetColor() const {
    return color_ * is_on_flag_;
}

void PointLight::SetLight(double c_attenuation, double l_attenuation, double q_attenuation) {
    constant_attenuation_ = c_attenuation;
    linear_attenuation_ = l_attenuation;
    quad_attenuation_ = q_attenuation;
}

void PointLight::Switch() {
    is_on_flag_ = !is_on_flag_;
}
}
