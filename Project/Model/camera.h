#pragma once

#include "Eigen/Dense"
#include <array>
#include "primitive.h"
#include "direction.h"

namespace Renderer {
class Camera {
public:
    Camera(int width, int height);
    Mat34D PerspectiveTransformation(const Mat34D &) const;
    Vector4D InversePerspectiveTransformation(const Vector4D &) const;
    const Mat54D &GetClippingPlanes() const;
private:
    void InitFromConst(int width, int height);
    void InitPlanes();
    void InitPerspective();
    
    Mat54D planes_;
    double right_, left_, top_, bottom_;
    Mat4D perspective_matrix_ = Mat4D::Zero();
    Mat4D inverse_perspective_matrix_ = Mat4D::Zero();

    static constexpr double DefaultFrontPlaneDist = 2;
    double front_plane_dist_ = DefaultFrontPlaneDist;
    double field_of_view_ = -1;
};
}
