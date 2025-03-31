#pragma once

#include "utility.h"
#include "point.h"
#include "direction.h"
#include <Eigen/Dense>

namespace Renderer {
class Triangle {
public:
    using ColorFunction = const std::function<Color(const Triangle &, const Vector3D &)>;
    using NormalFunction = const std::function<Vector3D(const Triangle &, const Vector3D &)>;

    Triangle() = default;
    Triangle(Mat3D);
    Vector3D CalculateCoordsFromBarycentric(const Vector3D&) const;
    Vector3<Vertex> &GetVerts();
    const Vector3<Vertex> &GetVerts() const;
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
    Vector3<Vertex> verticies_;
    Vector3D normal_;

    ColorFunction *ambient_color_function_ = nullptr;
    ColorFunction *diffuse_color_function_ = nullptr;
    ColorFunction *specular_color_function_ = nullptr;
    NormalFunction *normal_function_ = nullptr;
};


class BarycentricSystem {
public:
    BarycentricSystem(const Triangle &original, const Mat34D &transformed);
    
    const Triangle &GetTriangle() const;
    const Mat34D &GetOriginalCoordsMatrix() const;

    static Mat2D MakeBarycentricTransformationMatrix(const Mat3D &coords);
    static Vector3D TransformToBarycentric(const Mat2D &transformation_matrix, 
                                           const Mat3D &coords, const Vector2D &point);
    Vector3D GetOriginalCoordinates(const Vector3D &) const;
    Vector4D GetNewCoordinates(const Vector3D &) const;
    Mat3D GetTriangleCoordinates(const Vector3<Vector3D> &) const;
    double InterpolateZCoordinate(const Vector3D &);
    Color GetColor(const Vector3D &b_coords) const;
    Vector3D ConvertToBarycentricCoordinates(Vector2D) const;
    
private:
    Triangle triangle_;
    Mat34D original_coords_matrix_;
    Mat34D new_coords_matrix_;
    Mat2D transformation_matrix_;

};
}
