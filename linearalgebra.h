#include <Eigen/Dense>
namespace Renderer {
    template <typename T>
    using Vector3 = Eigen::Vector3<T>;
    using Vector2D = Eigen::Vector2d;
    using Vector3D = Eigen::Vector3d;
    using Vector4D = Eigen::Vector4d;
    using Mat2D = Eigen::Matrix2d;
    using Mat3D = Eigen::Matrix3d;
    using Mat34D = Eigen::Matrix<double, 3, 4>;
    using Vector3I = Eigen::Vector3i;
}