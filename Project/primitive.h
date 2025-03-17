#pragma once

#include "utility.h"
#include <Eigen/Dense>

namespace Renderer {
class Triangle {
public:
    using Vector3D = Eigen::Vector3d;
    using Mat34D = Eigen::Matrix<double, 3, 4>;
    using Mat3D = Eigen::Matrix3d;
    using ColorFunction = const std::function<Color(const Triangle &, const Vector3D &)>;
    using NormalFunction = const std::function<Vector3D(const Triangle &, const Vector3D &)>;

    Triangle() = default;
    Triangle(Mat3D);
    Vector3D CalculateCoordsFromBarycentric(const Vector3D&) const;
    Eigen::Vector3<Vertex> &GetVerts();
    const Eigen::Vector3<Vertex> &GetVerts() const;
    Mat3D GetVertsCoords() const;
    Mat34D GetVertsHomoCoords() const;
    const Vector3D &GetRealNormal() const;
    void CalculateNorm();

    Color GetAmbientColor(const Vector3D &b_coords) const;
    Color GetDiffuseColor(const Vector3D &b_coords) const;
    Color GetSpecularColor(const Vector3D &b_coords) const;

    Vector3D GetNormal(const Vector3D &b_coords) const;
    void SetColorFunction(const ColorFunction *ambient,
                          const ColorFunction *diffuse,
                          const ColorFunction *specular);
    void SetNormalFunction(const NormalFunction *normal);
private:
    Eigen::Vector3<Vertex> verticies_;
    Vector3D normal_;

    ColorFunction *ambient_color_function_ = nullptr;
    ColorFunction *diffuse_color_function_ = nullptr;
    ColorFunction *specular_color_function_ = nullptr;
    NormalFunction *normal_function_ = nullptr;
};
}
