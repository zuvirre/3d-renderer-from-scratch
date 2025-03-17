#pragma once

#include "utility.h"
#include <Eigen/Dense>

namespace Renderer {
class Triangle {
public:
    using Vector3D = Eigen::Vector3d;
    using Mat34D = Eigen::Matrix<double, 3, 4>;
    using Mat3D = Eigen::Matrix3d;

    Triangle() = default;
    Triangle(Mat3D);
    Vector3D CalculateCoordsFromBarycentric(const Vector3D&) const;
    Eigen::Vector3<Vertex> &GetVerts() const;
    Mat3D GetVertsCoords() const;
    Mat34D GetVertsHomoCoords() const;

    Vector3D GetNormal(const Vector3D &b_coords) const;
    const Vector3D &GetRealNormal() const;
    void CalculateNorm();
private:
     Eigen::Vector3<Vertex> verticies_;
    Vector3D normal_;
};
}
