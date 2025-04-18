#include "point.h"

namespace Renderer {
    Point::Point(const Vector4D& data) {
        data_(data);
    }
    Point::Point(const Vector3D& data) {
        data_.topLeftCorner<3,1>() = data;
    }

    Vector3D Point::GetCoordinates() const{
        return data_.topLeftCorner<3,1>();
    }
    const Vector4D &Point::GetHomogeneousCoordinates() const {
        return data_;
    }

    double Point::w() {
        return data_.w();
    }

    Point &Point::operator=(const Vector4D& coords) {
        data_ = coords;
        return *this;
    }
    Point& Point::operator=(const Vector3D& coords) {
        data_.topLeftCorner<3,1>() = coords;
        data_.w() = 1;
        return *this;
    }
}