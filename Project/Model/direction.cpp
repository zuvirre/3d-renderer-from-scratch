#include "direction.h"

namespace Renderer {
Direction::Direction(const Vector3D& vec) : data_(vec) {
}

Direction::Direction(const Vector4D& vec) : data_(vec.topLeftCorner<3,1>()) {
}

Direction& Direction::operator=(const Vector3D& coordinates) {
    data_ = coordinates;
    return *this;
}

Direction& Direction::operator=(const Vector4D& h_coordinates) {
    data_ = h_coordinates.topLeftCorner<3, 1>();
    return *this;
}

Vector3D Direction::GetCoordinates() const {
    return data_;
}

Vector4D Direction::GetHomogeneousCoordinates() const {
    Vector4D ans;
    ans.topLeftCorner<3, 1>() = data_;
    ans.w() = 0;
    return ans;
}
}