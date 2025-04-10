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
        return ans;
}

Vector3<Vertex> &Triangle::GetVerts() {
    return verticies_;
}

const Vector3<Vertex> &Triangle::GetVerts() const {
    return verticies_;
}

Mat3D Triangle::GetVertsCoords() const {
    Mat3D ans;
    ans.row(0) = verticies_.x().coordinates.GetCoordinates();
    ans.row(1) = verticies_.y().coordinates.GetCoordinates();
    ans.row(2) = verticies_.z().coordinates.GetCoordinates();
    return ans;
}

Mat34D Triangle::GetVertsHomoCoords() const {
    Mat34D ans;
    for (int i = 0; i < 3; ++i) {
        ans.row(i) = verticies_[i].coordinates.GetHomogeneousCoordinates();
    }
    return ans;
}

Color Triangle::GetAmbientColor(const Vector3D &b_coords) const {
    return (*ambient_color_function_)(*this, b_coords);
}
Color Triangle::GetDiffuseColor(const Vector3D &b_coords) const {
    return (*diffuse_color_function_)(*this, b_coords);
}
Color Triangle::GetSpecularColor(const Vector3D &b_coords) const {
    return (*specular_color_function_)(*this, b_coords);
}

const Vector3D &Triangle::GetRealNormal() const {
    return normal_;
}

void Triangle::CalculateNorm() {
    normal_ =
        (verticies_[0].coordinates.GetCoordinates() - verticies_[1].coordinates.GetCoordinates())
            .cross(verticies_[0].coordinates.GetCoordinates() -
                   verticies_[2].coordinates.GetCoordinates()).normalized();
    normal_ = -normal_;
}

Vector3D Triangle::GetNormal(const Vector3D &b_coords) const {
    if (!normal_function_) {
        return normal_;
    }
    return (*normal_function_) (*this, b_coords);
}

void Triangle::SetColorFunction(const ColorFunction *ambient, const ColorFunction *diffuse,
                                const ColorFunction *specular) {
    ambient_color_function_ = ambient;
    diffuse_color_function_ = diffuse;
    specular_color_function_ = specular;
}

void Triangle::SetNormalFunction(const NormalFunction *normal) {
    normal_function_ = normal;
}

//here starts BarycentricSystem

BarycentricSystem::BarycentricSystem(const Triangle &original,
                                     const Mat34D &transformed)
    : triangle_(original), new_coords_matrix_(transformed) {
    for (int i = 0; i < 3; ++i) {
        original_coords_matrix_.row(i) =
        triangle_.GetVerts()[i].coordinates.GetHomogeneousCoordinates();
    }
}

const Triangle &BarycentricSystem::GetTriangle() const {
    return triangle_;
}

const Mat34D &BarycentricSystem::GetOriginalCoordsMatrix() const {
    return original_coords_matrix_;
}

Mat2D BarycentricSystem::MakeBarycentricTransformationMatrix(const Mat3D &coords) {
    Mat2D inverse_mat;
    inverse_mat = coords.topLeftCorner<2, 2>().transpose();
    inverse_mat.col(0) -= coords.row(2).topLeftCorner<1, 2>();
    inverse_mat.col(1) -= coords.row(2).topLeftCorner<1, 2>();
    Mat2D result;
    result = inverse_mat.inverse().eval();
    return result;
}

Vector3D 
BarycentricSystem::TransformToBarycentric(const Mat2D &transformation_matrix,
                                          const Mat3D &coords, const Vector2D &point) {
    Vector3D result;
    result.topLeftCorner<2, 1>() = point;
    result.topLeftCorner<2, 1>() -= coords.row(2).topLeftCorner<1, 2>();
    result.topLeftCorner<2, 1>() = transformation_matrix * result.topLeftCorner<2, 1>();
    result.z() = 1 - result.x() - result.y();
    return result;
}



Vector3D
BarycentricSystem::GetOriginalCoordinates(const Vector3D &coords) const {
    return coords.transpose() * original_coords_matrix_.topLeftCorner<3, 3>();
}

Vector4D BarycentricSystem::GetNewCoordinates(const Vector3D &coords) const {
    return static_cast<Vector4D>(coords.transpose() * new_coords_matrix_);
}

Mat3D BarycentricSystem::GetTriangleCoordinates(const Vector3<Vector3D> &triangle) const {
    Mat3D result;
    result.row(0) = GetOriginalCoordinates(triangle.x());
    result.row(1) = GetOriginalCoordinates(triangle.y());
    result.row(2) = GetOriginalCoordinates(triangle.z());
    return result;
}

double BarycentricSystem::InterpolateZCoordinate(const Vector3D &coords) {
    return new_coords_matrix_.col(3).dot(coords);
}

Color BarycentricSystem::GetColor(const Vector3D &b_coords) const {
    return triangle_.GetAmbientColor(b_coords);
}

Vector3D BarycentricSystem::ConvertToBarycentricCoordinates(Vector2D vec) const {
    vec -= new_coords_matrix_.row(2).topLeftCorner<1,2>();
    Vector3D result;
    result.topLeftCorner<2, 1>() = transformation_matrix_ * vec;
    result.z() = 1 - result.x() - result.y();
    return result;
}

}
