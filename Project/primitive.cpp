#include "primitive.h"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace Renderer {
Triangle::Triangle(Mat3D coords) {
    verticies_.x().coordinates = coords.row(0);
    verticies_.y().coordinates = coords.row(1);
    verticies_.z().coordinates = coords.row(2);
}
Vector3D Triangle::CalculateCoordsFromBarycentric(
    const Vector3D &coordinates) const {
        Vector3D ans = Vector3D::Zero();
        ans = verticies_.x().coordinates.GetCoordinates() * coordinates.x() +
              verticies_.y().coordinates.GetCoordinates() * coordinates.y() +
              verticies_.z().coordinates.GetCoordinates() * coordinates.z();
    }
)
}
