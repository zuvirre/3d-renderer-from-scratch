#include "AmbientLight.h"

namespace Renderer {
const Color AmbientLight::kDefaultColor{1,1,1};

double AmbientLight::GetIntensity() const {
    return intencity_;
}
}
