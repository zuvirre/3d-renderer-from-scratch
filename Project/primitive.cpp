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

Triangle::Mat34D Triangle::GetVertsHomoCoords() const {
    Mat34D ans;
    for (int i = 0; i < 3; ++i) {
        ans.row(i) = verticies_[i].coordinates.GetHomogeneousCoordinates();
    }
    return ans;
}

const Triangle::Vector3D &Triangle::GetRealNormal() const {
    return normal_;
}

void Triangle::CalculateNorm() {
    normal_ =
        (verticies_[0].coordinates.GetCoordinates() - verticies_[1].coordinates.GetCoordinates())
            .cross(verticies_[0].coordinates.GetCoordinates() -
                   verticies_[2].coordinates.GetCoordinates()).normalized();
    normal_ = -normal_;
}

Triangle::Vector3D Triangle::GetNormal(const Vector3D &b_coords) const {
    if (!normal_function_) {
        return normal_;
    }
    return (*normal_function_) (*this, b_coords);
}

void Triangle::SetColorFunction(const ColorFunction *ambient, const ColorFunction *diffuse,
                                const ColorFunction *specular) {
    ambient_color_function_ = ambient;
    diffuse_color_funcion_ = diffuse;
    specular_color_function_ = specular;
}

void Triangle::SetNormalFunction(const NormalFunction *normal) {
    normal_function_ = normal;
}
}