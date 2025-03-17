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
Triangle::Vector3D Triangle::CalculateCoordsFromBarycentric(
    const Vector3D &coordinates) const {
        Vector3D ans = Vector3D::Zero();
        ans = verticies_.x().coordinates.GetCoordinates() * coordinates.x() +
              verticies_.y().coordinates.GetCoordinates() * coordinates.y() +
              verticies_.z().coordinates.GetCoordinates() * coordinates.z();
        return ans;
}

Eigen::Vector3<Vertex> &Triangle::GetVerts() {
    return verticies_;
}

const Eigen::Vector3<Vertex> &Triangle::GetVerts() const {
    return verticies_;
}

Triangle::Mat3D Triangle::GetVertsCoords() const {
    Mat3D ans;
    ans.row(0) = verticies_.x().coordinates.GetCoordinates();
    ans.row(1) = verticies_.y().coordinates.GetCoordinates();
    ans.row(2) = verticies_.z().coordinates.GetCoordinates();
    return ans;
}

triangle::Mat34D Triangle::GetVertsHomoCoords() const {
    Mat34D ans;
    for (int i = 0; i < 3; ++i) {
        ans.row(i) = verticies_[i].coordinates.GetHomogeneousCoordinates();
    }
    return ans;
}

}