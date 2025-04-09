#include <cassert>
#include "camera.h"

namespace Renderer {
void Camera::InitFromConst(int width, int height) {
    double d_width = static_cast<double>(width);
    double d_height = static_cast<double>(height);
    assert("Width and Height are positive values:" && d_width > 0 && d_height > 0);
    double ratio = d_height / d_width;
    left_ = -1;
    right_ = 1;
    bottom_ = -ratio;
    top_ = ratio;
    field_of_view_ = 2.0 * std::atan(1 / front_plane_dist_);
}

void Camera::InitPlanes() {
    double focal_length = front_plane_dist_;
    double x_coef = std::sqrt(focal_length * focal_length + 1);
    double y_coef = std::sqrt(focal_length * focal_length + top_ * top_);
    planes_ << 0, 0, -1.0, front_plane_dist_,
               focal_length / x_coef, 0, -1.0 / x_coef, 0,
               -focal_length / x_coef, 0, -1.0 / x_coef, 0,
               0, focal_length / y_coef, -top_ / y_coef, 0,
               0, -focal_length / y_coef, -top_ / y_coef, 0;
}

void Camera::InitPerspective() {
    perspective_matrix_(0, 0) = (2.0 * front_plane_dist_ / (right_ - left_));
    perspective_matrix_(0, 2) = ((right_ + left_) / (right_ - left_));
    perspective_matrix_(1, 1) = (2.0 * front_plane_dist_ / (top_ - bottom_));
    perspective_matrix_(1, 2) = ((top_ + bottom_) / (top_ - bottom_));
    perspective_matrix_(2, 2) = -1;
    perspective_matrix_(2, 3) = -2.0 * front_plane_dist_;
    perspective_matrix_(3, 2) = -1;
    inverse_perspective_matrix_ = perspective_matrix_.inverse();
}

Camera::Camera(int width, int height) {
    InitFromConst(width, height);
    InitPlanes();
    InitPerspective();
}

Mat34D Camera::PerspectiveTransformation(const Mat34D& vertices) const {
    Mat34D result = vertices;
    result = (perspective_matrix_ * result.transpose()).transpose();
    for (int i = 0; i < 3; ++i) {
        assert("W coordinate is non-zero" && result.row(i).w() != 0);
        result.row(i).topLeftCorner<1, 3>() /= result.row(i).w();
    }
    return result;
}

Vector4D Camera::InversePerspectiveTransformation(const Vector4D& vec) const {
    Vector4D result = vec;
    result.topLeftCorner<3, 1>() *= result.w();
    return inverse_perspective_matrix_ * result;
}

const Mat54D& Camera::GetClippingPlanes() const {
    return planes_;
}

}
